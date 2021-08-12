//
// Created by mars on 2021/8/11.
//
#include "unp.h"

void my_str_cli(int sockfd) {
  char sendline[MAXLINE], recvline[MAXLINE];

  while (Fgets(sendline, MAXLINE, stdin) != NULL) {
#if 0
    Write(sockfd, sendline, strlen(sendline));
#else
    //在客户端阻塞于输入时杀死服务端子进程
    //客户端第一次写收到RST
    //第二次写会收到SIGPIPE
    Write(sockfd, sendline, 1);
    sleep(1);
    Write(sockfd, sendline+1, strlen(sendline)-1);
#endif

    if (Readline(sockfd, recvline, MAXLINE) == 0)
      err_quit("str_cli: server terminated permaturely");

    Fputs(recvline, stderr);
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

  my_str_cli(sockfd);

  exit(0);
}
