//
// Created by Administrator on 2022/11/5.
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 30

void error_handling(char *msg);

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_addr_size;
    struct sockaddr_in serv_addr, clnt_addr;

    if (argc != 2) {
        fprintf(stdout, "Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (-1 == bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
        error_handling("bind error!");

    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0,
                           (struct sockaddr *) &clnt_addr, &clnt_addr_size);
        sendto(sock, message, str_len, 0,
               (const struct sockaddr *) &clnt_addr, clnt_addr_size);
    }
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}