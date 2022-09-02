#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int nonblocking_connect(const char *addr, unsigned short port, int wait_seconds) {
  struct sockaddr_in sockaddr;
  bzero(&sockaddr, sizeof(sockaddr));

  sockaddr.sin_family = AF_INET;
  inet_pton(AF_INET, addr, &sockaddr.sin_addr);
  sockaddr.sin_port = htons(port);

  int sock_fd = socket(PF_INET, SOCK_STREAM, 0);

  // Set nonblocking
  int old_opts = fcntl(sock_fd, F_GETFL);
  int new_opts = old_opts | O_NONBLOCK;
  fcntl(sock_fd, F_SETFL, new_opts);

  int rc = connect(sock_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));

  if (rc == 0) {
    // Connect success
    fcntl(sock_fd, F_SETFL, old_opts);
    return sock_fd;
  } else if (errno != EINPROGRESS) {
    return -1;
  }

  fd_set readfds;
  fd_set writefds;
  struct timeval timeout;

  FD_ZERO(&readfds);
  FD_SET(sock_fd, &writefds);

  timeout.tv_sec = wait_seconds;
  timeout.tv_usec = 0;

  rc = select(sock_fd + 1, NULL, &writefds, NULL, &timeout);
  if (rc <= 0) {
    // Timeout or select failed
    close(sock_fd);
    return -1;
  }

  if (!FD_ISSET(sock_fd, &writefds)) {
    close(sock_fd);
    return -1;
  }

  int err = 0;
  socklen_t len = sizeof(err);
  if (getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
    // Get socket option failed
    close(sock_fd);
    return -1;
  }

  if (err != 0) {
    // Connect failed
    close(sock_fd);
    return -1;
  }

  fcntl(sock_fd, F_SETFL, old_opts);
  return sock_fd;
}

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    printf("usage: %s <addr> <port>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *addr = argv[1];
  unsigned short port = atoi(argv[2]);

  int sock_fd = nonblocking_connect(addr, port, 5);
  if (sock_fd < 0) {
    printf("connect failed\n");
  } else {
    printf("connect success: %d\n", sock_fd);
  }
  close(sock_fd);

  return EXIT_SUCCESS;
}
