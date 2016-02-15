#include "header.hpp"

using namespace std;
char buf[4096];
char *host = "localhost";
int main(int argc, char *argv[], char *env[])
{
	/*Setting up connection*/
	int clientfd = open_clientfd(host, 8001);
	char str[64] = "WAIT\r\n123\r\n";
	write(clientfd, str, strlen(str));
	char sbuf[16]={0};
	read(clientfd, sbuf, 10);
	if (strcmp(sbuf, "ACTV\r\n") != 0)
	{
		printf("[Error] Connection Fail\n");
		exit(0);
	}

	/*set up pipe*/
	int infd = open("tmph264", O_RDONLY);
	assert(infd!=-1);
	while(true)
	{
		long len = read(infd, buf, 4096);
		if (len <= 0)
		{
			break;
		}
		stringstream ss;
		ss<<"SEND\r\n123\r\n"<<len<<"\r\n";
		write(clientfd, ss.str().c_str(), ss.str().size());
		write(clientfd, buf, len);
		printf("[Notice] Write %d\n", len);
	}
	close(infd);
}