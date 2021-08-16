//
// Created by mars on 2021/8/11.
//
#include "unp.h"

void my_cli_deal(int sockfd) {
  char sendline[MAXLINE], recvline[MAXLINE];

  while (Fgets(sendline, MAXLINE, stdin) != NULL) {
    Write(sockfd, sendline, strlen(sendline));

    if (Readline(sockfd, recvline, MAXLINE) == 0)
      err_quit("str_cli: server terminated permaturely");

    Fputs(recvline, stderr);
  }
}

int main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_un servaddr;

  sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strcpy(servaddr.sun_path, UNIXSTR_PATH);

  Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  my_cli_deal(sockfd);

  exit(0);
}
