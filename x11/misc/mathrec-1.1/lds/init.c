/*************************************************************************
 *                                                                       *
 *  Copyright (c) 1992, 1993 Ronald Joe Record                           *
 *                                                                       *
 *  All rights reserved. No part of this program or publication may be   *
 *  reproduced, transmitted, transcribed, stored in a retrieval system,  *
 *  or translated into any language or computer language, in any form or *
 *  by any means, electronic, mechanical, magnetic, optical, chemical,   *
 *  biological, or otherwise, without the prior written permission of:   *
 *                                                                       *
 *      Ronald Joe Record (408) 458-3718                                 *
 *      212 Owen St., Santa Cruz, California 95062 USA                   *
 *                                                                       *
 *************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "x.h"
#include "defines.h"
#include "externals.h"
#include "xexterns.h"

XPoint *hpoints;

/* routines in this file */
void init_data(), init_canvas(), init_mem(), init_gen();

/* external routines */
extern void InitBuffer(), phasediff(), grafline(), grafgen(), zero_mem();
extern void FlushPixmap();

void
init_data()
{
    dpy = XOpenDisplay("");
    screen = DefaultScreen(dpy);
	background = BlackPixel(dpy, screen);
	foreground = WhitePixel(dpy, screen);
	srand48(getpid());
	if (width == 0)
	  if ((width = wide) == 0)
		if (two_dim)
		  width = DEF_2D_WIDTH;
		else
		  width = XDisplayWidth(dpy, screen);
	if (height == 0)
	  if ((height = high) == 0)
		if (two_dim)
		  height = DEF_2D_HEIGHT;
		else
		  height = XDisplayHeight(dpy, screen);
	if (wide == 0)
		wide = width;
	if (high == 0)
		if (two_dim)
			high = height;
		else
			high = 1;
	if (high == 1) { /* one dimensional LDS */
		EU = ED = 0.0;
		EC = 1.0 - (ER + EL);
	}
	else /* two dimensional LDS */
		EC = 1.0 - (EU + ED + ER + EL);
	numgen=1;
	parity = numgen % freq;
	incline=omega/lamval;
	decline=omega/(lamval-1.0);
	numcolors = XDisplayCells(dpy, XDefaultScreen(dpy));
	if (numcolors > MAXCOLOR)
		numcolors = MAXCOLOR;
	mincolor = (numcolors/16) + 1;
	displayplanes = DisplayPlanes(dpy, XDefaultScreen(dpy));
	xpoint = ypoint = 0;
	InitBuffer(&Points, numcolors);
}

void
init_canvas()
{
	static int i;

	/*
 	* create default, writable, graphics contexts for the canvas.
 	*/
	for (i=0; i<numcolors; i++) {
		Data_GC[i] = XCreateGC(dpy, DefaultRootWindow(dpy),
			(unsigned long) NULL, (XGCValues *) NULL);
		/* set the background to black */
		XSetBackground(dpy,Data_GC[i],BlackPixel(dpy,XDefaultScreen(dpy)));
		/* set the foreground of the ith context to i */
		XSetForeground(dpy, Data_GC[i], i);
	}
	XSetForeground(dpy,Data_GC[0],BlackPixel(dpy,XDefaultScreen(dpy)));
	XSetForeground(dpy,Data_GC[1],WhitePixel(dpy,XDefaultScreen(dpy)));
}

/*
 *  init_mem() - malloc necessary memory
 */
void
init_mem()
{
	static int i;

	if ((diff = (double *)malloc(wide*sizeof(double)))==NULL){
			printf("Error malloc'ing diff. Exiting\n");
			exit(-1);
	}
	if ((histogram=(int *)malloc((unsigned)(wide*sizeof(int))))==NULL){
		printf("Error malloc'ing histogram. Exiting\n");
		exit(-1);
	}
	if ((hpoints=(XPoint *)malloc((unsigned)(wide*sizeof(XPoint))))==NULL){
		printf("Error malloc'ing hpoints. Exiting\n");
		exit(-1);
	}
	if ((histarray=(int **)malloc((unsigned)(width*sizeof(int *)))) == 
	  (int **)0) {
		printf("Error malloc'ing histarray. Exiting\n");
		exit(-1);
	}
	if ((lambda = (double **)malloc(high*sizeof(double)))==NULL) {
		printf("Error malloc'ing lambda. Exiting\n");
		exit(-1);
	}
	if ((lftconn = (double **)malloc(high*sizeof(double)))==NULL) {
		printf("Error malloc'ing lftconn. Exiting\n");
		exit(-1);
	}
	if ((rgtconn = (double **)malloc(high*sizeof(double)))==NULL) {
		printf("Error malloc'ing rgtconn. Exiting\n");
		exit(-1);
	}
	if ((ctrconn = (double **)malloc(high*sizeof(double)))==NULL) {
		printf("Error malloc'ing ctrconn. Exiting\n");
		exit(-1);
	}
	if ((uprconn = (double **)malloc(high*sizeof(double)))==NULL) {
		printf("Error malloc'ing uprconn. Exiting\n");
		exit(-1);
	}
	if ((lwrconn = (double **)malloc(high*sizeof(double)))==NULL) {
		printf("Error malloc'ing lwrconn. Exiting\n");
		exit(-1);
	}
	if ((avg=(double **)malloc(high*sizeof(double)))==NULL){
		printf("Error malloc'ing avg. Exiting\n");
		exit(-1);
	}
	if ((currentgen=(double **)malloc(high*sizeof(double)))==NULL){
		printf("Error malloc'ing currentgen. Exiting\n");
		exit(-1);
	}
	if ((nextgen = (double **)malloc(high*sizeof(double)))==NULL) {
		printf("Error malloc'ing nextgen. Exiting\n");
		exit(-1);
	}
	for (i=0; i<width; i++) {
	  if ((histarray[i]=(int *)malloc((unsigned)(height*sizeof(int))))==INULL){
		printf("Error malloc'ing histarray[%d]. Exiting\n",i);
		exit(-1);
	  }
	}
	for (i=0; i<high; i++) {
	  if ((lambda[i] = (double *)malloc(wide*sizeof(double)))==NULL) {
		printf("Error malloc'ing lambda. Exiting\n");
		exit(-1);
	  }
	  if ((lftconn[i] = (double *)malloc(wide*sizeof(double)))==NULL) {
		printf("Error malloc'ing lftconn. Exiting\n");
		exit(-1);
	  }
	  if ((rgtconn[i] = (double *)malloc(wide*sizeof(double)))==NULL) {
		printf("Error malloc'ing rgtconn. Exiting\n");
		exit(-1);
	  }
	  if ((uprconn[i] = (double *)malloc(wide*sizeof(double)))==NULL) {
		printf("Error malloc'ing uprconn. Exiting\n");
		exit(-1);
	  }
	  if ((lwrconn[i] = (double *)malloc(wide*sizeof(double)))==NULL) {
		printf("Error malloc'ing lwrconn. Exiting\n");
		exit(-1);
	  }
	  if ((ctrconn[i] = (double *)malloc(wide*sizeof(double)))==NULL) {
		printf("Error malloc'ing ctrconn. Exiting\n");
		exit(-1);
	  }
	  if ((avg[i] = (double *)malloc(wide*sizeof(double)))==NULL){
		printf("Error malloc'ing avg. Exiting\n");
		exit(-1);
	  }
	  if ((currentgen[i] = (double *)malloc(wide*sizeof(double)))==NULL){
		printf("Error malloc'ing currentgen. Exiting\n");
		exit(-1);
	  }
	  if ((nextgen[i] = (double *)malloc(wide*sizeof(double)))==NULL) {
		printf("Error malloc'ing nextgen. Exiting\n");
		exit(-1);
	  }
	}
}

void
init_gen()
{
	static char *tmpvalptr; 
	static int first, i, j, k, F;

	zero_mem();
	for (j=0;j<high;j++) {
	  tmpvalptr=valinit;
	  if ((*valinit != 'r') && (*valinit != 'p') && (iflag)) {
		if (*valinit == 'l') {
			for (i=0;i<wide;i++)
				currentgen[j][i] = (double)(i+j)/(wide+high);
		}
		else {
			k = (wide/2) - (strlen(valinit)/2) - 1;
			while(*tmpvalptr != '\0') {
				first = *tmpvalptr++ - '0';
				currentgen[k++][high/2] = 
						(double)(first + drand48()) / (double)numcolors;
			}
		}
	  }
	  else if (*tmpvalptr == 'p') {
		tmpvalptr++;
		if (*tmpvalptr == '\0')
			F = 1;
		else
			while (*tmpvalptr != '\0')
		  		F = (F*10)+(*tmpvalptr++ - '0');
		for (i=0;i<wide;i++) {
	 	  currentgen[j][i] =
			(sin((double)((double)(i+j)/(double)(wide+high))*M_PI*F)+1.0)/2.0;
		}
	  }
	  else {
		for (i=0;i<wide;i++)
			currentgen[j][i] = drand48();
	  }
	  if (rancon)				/* randomly assign connection values */
		for (i=0;i<wide;i++) {
			ctrconn[j][i] = drand48();
			rgtconn[j][i] = lftconn[j][i] = uprconn[j][i] = lwrconn[j][i] = 
					(1.0 - ctrconn[j][i])/4.0;
		}
	  else if (lincon)				/* linearly assign connection values */
		for (i=0;i<wide;i++) {
			ctrconn[j][i] = (double)(i+j)/(wide+high);
			rgtconn[j][i] = lftconn[j][i] = uprconn[j][i] = lwrconn[j][i] = 
					(1.0 - ctrconn[j][i])/4.0;
		}
	  else if (percon)			/* periodically assign connection values */
		for (i=0;i<wide;i++) {
			ctrconn[j][i] = 
				(sin(((double)(i+j)/(double)(wide+high))*M_PI*G)+1.0)/2.0;
			rgtconn[j][i] = lftconn[j][i] = uprconn[j][i] = lwrconn[j][i] = 
					(1.0 - ctrconn[j][i])/4.0;
		}
	  else					/* all connection strengths equal */
		for (i=0;i<wide;i++) {
			ctrconn[j][i] = EC;
			rgtconn[j][i] = ER;
			lftconn[j][i] = EL;
			uprconn[j][i] = EU;
			lwrconn[j][i] = ED;
		}
	  if (ranlam)				/* randomly assign parameter values */
		for (i=0;i<wide;i++) {
			if (tent)
				lambda[j][i] = drand48();
			else if (circle)
				lambda[j][i] = drand48()/M_2PI;
			else
				lambda[j][i] = drand48() + 3.0;
		}
	  else if (linlam)				/* linearly assign parameter values */
		for (i=0;i<wide;i++) {
			if (tent)
				lambda[j][i] = ((double)(i+j)/(wide+high));
			else if (circle)
				lambda[j][i] = ((double)(i+j)/(wide+high))/M_2PI;
			else
				lambda[j][i] = 3.0 + ((double)(i+j)/(wide+high));
		}
	  else if (perlam)				/* periodically assign parameter values */
		for (i=0;i<wide;i++) {
			if (circle)
			 lambda[j][i] = 
			  ((sin(((double)(i+j)/(double)(wide+high))*M_PI*A)+1.0)/2.0)/M_2PI;
			else if (tent)
			 lambda[j][i] = 
				((sin(((double)(i+j)/(double)(wide+high))*M_PI*A)+1.0)/2.0);
			else
			 lambda[j][i] = 
				3.0+((sin(((double)(i+j)/(double)(wide+high))*M_PI*A)+1.0)/2.0);
		}
	  else if (xflag) {	/* parameter values equal state of cell to left */
		if (circle)
			lambda[j][0] = currentgen[j][wide-1]/M_2PI;
		else if (tent)
			lambda[j][0] = currentgen[j][wide-1];
		else 
			lambda[j][0] = 3.0 + currentgen[j][wide-1];
		for (i=1;i<wide;i++) {
			if (circle)
				lambda[j][i] = currentgen[j][i-1]/M_2PI;
			else if (tent)
				lambda[j][i] = currentgen[j][i-1];
			else 
				lambda[j][i] = 3.0 + currentgen[j][i-1];
		}
	  }
	  else 					/* all parameter values equal lamval */
		for (i=0;i<wide;i++) {
			if (circle)
				lambda[j][i] = lamval/M_2PI;
			else 
				lambda[j][i] = lamval;
		}
	  if (begin == 1) {
		if (pflag)
			phasediff(currentgen[j]);
		else if (cflag)
			grafline(currentgen[j]);
		else
			grafgen(currentgen[j]);
	  }
	}
	if (!cflag) {
		FlushPixmap(dpy, pixmap, Data_GC, &Points, mincolor, numcolors);
		XCopyArea(dpy, pixmap, canvas, Data_GC[0], 
						0, height - high, wide, high, 0, height - high);
	}
}
