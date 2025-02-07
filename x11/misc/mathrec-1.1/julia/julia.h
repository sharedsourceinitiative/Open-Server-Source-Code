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

/*
 *	@(#) julia.h 1.3 93/11/01 MRINC
 */
/*
 *	Written by Ron Record (rr@sco.com) 03 Sep 1991.
 */

int bottom, screen;
Display*	dpy;
char*		displayname = 0;

extern double log();
extern double fabs();
extern long time();
extern int optind;
extern char *optarg;

unsigned long foreground, background;

Window canvas;

void     resize();
void     redisplay();
void     quit();
void     Spin();
void     start_iterate();
void     stop_iterate();
void	 show_defaults();
void	 StartRubberBand();
void	 TrackRubberBand();
void	 EndRubberBand();
void	 CreateXorGC();
void	 InitBuffer();
void	 BufferPoint();
void	 FlushBuffer();
int		 compjulia();
int		 compmandel();

points_t Points;
image_data_t rubber_data;

PFI func;
PFI Funcs[2] = { compjulia, compmandel };

GC Data_GC[MAXCOLOR], RubberGC;

int Aflag=0, Bflag=0, aflag=0, bflag=0, wflag=0, hflag=0, Rflag=0, second=0;
int maxcolor=256, startcolor=17, color_offset=96, delay=0;
int width=512, height=384, res=1, kmax=256, M=100;
int symetrical, w2, h2, rem;
double center_x = 0.0, center_y = 0.0, p=0.32, q=0.043;
double	min_a=-1.5, min_b=-1.5, a_range=3.0, b_range=3.0;
double  max_a=1.5, max_b=1.5;
double  a_inc, b_inc, a, b;
double xper=0.0, yper=0.0, delta=0.01;
int	numcolors=16, numfreecols, displayplanes, lowrange;
xy_t	point;
Pixmap  pixmap;
Colormap cmap;
XColor	Colors[MAXCOLOR];
int  *exponents[MAXFRAMES];
double  a_minimums[MAXFRAMES], b_minimums[MAXFRAMES];
double  a_maximums[MAXFRAMES], b_maximums[MAXFRAMES];
double prob=0.5;
int  mincnt, maxcnt; 
int     expind[MAXFRAMES]={0}, resized[MAXFRAMES]={0};
int	numwheels=0, negative=1, logindex=1;
int     nostart=1, stripe_interval=7;
int	save=1, show=0, spinlength=256, savefile=0, restfile=0, storefile=0;
int	maxframe=0, frame=0, dorecalc=0, run=1, demo=0;
char	*savname="julia.sav";
char	*outname="julia.out";
char	*inname="julia.sav";
char *path;
