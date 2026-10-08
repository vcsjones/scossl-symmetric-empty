CC ?= gcc
CFLAGS ?= -O0 -g -Wall -Wextra
LDLIBS = -lcrypto
TARGETS = repro-decrypt repro-encrypt

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean
