/*
 * Copyright (c) 1990, 1993, 1994, 1995, 1996
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @(#) $Header: /proj/cvs/isc/bind/contrib/nslint-1.5.1/lbl/os-ultrix4.h,v 1.1 1996/10/25 17:13:45 vixie Exp $ (LBL)
 */

#ifndef	IPPROTO_ND
/* Trying to compile this not on a Sun system */
#define	IPPROTO_ND 77	/* From <netinet/in.h> on a Sun somewhere */
#endif

#ifndef ETHERTYPE_REVARP
/* some systems don't define this */
#define ETHERTYPE_REVARP	0x8035
#define REVARP_REQUEST		3
#define REVARP_REPLY		4
#endif

/* Map protocol types */
#define ETHERPUP_IPTYPE ETHERTYPE_IP
#define ETHERPUP_REVARPTYPE ETHERTYPE_REVARP
#define ETHERPUP_ARPTYPE ETHERTYPE_ARP

/* newish RIP commands */
#ifndef	RIPCMD_POLL
#define	RIPCMD_POLL 5
#endif
#ifndef	RIPCMD_POLLENTRY
#define	RIPCMD_POLLENTRY 6
#endif

extern	char *optarg;
extern	int optind, opterr;


/* Prototypes missing in Ultrix 4 */
int	bcmp(const char *, const char *, u_int);
void	bcopy(const void *, void *, u_int);
void	bzero(void *, u_int);
void	endservent(void);
int	getopt(int, char * const *, const char *);
#if __STDC_
struct timeval;
struct timezone;
#endif
int	gettimeofday(struct timeval *, struct timezone *);
int	ioctl(int, int, caddr_t);
int	pfopen(char *, int);
int	setlinebuf(FILE *);
int	socket(int, int, int);
int	strcasecmp(const char *, const char *);
