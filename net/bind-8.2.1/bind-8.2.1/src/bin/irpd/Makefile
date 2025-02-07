## Copyright (c) 1996, 1997 by Internet Software Consortium
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

# $Id: Makefile,v 1.3 1999/02/22 02:47:55 vixie Exp $

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
YACC = yacc -d
SYSLIBS = -ll -lutil
DESTBIN = /usr/local/bin
DESTSBIN = /usr/local/sbin
DESTEXEC = /usr/local/libexec
DESTMAN = /usr/share/man
DESTHELP= /usr/share/misc
DESTETC= /etc
DESTRUN= /var/run
AR= ar cru
INSTALL= install
STRIP=-s

PS=ps
LDFLAGS=
CFLAGS=	${CDEBUG} -I${PORTINCL} -I${INCL} -I${TOP}/lib/irs ${DEFS}

VER= LOCAL-`date +%y%m%d.%H%M%S`
HOSTNAMECMD= hostname || uname -n

PROG=	irpd
HDRS=	
SRCS=	irpd.c
OBJS=	irpd.${O}

all: ${PROG}${EXE}

${PROG}${EXE}: irpd.${O} tmp_version.${O} ${LIBBIND}
	${CC} ${CDEBUG} ${LDFLAGS} -o ${PROG}${EXE} ${OBJS} tmp_version.${O} \
		${LIBBIND} ${SYSLIBS}

tmp_version.${O}: tmp_version.c

tmp_version.c: version.c Makefile ../Makefile ${SRCS} ${HDRS}
	(u=$${USER-root} d=`pwd` h=`${HOSTNAMECMD}` t=`date`; \
	sed -e "s|%WHEN%|$${t}|" -e "s|%VERSION%|"${VER}"|" \
	    -e "s|%WHOANDWHERE%|$${u}@$${h}:$${d}|" \
	    < version.c > tmp_version.c)

distclean: clean

clean: FRC
	rm -f ${PROG}${EXE} ${OBJS} core .depend
	rm -f *.BAK *.CKP *~ *.orig
	rm -f tmp_version.c tmp_version.${O}

depend: ${SRCS}
	mkdep ${CPPFLAGS} -I${INCL} -I${PORTINCL} -I${TOP}/lib/irs ${DEFS} ${SRCS}

${DESTDIR}${DESTSBIN}:
	mkdir -p ${DESTDIR}${DESTSBIN}

install: ${DESTDIR}${DESTSBIN} ${PROG}${EXE}
	${INSTALL} ${STRIP} -c -m 755 ${PROG}${EXE} ${DESTDIR}${DESTSBIN}/${PROG}${EXE}

links: FRC
	@ln -s SRC/*.[chy] SRC/test .; rm -f ns_parser.[ch]

tags: FRC
	ctags ${SRCS} *.h

FRC:

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

irpd.o: irpd.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/../../../../i686-UnixWare7.0.1-sysv5/include/assert.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h /usr/include/grp.h \
 ../../include/netdb.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/pwd.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/utmp.h \
 ../../include/isc/ctl.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/memcluster.h ../../include/isc/logging.h \
 ../../include/irs.h ../../include/irp.h \
 ../../include/isc/irpmarshall.h ../../lib/irs/irs_data.h \
 ../../port/unixware7/include/port_after.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
