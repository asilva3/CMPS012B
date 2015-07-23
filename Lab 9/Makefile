# $Id: Makefile,v 1.11 2013-10-16 13:15:08-07 - - $

MKFILE 	= Makefile

GCC      = gcc -g -O0 -Wall -Wextra -std=gnu99

CSOURCE = numsort.c inssort.c linesort.c
SOURCES = ${CSOURCES} Makefile PARTNER README inssort.h
EXECBIN = numsort linesort
SUBMIT   = submit cmps012b-wm.f13 lab9

all : numsort linesort

numsort: numsort.o inssort.o
	${GCC} numsort.o inssort.o -o numsort
linesort: linesort.o inssort.o
	${GCC} linesort.o inssort.o -o linesort

%.o : %.c
	cid + $<
	${GCC} $< -c $<

ci :${SOURCES}
	cid + ${SOURCES}

clean :
	- rm core

spotless : clean
	- rm ${EXEBINS}

lis : ${CSOURCES} Makefile
	mkpspdf ${LISTING} ${SOURCES}

submit : ${SOURCES}
	-checksource ${SOURCES}
	${SUBMIT} ${SOURCES}

again : ${SOURCES}
	gmake --no-print-directory spotless ci all lis

