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

# $Id: Makefile,v 1.14 1999/02/22 02:47:56 vixie Exp $

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
LEX = lex -I
SYSLIBS = -ll -lutil
DESTBIN = /usr/local/bin
DESTSBIN = /usr/local/sbin
DESTEXEC = /usr/local/libexec
DESTMAN = /usr/share/man
DESTHELP= /usr/share/misc
AR= ar cru
INSTALL= install
STRIP=-s
INSTALL_EXEC=
INSTALL_LIB=-o bin -g bin
PS=ps

LDFLAGS=
CFLAGS=	${CDEBUG} -I${PORTINCL} -I${INCL} ${DEFS}

PROG= ndc
SRCS= ${PROG}.c
OBJS= ${PROG}.${O}
HDRS= pathnames.h

all: ${PROG}${EXE}

${PROG}${EXE}: ${HDRS} ${OBJS} ${LIBBIND} Makefile
	${CC} ${CDEBUG} ${LDFLAGS} ${BOUNDS} -o ${PROG}${EXE} ${OBJS} ${LIBBIND} ${SYSLIBS}

.c.${O}:
	${CC} ${CPPFLAGS} ${CFLAGS} ${BOUNDS} -c $*.c

pathnames.h: Makefile ../named/pathtemplate.h
	rm -f pathnames.h
	sed -e "s|%DESTSBIN%|${DESTSBIN}|" \
	    -e "s|%DESTEXEC%|${DESTEXEC}|" \
	    -e "s|%DESTETC%|${DESTETC}|" \
	    -e "s|%DESTRUN%|${DESTRUN}|" \
	    < ../named/pathtemplate.h > pathnames.h

distclean: clean

clean: FRC
	rm -f ${PROG}${EXE} ${OBJS} core .depend
	rm -f *.BAK *.CKP *~ *.orig
	rm -f pathnames.h

depend: ${SRCS} pathnames.h
	mkdep -p ${CPPFLAGS} -I${INCL} -I${PORTINCL} ${SRCS}

${DESTDIR}${DESTSBIN}:
	mkdir -p ${DESTDIR}${DESTSBIN}

install: ${DESTDIR}${DESTSBIN} ${PROG}${EXE}
	${INSTALL}  ${INSTALL_EXEC} ${STRIP} -c -m 755 ${PROG}${EXE} ${DESTDIR}${DESTSBIN}/${PROG}${EXE}

links: FRC
	@set -e; ln -s SRC/*.[ch] .

tags: FRC
	ctags ${SRCS} *.h

FRC:

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

ndc: ndc.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/file.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/unistd.h /usr/include/sys/unistd.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/signal.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/signal.h \
 /usr/include/sys/siginfo.h /usr/include/sys/procset.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/ctl.h ../../port/unixware7/include/port_after.h \
 pathnames.h ../../port/unixware7/include/paths.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
