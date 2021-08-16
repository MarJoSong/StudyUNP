//
// Created by mars on 2021/8/11.
//
#include "unp.h"

/*
 * 总结一下几个点：
 * 需要一个pollfd结构数组，因为不是select这种值-结果参数所以不需要rset和all_set
 *
 * 每轮select后，记录返回的数量nready，
 *
 * 先检查是否有新的监听到来FD_ISSET(listenfd, &rset)
 * 有新监听则建立新套接字，入client(溢出检查)，入all_set，
 * 修改maxfd和客户端套接字数组里最大索引maxi的值，最后nready减1，为0时进行下一次轮询
 *
 * 在遍历检查客户端套接字是否有数据（注意跳过已关闭的客户端）
 * 如果数据量为零，即FIN则关套接字，移出all_set，对应client数组置-1
 * 否则回写个客户端
 * 最后nready减1，为0跳出客户端套接字遍历
 *
 *
 * TIPS: 了解拒绝服务型攻击
 * 针对服务器作某些动作，导致服务器不能为其他合法客户提供服务
 */

#define OPEN_MAX_LIMIT  1024

int main(int argc, char **argv) {
  int i, n;
  int listenfd, connfd, sockfd;
  int maxfd;
  int maxi; //指向客户端套接字数组达到的最大索引值
  int nready; //就绪套接字的数量
  char buf[MAXLINE];
  socklen_t clilen;

  struct pollfd client[OPEN_MAX_LIMIT];
  struct sockaddr_in servaddr, cliaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  client[0].fd = listenfd;
  client[0].events = POLLRDNORM;

  for (i = 1; i < OPEN_MAX_LIMIT; i++)
    client[i].fd = -1;

  maxi = 0;

  for (;;) {

    nready = Poll(client, maxi + 1, INFTIM);

    if (client[0].revents * POLLRDNORM) {
      clilen = sizeof(cliaddr);
      connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

      for (i = 1; i < OPEN_MAX_LIMIT; i++) {
        if (client[i].fd < 0) {
          client[i].fd = connfd;
          break;
        }
      }

      if (i == OPEN_MAX_LIMIT)
        err_quit("too many clients");

      client[i].events = POLLRDNORM;

      if (i > maxi)
        maxi = i;

      if (--nready <= 0)
        continue;
    }

    for (i = 1; i <= maxi; i++) {
      if ((sockfd = client[i].fd) < 0)
        continue;

      if (client[i].revents & (POLLRDNORM | POLLERR)) {
        if ((n = Read(sockfd, buf, MAXLINE)) < 0) {
          if (errno == ECONNRESET) {
            Close(sockfd);
            client[i].fd = -1;
          }
        } else if (n == 0) {
          Close(sockfd);
          client[i].fd = -1;
        } else {
          Write(sockfd, buf, n);
        }

        if (--nready <= 0)
          break;
      }
    }
  }
}
