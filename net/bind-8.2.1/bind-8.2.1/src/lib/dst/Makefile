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

# $Id: Makefile,v 1.13 1999/03/07 09:33:47 vixie Exp $

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

HDRS=   md5.h md5_locl.h 

SRCS=	dst_api.c prandom.c rsaref_link.c support.c bsafe_link.c \
	cylink_link.c hmac_link.c md5_dgst.c eay_dss_link.c

OBJS=	dst_api.${O} prandom.${O} rsaref_link.${O} support.${O} \
	bsafe_link.${O} cylink_link.${O} hmac_link.${O} md5_dgst.${O} \
	eay_dss_link.${O}

CRYPTINCL=	-I../cylink -I../dnssafe
CRYPTFLAGS=	-DCYLINK_DSS -DHMAC_MD5 -DUSE_MD5 -DDNSSAFE

all: ${LIBBIND}

${LIBBIND}: ${OBJS}
	( cd ${THREADED} ; \
	${AR} ${LIBBINDR} ${ARPREF} ${OBJS} ${ARSUFF} ; \
	${RANLIB} ${LIBBINDR} )
	${AR} ${LIBBIND} ${ARPREF} ${OBJS} ${ARSUFF}
	${RANLIB} ${LIBBIND}

.c.${O}:
	if test ! -d ${THREADED} ; then mkdir ${THREADED} ; fi
	${CC} ${CPPFLAGS} ${CFLAGS} ${BOUNDS} ${REENTRANT} ${CRYPTINCL} ${CRYPTFLAGS} -c $*.c -o ${THREADED}/$*.${O}
	-${LDS} ${LD} ${LD_LIBFLAGS} ${THREADED}/$*.${O} -o a.out && \
	 ${LDS} mv a.out ${THREADED}/$*.${O}
	${CC} ${CPPFLAGS} ${CFLAGS} ${CRYPTINCL} ${CRYPTFLAGS} -c $*.c
	-${LDS} ${LD} ${LD_LIBFLAGS} $*.${O} -o a.out && \
	 ${LDS} mv a.out $*.${O}

$(SRCS):: $(HDRS)

distclean: clean

clean: FRC
	rm -f .depend a.out core ${LIB} tags
	rm -f *.${O} *.BAK *.CKP *~
	rm -f prand_conf.h
	rm -f ${THREADED}/*.${O}
	-rmdir ${THREADED}

depend: FRC
	mkdep -I${INCL} -I${PORTINCL} ${CPPFLAGS} ${SRCS}

links: FRC
	@set -e; ln -s SRC/*.[ch] SRC/*.pl .

install:

FRC:

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

dst_api.o: dst_api.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/errno.h /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h /usr/include/string.h \
 /usr/include/memory.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/resolv.h dst_internal.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h
prandom.o: prandom.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/unistd.h /usr/include/sys/unistd.h \
 /usr/include/fcntl.h /usr/include/sys/fcntl.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/dirent.h /usr/include/sys/dirent.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h dst_internal.h /usr/include/sys/bitypes.h \
 ../../include/isc/dst.h ../../port/unixware7/include/prand_conf.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/socket.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/netinet/in6_f.h \
 /usr/include/netinet/in6.h
rsaref_link.o: rsaref_link.c
support.o: support.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h /usr/include/memory.h \
 /usr/include/string.h /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/sys/bitypes.h \
 /usr/include/netinet/in6_f.h /usr/include/sys/convsa.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/cdefs.h \
 ../../include/arpa/nameser_compat.h ../../include/resolv.h \
 dst_internal.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/socket.h \
 /usr/include/sys/netconfig.h
bsafe_link.o: bsafe_link.c
cylink_link.o: cylink_link.c
hmac_link.o: hmac_link.c
md5_dgst.o: md5_dgst.c \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/sys/types.h /usr/include/sys/select.h
eay_dss_link.o: eay_dss_link.c

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
