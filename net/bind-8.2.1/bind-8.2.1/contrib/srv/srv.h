/****************************************************************************
** $Id: srv.h,v 1.1 1998/07/30 21:56:39 vixie Exp $
**
** Definition of something or other
**
** Created : 979899
**
** Copyright (C) 1997 by Troll Tech AS.  All rights reserved.
**
****************************************************************************/

#ifndef SRV_H
#define SRV_H

struct srvhost {
  unsigned int pref;
  struct srvhost * next;
  unsigned int port;
  unsigned int weight;
  unsigned int rweight;
  char name[1];
};


extern void freesrvhost ( struct srvhost * );

extern struct srvhost * getsrv( const char * domain,
				const char * service,
				const char * protocol );

#endif
