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

#include "logger.h"

#define SERV_PORT 8000
#define LISTENQ 64
#define SA struct sockaddr
#define MAXLINE 4096

void err_sys(const char* msg) {
    logger(LOG_ERROR, msg);
    exit(1);
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        logger(LOG_INFO, "child %d terminated\n", pid);
    }
    return;
}

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];

again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0)
        write(sockfd, buf, n);

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        err_sys("str_echo: read error");
}

int main() {
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    int bind_result = 0;
    struct sockaddr_in cliaddr, servaddr;

    void sig_chld(int);

    logger(LOG_DEBUG, "Hello server\n");

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind_result = bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    if (bind_result != 0 ){
        logger(LOG_ERROR, "bind result : %d\n", bind_result);
        exit(1);
    }
    listen(listenfd, LISTENQ);

    signal(SIGCHLD, sig_chld);

    for (;;) {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (SA *)&cliaddr, &clilen)) < 0) {
            if (errno == EINTR)
                continue; /* back to for() */
            else
                err_sys("accept error");
        }

        if ((childpid = fork()) == 0) {                     /* child process */
            close(listenfd);  /* close listening socket */
            str_echo(connfd); /* process the request */
            exit(0);
        }
        close(connfd); /* parent closes connected socket */
    }

    return 0;
}