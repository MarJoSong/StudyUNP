//
// Created by mars on 2021/8/11.
//
#include "unp.h"
#include "sum.h"

//ypedef	void	Sigfunc(int);
//typedef void(*MySigfunc)(int);
typedef void MySigfunc(int);

MySigfunc * MySignal(int signo, MySigfunc* func) {
  struct sigaction act, oact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
    act.sa_flags != SA_INTERRUPT;
#endif
  } else {
#ifdef SA_RESTART
    act.sa_flags != SA_RESTART;
#endif

    if (sigaction(signo, &act , &oact) <0)
      return (SIG_ERR);
    return (oact.sa_handler);
  }

}

void sig_child(int signo) {
  pid_t pid;
  int stat;

  pid = wait(&stat);
  fprintf(stderr, "child %d terminated\n", pid);
}

void my_ser_deal(int sockfd) {
#if defined(ECHO_TEST)
  ssize_t n;
  char buf[MAXLINE];
  bzero(buf, MAXLINE);

  again:
  while ((n = read(sockfd, buf, MAXLINE)) > 0) {
    Write(sockfd, buf, strlen(buf));
    //读入数据之前需要清空缓存区
    bzero(buf, MAXLINE);
  }
#if defined(FINL_4_SEG)
  //服务器端若在发送对客户端FIN的ACK响应同时立即关闭，TCP会将ACK和服务器的FIN分节合并，导致四次挥手只发送三个分节
  sleep(1);
#endif

  if (n<0&& errno==EINTR)
    goto again;
  else if (n<0)
    err_sys("str_echo: read error");
#elif defined(ENDIAN_TEST)
  ssize_t n;
  struct args args;
  struct result result;

  again:
  for (;;) {
    if ((n=read(sockfd,&args, sizeof(args)))==0)
      return;
    result.sum = args.arg1 + args.arg2;
    Write(sockfd, &result, sizeof(result));
  }
#endif
}

int main(int argc, char **argv) {
  int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  MySignal(SIGCHLD, sig_child);

  //sleep(10);
  for (;;) {
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    //int connfd = Accept(listenfd, (SA * ) & cliaddr, &clilen);
    int connfd = accept(listenfd, (SA * ) & cliaddr, &clilen);
    if (connfd==-1) {
      if (errno == EINTR) {
        continue;
      } else {
        err_sys("accept error");
      }
    }

    char cliaddr_ip[MAXLINE];
    fprintf(stderr, "Connection from %s arrived at %d\n",
            inet_ntop(AF_INET, &cliaddr.sin_addr, cliaddr_ip, MAXLINE),
            ntohs(cliaddr.sin_port));

    pid_t childpid = Fork();
    if (childpid == 0) {
      Close(listenfd);
      my_ser_deal(connfd);
      exit(0);
    }
    Close(connfd);
  }
}
