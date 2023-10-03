#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

#ifdef USE_EPOLL
#include "epoll_event_handling.h"
#endif

#ifdef USE_KQUEUE
#include "kqueue_event_handling.h"
#endif

Server *createServer(int port) {
  char port_str[10];
  sprintf(port_str, "%d", port);
  int sock;
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;      // ipv4
  hints.ai_socktype = SOCK_DGRAM; // udp
  hints.ai_flags = AI_PASSIVE;    // fill in my IP for me

  if (getaddrinfo(NULL, port_str, &hints, &res) != 0) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if (sock == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(res);

  Server *server = malloc(sizeof(Server));
  memset(server, 0, sizeof(Server));
  server->port = port;
  server->sockfd = sock;
  return server;
}

void on(Server *server, EventType event, void (*callback)(Request *)) {
  server->cb_array[event] = callback;
}

void startServer(Server *server) {
#ifdef USE_EPOLL
  // Use epoll event handling
  epoll_event_handling(server);
#endif

#ifdef USE_KQUEUE
  // Use kqueue event handling
  kqueue_event_handling(server);
#endif
}
