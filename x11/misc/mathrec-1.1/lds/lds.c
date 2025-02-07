/*************************************************************************
 *                                                                       *
 *  Copyright (c) 1988-1993 Ronald Joe Record                           *
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

/*
 *	Written 2/22/88 by Ronald Record (sco!rr) in partial fulfillment of
 *	the Mathematics Ph.D. requirements at the University of California at 
 *	Santa Cruz. This work is in part based upon articles by Doyne Farmer, 
 *	Jim Crutchfield and Kunihiko Kaneko.
 *
 *  X11 port begun 20 Jan 93 by Ronald Record (rr@sco.com) for fun
 */

#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include "x.h"
#include "defines.h"
#include "lds.h"

/* routines in this file */
void usage(), Clear();
void redisplay(), BufferSpace(), FlushHisbuf();
void resize(), event_loop(), freemem(), zero_mem();
void spahist(), hist(), adjconn(), zerohist(), zerospa();
int complds(), upd_lambda();
double map();

/* external routines called in this file */
extern void Getkey(), draw_hist(), init_mem(), init_gen(), next_gen();
extern void InitBuffer();

void
usage()
{
  printf("\nUsage: lds [-PSpAcdx] [-w wide] [-h high] [-n lambda] [-b#]\n");
  printf("\t\t[-f#] [-F freq] [-C EC] [-L EL] [-R ER] [-U EU] [-D ED]\n");
  printf("\t\t[-W width] [-H height] [-r range] [-M omega] [-T tall] [-E rate] [-i init]\n");
	printf("Where :\n");
	printf("\t'-A' creates site histogram curve\n");
	printf("\t'-p' indicates draw phase 1st differences\n");
	printf("\t'-c' indicates draw curves\n");
	printf("\t'-d' indicates demo mode (use with -f# option)\n");
	printf("\t'-x' indicates complex dynamical system mode\n");
	printf("\t'-P' indicates draw points (default is curves)\n");
	printf("\t'-[M|T] #' indicates use circle|tent map with omega|height=#\n");
	printf("\t'-E #' indicates evolve connection strengths at rate #\n");
	printf("\tEC|EL|ER|EU|ED indicates weight of ctr|lft|rgt|upr|lwr cell\n");
	printf("\twidth\t= width of window (default is full screen)\n");
	printf("\theight\t= height of window (default is full screen)\n");
	printf("\twide\t= width of lattice (default is full screen)\n");
	printf("\thigh\t= height of lattice (default is full screen)\n");
	printf("\tinit\t= initial condition (default is random)\n");
	printf("\tlambda\t= logistic parameter (default is 3.7)\n");
	printf("\t-b#\t= first generation graphed (default is 1)\n");
	printf("\t-f#\t= Final generation graphed (default is MAXSHORT)\n");
	printf("\tfreq\t= display every freq'th generation (1 is default)\n");
	printf("\trange\t= range outside of which differences are graphed\n");
	printf("During display, use of the keys 'cseEdBmHPLSXq' indicates:\n");
	printf("'continue' 'single step' 'erase' 'Erase all' 'draw' 'Begin again' 'multi-step'\n");
	printf("'Histogram' 'Points' 'Lines' 'Spin one color' 'Xtended spin' 'quit'\n");
		exit(1);
}

void
Clear(w) 
Window w;
{
	if (w == canvas) {
		XFillRectangle(dpy, pixmap, Data_GC[0], 0, 0, width, height);
		XCopyArea(dpy, pixmap, w, Data_GC[0], 0, 0, width, height, 0, 0);
		InitBuffer(&Points, numcolors);
	}
	else if (w == spahis) {
		XFillRectangle(dpy, spamap, Data_GC[0], 0, 0, width, height);
		XCopyArea(dpy, spamap, w, Data_GC[0], 0, 0, width, height, 0, 0);
		InitBuffer(&Points, numcolors);
	}
	else
		XClearWindow(dpy, w);
}

void
BufferSpace(color, x, y)
int color;
int x, y;
{
	if (Hpoints.npoints[color] == MAXPOINTS)
	{
		XDrawPoints(dpy, spahis, Data_GC[color],
		    Hpoints.data[color], Hpoints.npoints[color], CoordModeOrigin);
		XDrawPoints(dpy, spamap, Data_GC[color],
		    Hpoints.data[color], Hpoints.npoints[color], CoordModeOrigin);
		Hpoints.npoints[color] = 0;
	}
	Hpoints.data[color][Hpoints.npoints[color]].x = x;
	Hpoints.data[color][Hpoints.npoints[color]].y = height - y;
	++Hpoints.npoints[color];
}

void
FlushHisbuf()
{
	int color;
	for (color = mincolor; color < numcolors; ++color)
		if (Hpoints.npoints[color])
		{
		    XDrawPoints(dpy, spahis, Data_GC[color],
			    Hpoints.data[color], Hpoints.npoints[color], CoordModeOrigin);
		    XDrawPoints(dpy, spamap, Data_GC[color],
			    Hpoints.data[color], Hpoints.npoints[color], CoordModeOrigin);
		    Hpoints.npoints[color] = 0;
		}
}

void
redisplay(ev, w)
XExposeEvent    *ev;
Window w;
{
	/*
 	* Extract the exposed area from the event and copy
 	* from the saved pixmap to the window.
 	*/
	if (w == canvas)
		XCopyArea(dpy, pixmap, canvas, Data_GC[0], 
           ev->x, ev->y, ev->width, ev->height, ev->x, ev->y);
	else if (w == spahis)
		XCopyArea(dpy, spamap, spahis, Data_GC[0], 
           ev->x, ev->y, ev->width, ev->height, ev->x, ev->y);
}

void
resize(w)
Window w;
{
	Window r;
	int x, y;
	unsigned int bw, d, new_w, new_h;
	static XWindowChanges values;

	XGetGeometry(dpy,w,&r,&x,&y,&new_w,&new_h,&bw,&d);
	if ((new_w == width) && (new_h == height))
		return;
	freemem();
	width = new_w; height = new_h;
	XClearWindow(dpy, canvas);
	if (sflag)
		XClearWindow(dpy, spahis);
	if (hflag)
		XClearWindow(dpy, hiswin);
	if (pixmap)
		XFreePixmap(dpy, pixmap);
	pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), 
			width, height, DefaultDepth(dpy, screen));
	values.width = width;
	values.height = height;
	if (w == canvas) {
		XConfigureWindow(dpy,hiswin,CWWidth|CWHeight,&values);
		XConfigureWindow(dpy,spahis,CWWidth|CWHeight,&values);
	}
	else if (w == hiswin) {
		XConfigureWindow(dpy,canvas,CWWidth|CWHeight,&values);
		XConfigureWindow(dpy,spahis,CWWidth|CWHeight,&values);
	}
	else {
		XConfigureWindow(dpy,canvas,CWWidth|CWHeight,&values);
		XConfigureWindow(dpy,hiswin,CWWidth|CWHeight,&values);
	}
	xpoint = ypoint = 0;
	run = 1;
	init_mem();
	InitBuffer(&Points, numcolors);
	Clear(canvas);
	init_gen();
}

int
complds()
{
	if (!run)
		return(TRUE);
	next_gen();
	if (hflag)
		draw_hist();
	return(FALSE);
}

void
event_loop()
{
	int n;
	XEvent event;

	if (complds() == TRUE)
	    run=0;
	n = XEventsQueued(dpy, QueuedAfterFlush);
	while (n--) {
        XNextEvent(dpy, &event);
        switch(event.type) 
        {
        case KeyPress:
			Getkey(&event);
			break;
        case Expose:
			redisplay(&event, event.xexpose.window);
	       	break;
        case ConfigureNotify:
			resize(event.xconfigure.window);
	      	break;
        }
    }
}

void
freemem()
{
	static int i;
	extern XPoint *hpoints;

	for (i=0; i<width; i++)
		free(histarray[i]);
	for (i=0; i<high; i++) {
		free(lambda[i]);
		free(lftconn[i]);
		free(rgtconn[i]);
		free(ctrconn[i]);
		free(uprconn[i]);
		free(lwrconn[i]);
		free(currentgen[i]);
		free(nextgen[i]);
		free(avg[i]);
	}
	free(lambda);
	free(lftconn);
	free(rgtconn);
	free(ctrconn);
	free(uprconn);
	free(lwrconn);
	free(currentgen);
	free(nextgen);
	free(histogram);
	free(histarray);
	free(diff); 
	free(avg);
	free(hpoints);
}

void
zerospa()
{
	static int i, j;

	for (i=0; i<width; i++)
		for (j=0; j<height; j++)
			histarray[i][j] = 0;
}

void
zerohist()
{
	static int i;

	for (i=0; i<width; i++)
		histogram[i] = 0;
}

void
zero_mem()
{
	static int i, j;

	for (i=0; i<wide; i++) {
		diff[i] = 0.0;
		for (j=0; j<high; j++) {
			lambda[j][i] = 0.0;
			lftconn[j][i] = 0.0;
			rgtconn[j][i] = 0.0;
			ctrconn[j][i] = 0.0;
			currentgen[j][i] = 0.0;
			nextgen[j][i] = 0.0;
			avg[j][i] = 0.0;
		}
		zerohist();
	}
}

int
upd_lambda() 
{
	static int i, j;

	if (xavg)
	  if (numgen % xavg) {
		for (i=0;i<wide;i++)
		  for (j=0;j<high;j++)
			avg[j][i] += currentgen[j][i];
		return(1);
	  }
	for (i=0;i<wide;i++)
	  for (j=0;j<high;j++)
		avg[j][i] = (avg[j][i] + (double)currentgen[j][i]) / (double)xavg;
	for (i=0;i<high;i++) {
	  if (circle)
		lambda[i][0] = avg[i][wide-1]/M_2PI;
	  else if (tent)
		lambda[i][0] = avg[i][wide-1];
	  else 
		lambda[i][0] = 3.0 + avg[i][wide-1];
	}
	for (i=1;i<wide;i++)
	  for (j=0;j<high;j++) {
		if (circle)
			lambda[j][i] = avg[j][i-1]/M_2PI;
		else if (tent)
			lambda[j][i] = avg[j][i-1];
		else 
			lambda[j][i] = 3.0 + avg[j][i-1];
	  }
	return(0);
}

void
spahist(sites)
double *sites;
{
	static int i, j, k, l, n;

	for (i=0;i<wide;i++) {
		j = (int)(sites[i]*height);
		k = (i*width)/wide;
		n = ++histarray[k][j];
		if (n > maxhist)
			maxhist = n;
		if (n < minhist)
			minhist = n;
		l = ((histarray[k][j]-minhist)*(numcolors-mincolor-1)/maxhist)+mincolor;
		BufferSpace(l, k, j);
	}
}

void
hist(sites)
double *sites;
{
	int i;

	for (i=0;i<wide;i++)
		histogram[(int)floor(sites[i]*width)]++;
}

void
adjconn()
{
	static int i,j,k,l,m,n;

	for (i=0;i<wide;i++) {
		if (i == wide -1)
			k = 0;
		else
			k = i + 1;
		if (i == 0)
			l = wide - 1;
		else
			l = i - 1;
		for (j=0;j<high;j++) {
			if (j == high -1)
				m = 0;
			else
				m = j + 1;
			if (j == 0)
				n = high - 1;
			else
				n = j - 1;
			rgtconn[j][i]=erate*(currentgen[j][i]*currentgen[j][k]);
			lftconn[j][i]=erate*(currentgen[j][i]*currentgen[j][l]);
			uprconn[j][i]=erate*(currentgen[j][i]*currentgen[m][i]);
			lwrconn[j][i]=erate*(currentgen[j][i]*currentgen[n][i]);
			ctrconn[j][i] = 1.0 - 
				(rgtconn[j][i] + lftconn[j][i] + uprconn[j][i] + lwrconn[j][i]);
		}
	}
}

double
map(npar,x)
double npar;
double x;
{
	double y;

	x = fmod(x,1.0);	/*	Make sure value is between 0 and 1 */
	if (logistic)
		y = npar * x * (1.0 - x);
	else if (circle)
		y = omega + x - npar * sin(M_2PI*x);
	else if (tent)
		if (x < npar)
			y = incline*x;
		else
			y = decline*(x-1.0);
	y = fmod(y,1.0);	/*	Make sure return value is between 0 and 1 */
	return(y);
}
