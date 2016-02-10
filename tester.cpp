#include "header.hpp"
int main()
{
	int clientfd = open_clientfd("localhost", 8001);
	char str[] = "WAIT\r\n123\r\nSEND\r\n123\r\n4\r\nhuzi\r\n";
	write(clientfd, str, strlen(str));
	char buf[16];
	read(clientfd, buf, 10);
	close(clientfd);
}