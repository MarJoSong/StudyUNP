//
// Created by mars on 2021/8/11.
//
#include "unp.h"

void my_str_cli(FILE *fp, int sockfd) {
  int maxfd;
  fd_set rset;
  char sendline[MAXLINE], recvline[MAXLINE];

  FD_ZERO(&rset);
  for (;;) {
    FD_SET(fileno(fp), &rset);
    FD_SET(sockfd, &rset);
    maxfd = max(fileno(fp), sockfd) + 1;

    if (select(maxfd, &rset, NULL, NULL, NULL) == -1) {
      err_sys("select error");
    }

    if (FD_ISSET(sockfd, &rset)) {
      bzero(sendline, MAXLINE);
      if (Readline(sockfd, recvline, MAXLINE) == 0)
        err_quit("str_cli: server terminated permaturely");
      Fputs(recvline, stdout);
    }

    if (FD_ISSET(fileno(fp), &rset)) {
      bzero(sendline, MAXLINE);
      if (Fgets(sendline, MAXLINE, fp) == NULL)
        return;
      Write(sockfd, sendline, strlen(sendline));
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
