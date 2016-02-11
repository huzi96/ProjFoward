#include "header.hpp"
using namespace std;

struct Client
{
	int fd1;
	int fd2;
	int id;
	int status;
	Client(int _fd, int _id, int st):fd1(_fd),fd2(-1),id(_id),status(st){}
	Client(){}
};

class thread_parameter
{
public:
    int fd1, fd2;
    int id;
    thread_parameter(int p1, int p2, int _id):fd1(p1),fd2(p2),id(_id){}
};

class daemon_args:public thread_parameter
{
public:
	daemon_args(int p1, int p2):thread_parameter(p1,p2,0){}
};

void * wait_procedure(void *vargp);
void * daemon(void *vargp);

/*Global set as a queue for waiting clients*/
map<int, Client> waiting_queue;

sem_t transmit;

int main(int argc, char *argv[])
{
	sem_init(&transmit, 0, 1);
	if (argc != 2) {
    	printf("usage: %s <port>\n", argv[0]);
    	exit(1);
    }
    
    int port = atoi(argv[1]);
	printf("[Notice] starting listening at port %d\n", port);
	int listenfd = open_listenfd(port);
	struct sockaddr_in clientaddr;


	while(1) {
    	int clientlen = sizeof(clientaddr);
    	int connfd = accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);
    	/*Now read the first request*/

    	rio_t * rp = new rio_t(connfd);
    	char head[16]={0};
    	rio_readlineb(rp, head, 16);
    	char idBuff[16]={0};
    	rio_readlineb(rp, idBuff, 16);
    	int id = atoi(idBuff);
    	printf("[Notice] triggerred by id %d with method %s", id, head);
    	if (strcmp(head, "WAIT\r\n")==0)
    	{
    		if (waiting_queue.find(id) == waiting_queue.end())
    		{
    			waiting_queue.insert(make_pair(id, Client(connfd, id, WAITING)));
    		}
    		else if (waiting_queue[id].status == WAITING)
    		{
    			Client &cl = waiting_queue[id];
    			cl.status = ACTIVE;
    			cl.fd2 = connfd;
    			pthread_t tid;
    			thread_parameter * para = new thread_parameter(cl.fd1, cl.fd2, id);
    			pthread_create(&tid, NULL, wait_procedure, para);
    		}
    		else
    		{
    			printf("[Notice] Occupied id\n");
    		}
      	}
      	else if (strcmp(head, "RSET\r\n")== 0)	
      	{
      		printf("[Notice] Doing reset\n");
      		map<int, Client>::iterator iter, end = waiting_queue.end();
      		for (iter = waiting_queue.begin(); iter!=end; ++iter)
      		{
      			if(iter->second.status == WAITING)
      			{
      				close(iter->second.fd1);
      				waiting_queue.erase(iter);
      			}
      			else if(iter->second.status == ACTIVE)
      			{
      				close(iter->second.fd2);
      				waiting_queue.erase(iter);
      			}
      		}
      		close(connfd);
      		if(waiting_queue.size() == 0)
      			printf("[Notice] Reset complete\n");
      		else
      			printf("[Error] Reset Fail\n");

      	}
      	delete rp;
    }
}

void * wait_procedure(void *vargp)
{
	pthread_detach(pthread_self());
	thread_parameter *para = (thread_parameter*) vargp;
	printf("[Notice] matching %d and %d\n", para->fd1, para->fd2);

	pthread_t *tid1 = new pthread_t, *tid2=new pthread_t;
	daemon_args *arg1 = new daemon_args(para->fd1, para->fd2);
	daemon_args *arg2 = new daemon_args(para->fd2, para->fd1);
	pthread_create(tid1, NULL, daemon, arg1);
	pthread_create(tid2, NULL, daemon, arg2);

	pthread_join(*tid1, NULL);
	pthread_join(*tid2, NULL);
	delete tid1;
	delete tid2;

	delete arg1;
	delete arg2;

	waiting_queue.erase(para->id);
	close(para->fd1);
	close(para->fd2);
	printf("[Notice] Clearing dead connection with id %d and fd %d %d\n"
		, para->id, para->fd1, para->fd2);
	return NULL;
}

void * daemon(void *vargp)
{
	int errorcnt = 0;
	pthread_detach(pthread_self());
	daemon_args *args = (daemon_args*) vargp;
	/*Will read from fd1 and write to fd2*/
	rio_t * rp = new rio_t(args->fd1);
	
	printf("[Notice] One daemon created hearing %d\n", args->fd1);
	rio_writen(args->fd1, "ACTV\r\n", 6);
	while(true)
	{
		char head[16]={0};
    	int status = rio_readlineb(rp, head, 16);
    	if (status == -1)
    	{
    		printf("[Error] header error\n");
    		pthread_exit(NULL);
    	}
    	if (errorcnt>10)
    	{
    		pthread_exit(NULL);
    	}
		if (strcmp(head, "SEND\r\n") == 0)
		{

			printf("[Notice] Send signal received\n");

			/*read the id*/
			char idBuff[16]={0};
    		rio_readlineb(rp, idBuff, 16);
    		/*read the length of the data*/
    		char lenBuff[16]={0};
    		rio_readlineb(rp, lenBuff, 16);
    		int len = atoi(lenBuff);
    		/*there must be just one transmition at one time*/
    		/*we will fix this later*/
			P(&transmit);
			int readlen = 0;
			char *buf = new char[len];
			int cnt_flag = 0;
			int writefd = args->fd2;

			printf("[Notice] start to write to %d with %d bytes\n", writefd, len);
			int readsize = len;
			while(1) {
				cnt_flag++;
				readlen = rio_read( rp, buf, readsize);
				printf("[Notice] readlen = %d\n", readlen);
				if (readlen == -1) {
					printf("[Error] read error\n");
					delete buf;
					pthread_exit(NULL);
				}
				if (readlen < readsize && readlen > 0) {
					rio_writen(writefd, buf, readlen);
					readsize-=readlen;
				}
				else
				{
					rio_writen(writefd, buf, readlen);
					break;
				}
			}
			printf("[Notice] break\n");

			V(&transmit);
			delete buf;
			errorcnt = 0;
		}
		else 
		{
			printf("[Error] Invalid header\n");
			errorcnt++;
		}
	}
	delete rp;
	return NULL;
}
