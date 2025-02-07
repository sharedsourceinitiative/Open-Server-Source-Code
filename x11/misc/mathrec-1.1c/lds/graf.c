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

#include "x.h"
#include "defines.h"
#include "externals.h"
#include "xexterns.h"

/* routines in this file */
void grafline(), grafgen(), phasediff(), draw_hist(), draw_spa();

/* external routines called in this file */
extern void BufferPoint(), FlushBuffer(), spahist(), hist(), Clear();
extern void BufferInPixmap(), BufferSpace(), FlushHisbuf();

extern XPoint *hpoints;

void
grafline(line)
double *line;
{
	static int i;

	if (hflag)
		hist(line);
	if (sflag)
		spahist(line);
	if (draw && (!parity)) {
		col++;
		if (col == numcolors) col=mincolor;
		if (eflag)
			Clear(canvas);
		if (Pflag) {
			for (i=0;i<wide;i++)
				BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, col, 
							i, (int)((1.0 - line[i])*height));
			FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, col, col+1);
		}
		else {
			for (i=0; i<wide; i++) {
				hpoints[i].x = i;
				hpoints[i].y = (int)((1.0-line[i])*height);
			}
			XDrawLines(dpy,canvas,Data_GC[col],hpoints,wide,CoordModeOrigin);
			XDrawLines(dpy,pixmap,Data_GC[col],hpoints,wide,CoordModeOrigin);
		}
	}
}

void
grafgen(line)
double *line;
{
	int i;

	if (hflag)
		hist(line);
	if (sflag)
		spahist(line);
	if (draw && (!parity)) {
		for (i=0; i<wide; i++) {
			if (mflag) {
				if (line[i] > 0.5)
					BufferInPixmap(dpy, pixmap, Data_GC, &Points, 
								mincolor + 1, xpoint + i, height - ypoint - 1);
				else
					BufferInPixmap(dpy, pixmap, Data_GC, &Points, 
							(numcolors+mincolor)/2, xpoint+i, height-ypoint-1);
			}
			else
				BufferInPixmap(dpy, pixmap, Data_GC, &Points, 
							(int)(line[i]*(numcolors-mincolor-1))+mincolor, 
							xpoint + i, height - ypoint - 1);
		}
		ypoint++;
		if (ypoint >= height) {
			ypoint = 0;
			xpoint += wide;
			if ((xpoint + wide) > width)
				xpoint = 0;
		}
		if ((ypoint >= high) && (!quilt))
			ypoint = 0;
	}
}

void
phasediff(line)
double *line;
{
	int i;

	if (draw && (!parity)) {
		if (boundflag == 1)
			diff[0] = 0.0;
		else if (boundflag == 2)
			diff[0] = fabs(line[0] - drand48());
		else if (boundflag == 3)
			diff[0] = fabs(line[0] - boundary);
		else
			diff[0] = fabs(line[0]-line[wide-1]);
		if (diff[0] > RANGE)
			diff[0] = (diff[0]-RANGE)/ORANGE;
		else
			diff[0] = 0.0;
		for (i=1; i<wide; i++) {
			diff[i] = fabs(line[i]-line[i-1]);
			if (diff[i] > RANGE)
				diff[i] = (diff[i]-RANGE)/ORANGE;
			else
				diff[i] = 0.0;
		}
		if (cflag)
			grafline(diff);
		else
			grafgen(diff);
	}
}

void
draw_spa()
{
	static int i, j, k, spacol;
	extern int maxhist, minhist;

	k = numcolors - mincolor - 1;
	for (j=0;j<height;j++) {
		for (i=0;i<width;i++) {
			if (histarray[i][j]) {
				spacol = ((histarray[i][j] - minhist) * k / maxhist) + mincolor;
				BufferSpace(spacol, i,  j);
			}
		}
	}
	FlushHisbuf();
}

void
draw_hist()
{
	int i;
	unsigned long numsites;

	numsites = high*wide*(numgen - begin - 1)/60;
	if (numsites) {
		Clear(hiswin);
		for (i=0;i<wide;i++) {
			hpoints[i].x = i;
			hpoints[i].y = height - (int)(histogram[i]*height/numsites);
		}
		if (Pflag)
			XDrawPoints(dpy, hiswin, Data_GC[histcol], hpoints, wide,
					CoordModeOrigin);
		else
			XDrawLines(dpy, hiswin, Data_GC[histcol], hpoints, wide,
					CoordModeOrigin);
		histcol++;
		if (histcol >= numcolors)
			histcol = mincolor;
	}
}
