#include "header.hpp"
int main()
{
	int clientfd = open_clientfd("10.211.55.7", 8001);
	char str[64] = "WAIT\r\n123\r\n";
	write(clientfd, str, strlen(str));
	char buf[16]={0};
	read(clientfd, buf, 10);
	printf("%s\n", buf);
	if (strcmp(buf, "ACTV\r\n") == 0)
	{
		strcpy(str, "SEND\r\n123\r\n10\r\nmichael4");
		write(clientfd, str, strlen(str));
	}
	close(clientfd);
	getchar();
}