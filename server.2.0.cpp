#include "header.cpp"
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
    thread_parameter(int p1,int p2):fd1(p1),fd2(p2){}
};

class daemon_args:public thread_parameter
{
public:
    daemon_args(int p1, int p2):thread_parameter(p1,p2){}
};

void * wait_procedure(void *vargp);
void * daemon(void *vargp);

/*Global set as a queue for waiting clients*/
map<int, Client> waiting_queue;

sem_t transmit;

int main (int argv, char *argv[])  
{
    if (argc != 2) {
        printf("usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    string port(argv[1]);
    cout<<"[Notice] starting listening at port "<<port<<endl;

    // Socket to talk to clients  
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:"+port);
    assert(rc == 0);

    while(1){
        char buffer[16]={0};
        zmq_recv(responder, buffer, 14, 0);
        
        stringstream ss;
        ss<<buffer;
        string method;
        int id;
        ss>>method>>id;

        if (method == "WAIT")
        {

        }
        zmq_send (responder, "World", 5, 0);
        sleep (1); // Do some 'work'
    }
    return 0;
}