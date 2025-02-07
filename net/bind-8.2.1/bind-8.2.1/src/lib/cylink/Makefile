# Copyright (c) 1996,1999 by Internet Software Consortium
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
# ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
# CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
# DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
# PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
# ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
# SOFTWARE.

# $Id: Makefile,v 1.7 1999/02/22 02:47:57 vixie Exp $

# these are only appropriate for BSD 4.4 or derivatives, and are used in
# development.  normal builds will be done in the top level directory and
# this Makefile will be invoked with a lot of overrides for the following:
SYSTYPE= bsdos
DESTDIR =
DESTLIB = /usr/local/lib
O=o
A=a
CC= cc
LD= ld
SHELL= /bin/sh
CDEBUG= -g
TOP= ../..
INCL = ${TOP}/include
PORTINCL = ${TOP}/port/${SYSTYPE}/include
LIBBIND = ${TOP}/lib/libbind.${A}
LIBBINDR = ../${TOP}/lib/libbind_r.${A}
CFLAGS=	${CDEBUG} -I${PORTINCL} -I${INCL}
LD_LIBFLAGS= -x -r
AR= ar cru
RANLIB= ranlib
INSTALL= install
INSTALL_EXEC=
INSTALL_LIB=-o bin -g bin
THREADED= threaded

SRCS=	bn.c bn00.c lbn00.c lbnmem.c legal.c \
	bits.c dss.c math.c ctk_prime.c rand.c sha.c swap.c 

OBJS=	bn.${O} bn00.${O} lbn00.${O} lbnmem.${O} legal.${O} \
	bits.${O} dss.${O} math.${O} ctk_prime.${O} rand.${O} sha.${O} \
	swap.${O} 

TEST= main.${O} test.${O}

all: ${LIBBIND}

${LIBBIND}: ${OBJS}
	( cd ${THREADED} ; \
	${AR} ${LIBBINDR} ${ARPREF} ${OBJS} ${ARSUFF} ; \
	${RANLIB} ${LIBBINDR} )
	${AR} ${LIBBIND} ${ARPREF} ${OBJS} ${ARSUFF}
	${RANLIB} ${LIBBIND}

.c.${O}:
	if test ! -d ${THREADED} ; then mkdir ${THREADED} ; fi
	${CC} ${CPPFLAGS} ${CFLAGS} ${BOUNDS} ${REENTRANT} -c $*.c \
	 -o ${THREADED}/$*.${O}
	-${LDS} ${LD} ${LD_LIBFLAGS} ${THREADED}/$*.${O} -o a.out && \
	 ${LDS} mv a.out ${THREADED}/$*.${O}
	${CC} ${CPPFLAGS} ${CFLAGS} ${BOUNDS} -c $*.c
	-${LDS} ${LD} ${LD_LIBFLAGS} $*.${O} -o a.out && \
	 ${LDS} mv a.out $*.${O}

distclean: clean

crypto: ${OBJS} ${TEST}
	${CC} -o crypto ${TEST} ${OBJS} 

clean: FRC
	rm -f .depend a.out core ${LIB} tags
	rm -f *.${O} *.BAK *.CKP *~
	rm -f ${THREADED}/*.${O}
	-rmdir ${THREADED}

depend: FRC
	mkdep -I${INCL} -I${PORTINCL} ${CPPFLAGS} ${SRCS}

links: FRC
	@set -e; ln -s SRC/*.[ch] .

install:

FRC:

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

bn.o: bn.c bn.h
bn00.o: bn00.c bnsize00.h lbn.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 bn16.c \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/../../../../i686-UnixWare7.0.1-sysv5/include/assert.h \
 /usr/include/string.h lbn16.h lbnmem.h bn16.h bn.h kludge.h \
 bninit16.c
lbn00.o: lbn00.c bnsize00.h lbn.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 lbn16.c \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/../../../../i686-UnixWare7.0.1-sysv5/include/assert.h \
 /usr/include/string.h lbn16.h lbnmem.h legal.h kludge.h
lbnmem.o: lbnmem.c \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h lbn.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 lbnmem.h kludge.h
legal.o: legal.c legal.h
bits.o: bits.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h bn.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h cylink.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/bitypes.h ctk_endian.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 lbn.h ../../port/unixware7/include/port_after.h \
 /usr/include/sys/socket.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h
dss.o: dss.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h cylink.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/bitypes.h ctk_endian.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 lbn.h ../../port/unixware7/include/port_after.h \
 /usr/include/sys/socket.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h toolkit.h cencrint.h dssnum.h sha.h
math.o: math.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 bn.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h cylink.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/bitypes.h ctk_endian.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 lbn.h toolkit.h
ctk_prime.o: ctk_prime.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h cylink.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/bitypes.h ctk_endian.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 lbn.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 toolkit.h cencrint.h ../../port/unixware7/include/port_after.h \
 /usr/include/sys/socket.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h
rand.o: rand.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h cylink.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/bitypes.h ctk_endian.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 lbn.h toolkit.h cencrint.h ../../port/unixware7/include/port_after.h \
 /usr/include/sys/socket.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h sha.h
sha.o: sha.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 cylink.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/bitypes.h ctk_endian.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 lbn.h toolkit.h cencrint.h ../../port/unixware7/include/port_after.h \
 /usr/include/sys/socket.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h sha.h
swap.o: swap.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h cylink.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/bitypes.h ctk_endian.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 lbn.h ../../port/unixware7/include/port_after.h \
 /usr/include/sys/socket.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
