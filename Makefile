ifeq ($(shell uname),Linux)
  CFLAGS += -DUSE_EPOLL
  EXCLUDE_EVENT_IMPL_SRC = $(SRCDIR)/kqueue_event_handling.c
endif

# Define USE_KQUEUE for macOS
ifeq ($(shell uname),Darwin)
  CFLAGS += -DUSE_KQUEUE
  EXCLUDE_EVENT_IMPL_SRC = $(SRCDIR)/epoll_event_handling.c
endif

CC=gcc
CFLAGS=-O0 -Wall -g #-Werror

SRCDIR=src
INCDIR=include
OBJDIR=obj
BINDIR=bin

SRCFILES=$(wildcard $(SRCDIR)/*.c)
TARGET=$(BINDIR)/demo

OBJFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(filter-out $(EXCLUDE_EVENT_IMPL_SRC), $(SRCFILES)))

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean
