/*standard c++ library*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>

/*Basic Unix headers*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef ZMQ
#include <zmq.h>
#endif

#define LISTENQ  1024  /* second argument to listen() */
#define WAITING 1
#define ACTIVE 2

#define MAX_BUFF_SIZE 4096

/* $begin rio_t */
#define RIO_BUFSIZE 8192
struct rio_t{
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
    rio_t(int fd) 
	{
	    rio_fd = fd;  
	    rio_cnt = 0;  
	    rio_bufptr = rio_buf;
	}
};
/* $end rio_t */

typedef struct sockaddr SA;

int open_clientfd(char *hostname, int port);
int open_listenfd(int port);
int rio_readlineb(rio_t *rp, void *usrbuf, int maxlen);
int rio_writen(int fd, char *usrbuf, size_t n);
int rio_read(rio_t *rp, char *usrbuf, size_t n);

void P(sem_t *sem);
void V(sem_t *sem);