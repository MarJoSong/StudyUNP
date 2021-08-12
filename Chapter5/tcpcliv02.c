//
// Created by mars on 2021/8/11.
//
#include "unp.h"

void my_str_cli(int sockfd) {
  char sendline[MAXLINE], recvline[MAXLINE];

  while (Fgets(sendline, MAXLINE, stdin) != NULL) {
    Write(sockfd, sendline, strlen(sendline));

    if (Readline(sockfd, recvline, MAXLINE) == 0)
      err_quit("str_cli: server terminated permaturely");

    Fputs(recvline, stderr);
  }
}

int main(int argc, char **argv) {
  if (argc != 2)
    err_quit("usage: %s <IPAddress>", argv[0]);

  int i, sockfd[5];
  for (i = 0; i < 5; ++i) {
    sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd[i], (struct sockaddr *) &servaddr, sizeof(servaddr));
  }

  my_str_cli(sockfd[0]);

  exit(0);
}
