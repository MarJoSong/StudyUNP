//
// Created by Mars on 2021/8/10.
//

#include "unp.h"

int main(int argc, char **argv) {
  //打开套接字
  int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  //设置(监听)套接字
  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(7692);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  //绑定监听套接字
  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

  //转换套接字为监听状态
  listen(listenfd, LISTENQ);

  //等待连接
  for (;;) {
    int connfd = Accept(listenfd, NULL, NULL); //服务端可以不关心客户端的信息
    time_t ticks = time(NULL);

    char buf[MAXLINE];
    snprintf(buf, MAXLINE, "%.24s\r\n", ctime(&ticks));
    Write(connfd, buf, strlen(buf));
    Close(connfd);
  }
}

