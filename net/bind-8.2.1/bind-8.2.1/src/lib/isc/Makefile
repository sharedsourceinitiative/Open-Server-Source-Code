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

# $Id: Makefile,v 8.22 1999/02/22 02:47:58 vixie Exp $

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
# -Wimplicit
LD_LIBFLAGS= -x -r
AR= ar cru
RANLIB= ranlib
INSTALL= install
INSTALL_EXEC=
INSTALL_LIB=-o bin -g bin
THREADED= threaded

SRCS=	tree.c base64.c bitncmp.c assertions.c \
	memcluster.c logging.c heap.c \
	ctl_p.c ctl_srvr.c ctl_clnt.c \
	eventlib.c ev_connects.c ev_files.c \
	ev_timers.c ev_streams.c ev_waits.c

OBJS=	tree.${O} base64.${O} bitncmp.${O} assertions.${O} \
	memcluster.${O} logging.${O} heap.${O} \
	ctl_p.${O} ctl_srvr.${O} ctl_clnt.${O} \
	eventlib.${O} ev_connects.${O} ev_files.${O} \
	ev_timers.${O} ev_streams.${O} ev_waits.${O}

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

tree.o: tree.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h ../../include/isc/memcluster.h \
 ../../include/isc/tree.h
base64.o: base64.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../port/unixware7/include/port_after.h
bitncmp.o: bitncmp.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/string.h ../../port/unixware7/include/port_after.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/netinet/in6_f.h \
 /usr/include/netinet/in6.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h ../../include/isc/misc.h
assertions.o: assertions.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../include/isc/assertions.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h
memcluster.o: memcluster.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/uio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/sys/bitypes.h \
 /usr/include/netinet/in6_f.h /usr/include/sys/convsa.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h /usr/include/sys/cdefs.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../include/isc/memcluster.h \
 ../../include/isc/assertions.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/socket.h \
 /usr/include/sys/netconfig.h
logging.o: logging.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/fcntl.h /usr/include/sys/fcntl.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 /usr/include/syslog.h /usr/include/sys/syslog.h /usr/include/errno.h \
 /usr/include/sys/errno.h /usr/include/unistd.h \
 /usr/include/sys/unistd.h ../../include/isc/assertions.h \
 ../../include/isc/logging.h ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h logging_p.h
heap.o: heap.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stddef.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/types.h \
 /usr/include/sys/select.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h ../../include/isc/heap.h
ctl_p.o: ctl_p.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
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
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/assertions.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h /usr/include/sys/time.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/ctl.h ctl_p.h \
 ../../port/unixware7/include/port_after.h
ctl_srvr.o: ctl_srvr.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/file.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/assertions.h ../../include/isc/ctl.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/list.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ctl_p.h \
 ../../port/unixware7/include/port_after.h
ctl_clnt.o: ctl_clnt.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/file.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/assertions.h ../../include/isc/ctl.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/list.h \
 ../../include/isc/memcluster.h ctl_p.h \
 ../../port/unixware7/include/port_after.h
eventlib.o: eventlib.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 ../../include/fd_setsize.h /usr/include/sys/types.h \
 /usr/include/sys/select.h /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/signal.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/signal.h \
 /usr/include/sys/siginfo.h /usr/include/sys/procset.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/isc/assertions.h eventlib_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/fcntl.h /usr/include/sys/fcntl.h /usr/include/string.h \
 ../../include/isc/heap.h ../../include/isc/list.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h
ev_connects.o: ev_connects.c \
 ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 ../../include/fd_setsize.h /usr/include/sys/types.h \
 /usr/include/sys/select.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/isc/assertions.h eventlib_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/errno.h \
 /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../include/isc/heap.h \
 ../../include/isc/list.h ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h
ev_files.o: ev_files.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 ../../include/fd_setsize.h /usr/include/sys/types.h \
 /usr/include/sys/select.h /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/errno.h /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h /usr/include/unistd.h \
 /usr/include/sys/unistd.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 eventlib_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../include/isc/heap.h \
 ../../include/isc/list.h ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h
ev_timers.o: ev_timers.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 ../../include/fd_setsize.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/isc/eventlib.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/uio.h /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 eventlib_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/fcntl.h /usr/include/sys/fcntl.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../include/isc/heap.h \
 ../../include/isc/list.h ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h
ev_streams.o: ev_streams.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 ../../include/fd_setsize.h /usr/include/sys/types.h \
 /usr/include/sys/select.h /usr/include/sys/uio.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/isc/eventlib.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/isc/assertions.h eventlib_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/fcntl.h /usr/include/sys/fcntl.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../include/isc/heap.h \
 ../../include/isc/list.h ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h
ev_waits.o: ev_waits.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 ../../include/fd_setsize.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/isc/eventlib.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/uio.h /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/isc/assertions.h eventlib_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/fcntl.h /usr/include/sys/fcntl.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h ../../include/isc/heap.h \
 ../../include/isc/list.h ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h \
 /usr/include/netinet/in6_f.h /usr/include/netinet/in6.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
