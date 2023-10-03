# Basic event loop example using epoll or kqueue

## Build and run

Build and run server

```bash
$ make
$ ./bin/demo
```

Connect using `nc`:

```bash
nc -u 127.0.0.1 41234
```

## Cleanup

```bash
$ make clean
```

Makefile automatically chooses right implementation for your OS.

