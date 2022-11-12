//
// Created by Administrator on 2022/11/5.
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

void error_handling(char *msg);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    char buf[BUF_SIZE] = {0,};
    int str_len, i;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    FILE *readfp;
    FILE *writefp;
    if (argc != 2) {
        fprintf(stdout, "Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (-1 == bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
        error_handling("bind error!");

    if (-1 == listen(serv_sock, 5))
        error_handling("listen error!");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept error!");

    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(dup(clnt_sock), "w");
    fputs("FROM SERVER: Hi! client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}