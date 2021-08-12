//
// Created by Mars on 2021/8/12.
//

#include "unp.h"

int main(int argc, char **argv) {
    //判断参数格式
    if (argc != 2)
        err_quit("usage: %s <IPAddress>", argv[0]);

    //打开套接字
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1)
        err_sys("socket error");

    //设置套接字
    struct sockaddr_in6 servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(3389);
    int ret = inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr);
    if (ret <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    //连接服务器
    errno = 0;
    ret = connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    if (ret == -1)
        err_quit("connect error");

    //get local ipv6 address
    struct sockaddr_in6 cliaddr;
    int len = sizeof(struct sockaddr_in6);
    getsockname(sockfd, (SA *) &cliaddr, &len);
    char ipv6[MAXLINE];
    fprintf(stderr, "Connection from dev:[%s] at port:[%d]",
            inet_ntop(AF_INET6, &cliaddr.sin6_addr, ipv6, MAXLINE),
            ntohs(cliaddr.sin6_port));

    //关闭套接字
    close(sockfd);
    exit(0);
}