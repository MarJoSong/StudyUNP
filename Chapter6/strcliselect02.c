//
// Created by mars on 2021/8/11.
//
#include "unp.h"


/*
 * 与strcliselect01相比，select后增加了标准输入是否结束的检测
 * 当标准输入结束后，将标准输入从rset中移除并关闭标准输入的读写，同时设置stdineof为1
 * 这样在检测接受描述符的时候，如果返回0，再检测是不是因为标准输入shutdown导致的
 */

void my_str_cli(FILE *fp, int sockfd) {
  int maxfd, stdineof;
  fd_set rset;
  char buf[MAXLINE];
  int n;

  stdineof = 0;
  FD_ZERO(&rset);
  for (;;) {

    if (stdineof == 0)
      FD_SET(fileno(fp), &rset);

    FD_SET(sockfd, &rset);
    maxfd = max(fileno(fp), sockfd) + 1;
    Select(maxfd, &rset, NULL, NULL, NULL);

    if (FD_ISSET(sockfd, &rset)) {
      bzero(buf, MAXLINE);
      if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
        if (stdineof == 1)
          return;
        else
          err_quit("str_cli: server terminated permaturely");
      }
      Write(fileno(stdout), buf, n);
    }

    if (FD_ISSET(fileno(fp), &rset)) {
      if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
        stdineof = 1;
        shutdown(fileno(fp), SHUT_WR);
        FD_CLR(fileno(fp), &rset);
        continue;
      }
      Write(sockfd, buf, n);
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2)
    err_quit("usage: %s <IPAddress>", argv[0]);

  int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  my_str_cli(stdin, sockfd);

  exit(0);
}
