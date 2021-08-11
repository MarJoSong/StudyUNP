//
// Created by Mars on 2021/8/10.
//

#include "unp.h"

int main(int argc, char **argv) {
  //判断参数格式
  if (argc != 2)
    err_quit("usage: %s <IPAddress>", argv[0]);

  //打开套接字
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    err_sys("socket error");

  //设置套接字
  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(7692);
  int ret = inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  if (ret <= 0)
    err_quit("inet_pton error for %s", argv[1]);

  //连接服务器
  errno = 0;
  ret = connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
  if (ret==-1)
    err_quit("connect error");

  //读，输出数据
  int n = 0;
  char recvline[MAXLINE+1];
  while ((n=read(sockfd, recvline, MAXLINE))>0) {
    recvline[n] = 0;
    if (fputs(recvline, stdout)==EOF)
      err_sys("fputs error");
  }

  if (n<0)
    err_sys("read error");

  //关闭套接字
  close(sockfd);
  exit(0);
}