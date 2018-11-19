# Apache#34464

CC=clang++
CFLAGS=-g0 -std=c++11
B=BUGGY
P1=PATCH1
P2=PATCH2
BUGID=apache34464
BUILD=../

.PHONY: all
all: ${BUGID}.buggy ${BUGID}.patch1 ${BUGID}.patch2

${BUGID}.buggy: ${BUILD}${BUGID}.cpp
	${CC} -D${B} ${CFLAGS} $^ -o $@

${BUGID}.patch1: ${BUILD}${BUGID}.cpp
	${CC} -D${P1} ${CFLAGS} $^ -o $@

${BUGID}.patch2: ${BUILD}${BUGID}.cpp
	${CC} -D${P2} ${CFLAGS} $^ -o $@ 

.PHONY: clean
clean:
	rm -rf *.buggy *.patch1 *.patch2
