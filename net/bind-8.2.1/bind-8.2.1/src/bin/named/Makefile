## Copyright (c) 1996-1999 by Internet Software Consortium
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

# $Id: Makefile,v 8.42 1999/02/22 02:47:55 vixie Exp $

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
INSTALL_EXEC=
INSTALL_LIB=-o bin -g bin

PS=ps
LDFLAGS=
CFLAGS=	${CDEBUG} -I${PORTINCL} -I${INCL} ${DEFS}

VER= LOCAL-`date +%y%m%d.%H%M%S`
HOSTNAMECMD= hostname || uname -n

PROG=	named
HDRS=	db_defs.h db_glob.h ns_defs.h ns_glob.h named.h pathnames.h
SRCS=	db_dump.c db_load.c db_lookup.c db_save.c db_update.c \
	db_glue.c db_ixfr.c db_sec.c db_tsig.c \
	ns_parser.c ns_lexer.c ns_parseutil.c ns_ctl.c \
	ns_forw.c ns_init.c ns_main.c ns_maint.c ns_req.c \
	ns_resp.c ns_stats.c ns_ncache.c ns_xfr.c ns_glue.c \
	ns_udp.c ns_config.c ns_update.c ns_ixfr.c ns_signal.c ns_sort.c
OBJS=	db_dump.${O} db_load.${O} db_lookup.${O} db_save.${O} db_update.${O} \
	db_glue.${O} db_ixfr.${O} db_sec.${O} db_tsig.${O} \
	ns_parser.${O} ns_lexer.${O} ns_parseutil.${O} ns_ctl.${O} \
	ns_forw.${O} ns_init.${O} ns_main.${O} ns_maint.${O} ns_req.${O} \
	ns_resp.${O} ns_stats.${O} ns_ncache.${O} ns_xfr.${O} ns_glue.${O} \
	ns_udp.${O} ns_config.${O} ns_update.${O} ns_ixfr.${O} ns_signal.${O} \
	ns_sort.${O}

all: ${PROG}${EXE} pathnames${EXE}

${PROG}${EXE}: pathnames.h ${OBJS} ${LIBBIND} Makefile tmp_version.${O}
	${CC} ${CDEBUG} ${LDFLAGS} ${BOUNDS} -o ${PROG}${EXE} ${OBJS} \
		tmp_version.${O} ${LIBBIND} ${SYSLIBS}

ns_parser.c ns_parser.h: ns_parser.y
	${YACC} ns_parser.y
	mv y.tab.c ns_parser.c
	mv y.tab.h ns_parser.h

tmp_version.${O}: tmp_version.c

tmp_version.c: version.c Makefile ../Makefile ${SRCS} ${HDRS}
	(u=$${USER-root} d=`pwd` h=`${HOSTNAMECMD}` t=`date`; \
	sed -e "s|%WHEN%|$${t}|" -e "s|%VERSION%|"${VER}"|" \
	    -e "s|%WHOANDWHERE%|$${u}@$${h}:$${d}|" \
	    < version.c > tmp_version.c)

pathnames.h: ${TOP}/.settings Makefile pathtemplate.h
	rm -f pathnames.h
	sed -e "s|%DESTSBIN%|${DESTSBIN}|" \
	    -e "s|%DESTEXEC%|${DESTEXEC}|" \
	    -e "s|%DESTETC%|${DESTETC}|" \
	    -e "s|%DESTRUN%|${DESTRUN}|" \
	    < pathtemplate.h > pathnames.h

pathnames${EXE}: pathnames.${O} pathnames.h ${LIBBIND} Makefile
	${CC} ${CDEBUG} ${LDFLAGS} ${BOUNDS} -o $@ pathnames.${O} \
		${LIBBIND} ${SYSLIBS}
ns_signal.${O}: ns_signal.c
	${CC} ${CPPFLAGS} ${CFLAGS} -c $*.c

.c.${O}:
	${CC} ${CPPFLAGS} ${CFLAGS} ${BOUNDS} -c $*.c

distclean: clean
	rm -f ns_parser.c ns_parser.h

clean: FRC
	rm -f ${PROG}${EXE} ${OBJS} core .depend
	rm -f *.BAK *.CKP *~ *.orig
	rm -f tmp_version.c tmp_version.${O}
	rm -f pathnames${EXE} pathnames.${O} tmp_pathnames.h pathnames.h
	rm -f y.tab.h y.tab.c

depend: ${SRCS} pathnames.h
	mkdep ${CPPFLAGS} -I${INCL} -I${PORTINCL} ${DEFS} ${SRCS}

${DESTDIR}${DESTSBIN}:
	mkdir -p ${DESTDIR}${DESTSBIN}

install: ${DESTDIR}${DESTSBIN} ${PROG}${EXE}
	${INSTALL} ${STRIP} -c ${INSTALL_EXEC} -m 755 ${PROG}${EXE} ${DESTDIR}${DESTSBIN}/${PROG}${EXE}

links: FRC
	@ln -s SRC/*.[chy] SRC/test .; rm -f ns_parser.[ch]

tags: FRC
	ctags ${SRCS} *.h

FRC:

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

db_dump.o: db_dump.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/netdb.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/resolv.h /usr/include/string.h /usr/include/syslog.h \
 /usr/include/sys/syslog.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_load.o: db_load.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
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
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/netdb.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_lookup.o: db_lookup.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_save.o: db_save.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_update.o: db_update.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_glue.o: db_glue.c ../../port/unixware7/include/port_before.h \
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
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/netdb.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 ../../include/isc/eventlib.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_ixfr.o: db_ixfr.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
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
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/netdb.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_sec.o: db_sec.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/tree.h \
 ../../include/isc/dst.h ../../port/unixware7/include/port_after.h \
 named.h ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
db_tsig.o: db_tsig.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/tree.h \
 ../../include/isc/dst.h ../../port/unixware7/include/port_after.h \
 named.h ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_parser.o: ns_parser.c \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 ../../port/unixware7/include/port_before.h \
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
 ../../include/arpa/nameser.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/dst.h ../../port/unixware7/include/port_after.h \
 named.h ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h ns_parseutil.h ns_lexer.h
ns_lexer.o: ns_lexer.c ../../port/unixware7/include/port_before.h \
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
 ../../include/arpa/nameser.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 /usr/include/syslog.h /usr/include/sys/syslog.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h ns_parser.h ns_parseutil.h ns_lexer.h
ns_parseutil.o: ns_parseutil.c \
 ../../port/unixware7/include/port_before.h \
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
 ../../include/arpa/nameser.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h ns_parseutil.h
ns_ctl.o: ns_ctl.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
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
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_forw.o: ns_forw.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_init.o: ns_init.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_main.o: ns_main.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/file.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/wait.h /usr/include/sys/siginfo.h \
 /usr/include/sys/procset.h /usr/include/sys/ioctl.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/un.h \
 /usr/include/sys/sockio.h /usr/include/sys/ioccom.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 /usr/include/net/route.h /usr/include/net/radix.h \
 /usr/include/net/if.h /usr/include/sys/ksynch.h \
 /usr/include/net/if_types.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h /usr/include/grp.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/signal.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/signal.h \
 ../../include/netdb.h /usr/include/pwd.h ../../include/resolv.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/list.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/resource.h \
 named.h ../../include/isc/assertions.h ../../include/isc/ctl.h \
 pathnames.h ../../port/unixware7/include/paths.h ns_defs.h db_defs.h \
 ns_glob.h db_glob.h ns_func.h db_func.h
ns_maint.o: ns_maint.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/wait.h /usr/include/sys/siginfo.h \
 /usr/include/sys/procset.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/inet.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/../../../../i686-UnixWare7.0.1-sysv5/include/assert.h \
 /usr/include/errno.h /usr/include/sys/errno.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/signal.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/signal.h \
 ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_req.o: ns_req.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/uio.h \
 /usr/include/sys/file.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/time.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_resp.o: ns_resp.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/file.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_stats.o: ns_stats.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/tree.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/resource.h \
 named.h ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_ncache.o: ns_ncache.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/file.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_xfr.o: ns_xfr.c ../../port/unixware7/include/port_before.h \
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
 /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_glue.o: ns_glue.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/uio.h /usr/include/sys/un.h \
 /usr/include/netinet/in.h /usr/include/netinet/in_f.h \
 /usr/include/netinet/in6.h /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/signal.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/signal.h \
 /usr/include/sys/siginfo.h /usr/include/sys/procset.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/eventlib.h /usr/include/sys/time.h \
 ../../include/isc/logging.h ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_udp.o: ns_udp.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/file.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/netdb.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/nlist.h ../../include/resolv.h /usr/include/syslog.h \
 /usr/include/sys/syslog.h /usr/include/unistd.h \
 /usr/include/sys/unistd.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_config.o: ns_config.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h /usr/include/sys/resource.h \
 named.h ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h ns_parseutil.h
ns_update.o: ns_update.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/uio.h \
 /usr/include/sys/file.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/un.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/syslog.h /usr/include/sys/syslog.h /usr/include/unistd.h \
 /usr/include/sys/unistd.h ../../include/isc/eventlib.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/dst.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_ixfr.o: ns_ixfr.c ../../port/unixware7/include/port_before.h \
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
 ../../include/arpa/inet.h /usr/include/errno.h \
 /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h ../../include/resolv.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 /usr/include/sys/time.h ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_signal.o: ns_signal.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/param_p.h /usr/include/sys/file.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stat.h \
 /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 /usr/include/sys/wait.h /usr/include/sys/siginfo.h \
 /usr/include/sys/procset.h /usr/include/sys/ioctl.h \
 /usr/include/sys/socket.h /usr/include/sys/bitypes.h \
 /usr/include/sys/netconfig.h /usr/include/sys/cdefs.h \
 /usr/include/sys/convsa.h /usr/include/sys/sockio.h \
 /usr/include/sys/ioccom.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 /usr/include/net/route.h /usr/include/net/radix.h \
 /usr/include/net/if.h /usr/include/sys/ksynch.h \
 /usr/include/net/if_types.h ../../include/arpa/nameser.h \
 ../../include/arpa/nameser_compat.h ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/ctype.h \
 /usr/include/errno.h /usr/include/sys/errno.h /usr/include/fcntl.h \
 /usr/include/sys/fcntl.h /usr/include/grp.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdlib.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/signal.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/signal.h \
 ../../include/netdb.h /usr/include/pwd.h ../../include/resolv.h \
 /usr/include/string.h /usr/include/syslog.h /usr/include/sys/syslog.h \
 /usr/include/unistd.h /usr/include/sys/unistd.h \
 ../../include/isc/eventlib.h /usr/include/sys/uio.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../include/isc/memcluster.h ../../include/isc/list.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h
ns_sort.o: ns_sort.c ../../port/unixware7/include/port_before.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/limits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/syslimits.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/byteorder.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/param.h \
 /usr/include/sys/types.h /usr/include/sys/select.h \
 /usr/include/sys/param_p.h /usr/include/sys/socket.h \
 /usr/include/sys/bitypes.h /usr/include/sys/netconfig.h \
 /usr/include/sys/cdefs.h /usr/include/sys/convsa.h \
 /usr/include/sys/file.h /usr/include/netinet/in.h \
 /usr/include/netinet/in_f.h /usr/include/netinet/in6.h \
 /usr/include/netinet/in6_f.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/sys/stream.h \
 /usr/include/sys/cred.h /usr/include/sys/strmdep.h \
 ../../include/arpa/nameser.h ../../include/arpa/nameser_compat.h \
 ../../include/arpa/inet.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdio.h \
 /usr/include/syslog.h /usr/include/sys/syslog.h \
 ../../include/resolv.h ../../include/isc/eventlib.h \
 /usr/include/sys/uio.h /usr/include/sys/time.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/time.h \
 ../../include/isc/logging.h \
 /usr/local/lib/gcc-lib/i686-UnixWare7.0.1-sysv5/2.95.1/include/stdarg.h \
 ../../port/unixware7/include/port_after.h named.h \
 ../../include/isc/assertions.h ../../include/isc/list.h \
 ../../include/isc/ctl.h pathnames.h \
 ../../port/unixware7/include/paths.h ns_defs.h db_defs.h ns_glob.h \
 db_glob.h ns_func.h db_func.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
