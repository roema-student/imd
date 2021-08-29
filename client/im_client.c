#include <stdio.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "logger.h"

#define SA struct sockaddr
#define MAXLINE 4096
#define SERV_PORT 8000

ssize_t Readline(int fd, void *ptr, size_t maxlen);

void str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	while (fgets(sendline, MAXLINE, fp) != NULL) {
		write(sockfd, sendline, strlen(sendline));
		if (Readline(sockfd, recvline, MAXLINE) == 0) {
            logger(LOG_ERROR, "str_cli: server terminated prematurely");
        }

		fputs(recvline, stdout);
	}
}


int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
    int conn_result;

	if (argc != 2) {
        printf("usage: im_cli <IPaddress>\n");
        exit(1);
    }
		

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
    
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	conn_result = connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    if (0 != conn_result) {
        logger(LOG_ERROR, "conn_result %d:%s", errno, strerror(errno));
        exit(1);
    }
    logger(LOG_INFO, "connected, input now");
	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}