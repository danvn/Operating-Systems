# File: Makefile
# By: Andy Sayler <www.andysayler.com>
# Adopted from work by: Chris Wailes <chris.wailes@gmail.com>
# Project: CSCI 3753 Programming Assignment 5
# Creation Date: 2010/04/06
# Modififed Date: 2012/04/12
# Description:
#	This is the Makefile for PA5.


CC           = gcc

CFLAGSFUSE   = `pkg-config fuse --cflags`
LLIBSFUSE    = `pkg-config fuse --libs`
LLIBSOPENSSL = -lcrypto

CFLAGS = -c -g -Wall -Wextra
LFLAGS = -g -Wall -Wextra

FUSE_EXAMPLES = fusehello fusexmp 
XATTR_EXAMPLES = xattr-util
OPENSSL_EXAMPLES = aes-crypt-util 

.PHONY: all fuse-examples xattr-examples openssl-examples clean

all: fuse-examples xattr-examples openssl-examples pa5-encfs

fuse-examples: $(FUSE_EXAMPLES)
xattr-examples: $(XATTR_EXAMPLES)
openssl-examples: $(OPENSSL_EXAMPLES)

fusehello: fusehello.o
	$(CC) $(LFLAGS) $^ -o $@ $(LLIBSFUSE)

fusexmp: fusexmp.o
	$(CC) $(LFLAGS) $^ -o $@ $(LLIBSFUSE)

pa5-encfs: pa5-encfs.o aes-crypt.o
	$(CC) $(LFLAGS) $^ -o $@ $(LLIBSFUSE) $(LLIBSOPENSSL)

xattr-util: xattr-util.o
	$(CC) $(LFLAGS) $^ -o $@

aes-crypt-util: aes-crypt-util.o aes-crypt.o
	$(CC) $(LFLAGS) $^ -o $@ $(LLIBSOPENSSL)

fusehello.o: fusehello.c
	$(CC) $(CFLAGS) $(CFLAGSFUSE) $<

fusexmp.o: fusexmp.c
	$(CC) $(CFLAGS) $(CFLAGSFUSE) $<

pa5-encfs.o: pa5-encfs.c
	$(CC) $(CFLAGS) $(CFLAGSFUSE) $<

xattr-util.o: xattr-util.c
	$(CC) $(CFLAGS) $<

aes-crypt-util.o: aes-crypt-util.c aes-crypt.h
	$(CC) $(CFLAGS) $<

aes-crypt.o: aes-crypt.c aes-crypt.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(FUSE_EXAMPLES)
	rm -f $(XATTR_EXAMPLES)
	rm -f $(OPENSSL_EXAMPLES)
	rm -f *.o
	rm -f pa5-encfs
	rm -f *~
	rm -f handout/*~
	rm -f handout/*.log
	rm -f handout/*.aux
	rm -f handout/*.out



