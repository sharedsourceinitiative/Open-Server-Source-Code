## Copyright (c) 1996,1999 by Internet Software Consortium
##
## Permission to use, copy, modify, and distribute this software for any
## purpose with or without fee is hereby granted, provided that the above
## copyright notice and this permission notice appear in all copies.
##
## THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
## ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
## OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
## CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
## DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
## PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
## ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
## SOFTWARE.

# $Id: Makefile,v 8.24 1999/01/08 23:28:15 vixie Exp $

DESTDIR=
CC= cc
SHELL= /bin/sh

CDEBUG= -g

#(net2 and its descendents)
SYSTYPE = bsdos
TOP = ../..
INCL =  ${TOP}/include
PORTINCL = ${TOP}/port/${SYSTYPE}/include
LIBBIND = ${TOP}/lib/libbind.a
A=a
O=o
EXE=
LEX = lex -I
SYSLIBS = -ll -lutil
DESTBIN = /usr/local/bin
DESTSBIN = /usr/local/sbin
DESTEXEC = /usr/local/libexec
DESTMAN = /usr/share/man
DESTHELP= /usr/share/misc
STRIP=-s
INSTALL_EXEC=
INSTALL_LIB=-o bin -g bin

LDFLAGS=
CFLAGS=	${CDEBUG} -I${PORTINCL} -I${INCL}

NSLOOKUP_OBJS= \
	../nslookup/subr.${O} ../nslookup/send.${O} \
	../nslookup/list.${O} ../nslookup/debug.${O}

PROG= dig
SRCS= ${PROG}.c
OBJS= ${PROG}.${O} 

all: ${PROG}${EXE}

${PROG}${EXE}: ${OBJS} ${NSLOOKUP_OBJS} ${LIBBIND} Makefile
	${CC} ${CDEBUG} ${LDFLAGS} ${BOUNDS} -o ${PROG}${EXE} ${OBJS} \
		${NSLOOKUP_OBJS} ${LIBBIND} ${SYSLIBS}

.c.${O}:
	${CC} ${CPPFLAGS} ${CFLAGS} ${BOUNDS} -c $*.c

distclean: clean

clean: FRC
	rm -f ${PROG}${EXE} ${OBJS} core .depend
	rm -f *.BAK *.CKP *~ *.orig

depend: ${SRCS}
	mkdep -p ${CPPFLAGS} -I${INCL} -I${PORTINCL} ${SRCS}

${DESTDIR}${DESTBIN}:
	mkdir -p ${DESTDIR}${DESTBIN}

install: ${DESTDIR}${DESTBIN} ${PROG}${EXE}
	${INSTALL} ${STRIP} -c ${INSTALL_EXEC} -m 755 ${PROG}${EXE} ${DESTDIR}${DESTBIN}/${PROG}${EXE}

links: FRC
	@set -e; ln -s SRC/*.[ch] .

tags: FRC
	ctags *.[ch]

FRC:

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

dig: dig.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/file.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/wait.h \
 /usr/include/sys/siginfo.h /usr/include/sys/procset.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h ../../include/netdb.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/setjmp.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../port/unixware7/include/port_after.h ../nslookup/res.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
