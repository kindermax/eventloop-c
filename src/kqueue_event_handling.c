#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>

#include "server.h"

void kqueue_event_handling(Server *server) {
  int kq;
  struct kevent event, events[MAX_EVENTS];
  struct sockaddr client_addr;
  socklen_t addr_size;
  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE);

  kq = kqueue();
  if (kq == -1) {
    perror("kqueue");
    exit(EXIT_FAILURE);
  }

  EV_SET(&event, server->sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

  printf("Listening on port %d\n", server->port);

  // The event loop
  while (1) {
    int nfds = kevent(kq, &event, 1, events, MAX_EVENTS, NULL);
    if (nfds == -1) {
      perror("kevent");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nfds; i++) {
      if (events[i].filter == EVFILT_READ &&
          events[i].ident == server->sockfd) {
        addr_size = sizeof(client_addr);
        int bytes_read = recvfrom(server->sockfd, buf, BUF_SIZE, 0,
                                  &client_addr, &addr_size);
        if (bytes_read == -1) {
          perror("recv");
          exit(EXIT_FAILURE);
        }
        // Run the callback
        Request req = {client_addr, addr_size, server->sockfd, buf};
        server->cb_array[EventTypeMessage](&req);
      }
    }
  }
}
