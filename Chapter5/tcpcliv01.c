//
// Created by mars on 2021/8/11.
//
#include "unp.h"
#include "sum.h"

void my_cli_deal(int sockfd) {
#if defined(ECHO_TEST)
  char sendline[MAXLINE], recvline[MAXLINE];
  bzero(sendline, MAXLINE);

  while (Fgets(sendline, MAXLINE, stdin) != NULL) {
#if 1
    Write(sockfd, sendline, strlen(sendline));
    //读入数据之前需要清空缓存区
    bzero(sendline, MAXLINE);
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
#elif defined(ENDIAN_TEST)
  char sendline[MAXLINE], recvline[MAXLINE];
  struct args args;
  struct result result;
  while (Fgets(sendline, MAXLINE, stdin) != NULL) {
    if (sscanf(sendline, "%ld %ld", &args.arg1, &args.arg2) != 2) {
      fprintf(stderr, "invalid input:%s\n", sendline);
      continue;
    }
    Write(sockfd, &args, sizeof(args));
    if (Read(sockfd, &result, sizeof(result)) ==0) {
      err_quit("str_cli: server terminated permaturely");
    }
    fprintf(stderr, "sum:%ld\n", result.sum);
  }
#endif
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

  my_cli_deal(sockfd);

  exit(0);
}
