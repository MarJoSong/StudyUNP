//
// Created by mars on 2021/8/11.
//
#include "unp.h"

typedef void MySigfunc(int);

MySigfunc *MySignal(int signo, MySigfunc *func) {
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

    if (sigaction(signo, &act, &oact) < 0)
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
  ssize_t n;
  char buf[MAXLINE];
  bzero(buf, MAXLINE);

  again:
  while ((n = read(sockfd, buf, MAXLINE)) > 0) {
    Write(sockfd, buf, strlen(buf));
    //读入数据之前需要清空缓存区
    bzero(buf, MAXLINE);
  }

  if (n < 0 && errno == EINTR)
    goto again;
  else if (n < 0)
    err_sys("str_echo: read error");
}

int main(int argc, char **argv) {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_un servaddr, cliaddr;

  listenfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strcpy(servaddr.sun_path, UNIXSTR_PATH);

  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  MySignal(SIGCHLD, sig_child);

  for (;;) {

    clilen = sizeof(cliaddr);

    connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
    if (connfd == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        err_sys("accept error");
      }
    }

    fprintf(stderr, "bound name:[%s]\n", cliaddr.sun_path);

    childpid = Fork();
    if (childpid == 0) {
      Close(listenfd);
      my_ser_deal(connfd);
      exit(0);
    }
    Close(connfd);
  }
}
