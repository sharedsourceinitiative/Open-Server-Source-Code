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

Display *dpy;
int xmax, ymax;
int screen, fd;
int histcol=1;
int freq=1;
int parity=0;
int width=0; 
int wide=0; 
int height=0; 
int high=0; 
int quilt=1;
int begin=1;
int col=1;
int draw=1;
int minhist = MAXINT;
int maxhist = 0;
int logistic=1;			/* use logistic map */
int tent=0;				/* use tent map */
int circle=0;			/* use circle map if set */
int Cflag=0;			/* Show connection strengths */
int Eflag=0;			/* Evolve connection strengths */
int sflag=0;			/* spatial histogram off */
int hflag=0;			/* site histogram off */
int oflag=0;			/* output file off */
int vflag=0;			/* don't display text windows describing dynamics */
int ranlam=0;			/* random lambda values off */
int linlam=0;			/* linearly assigned lambda values off */
int perlam=0;			/* periodically assigned lambda values off */
int rancon=0;			/* random connection values off */
int lincon=0;			/* linearly assigned connection values off */
int percon=0;			/* periodically assigned connection values off */
int eflag=0;			/* erase flag */
int Pflag=0;			/* draw points flag */
int mflag=0; 			/* monochrome display (site value > 1/2 -> color) */
int pflag=0;			/* generate phase 1st differences */
int iflag=0;			/* initial conditions specified */
int cflag=0;			/* draw curves */
int xflag=0;			/* coupled map lattice mode */
int xavg=4;				/* number of generations over which to average */
int two_dim=0;			/* two dimensional lattice ? */
int lamvalset=0;		/* non-linearity parameter specified */
double RANGE=0.01;		/* range outside of which differences get graphed*/
double erate=0.01;		/* rate at which connections evolve */
double ORANGE=0.99;		/* 1-RANGE */
double lamval=3.8;		/* default lambda value */
double EC=0.8;			/* default center cell weight */
double ER=0.05;		/* default right cell weight */
double EL=0.05;		/* default left cell weight */
double EU=0.05;		/* default upper cell weight */
double ED=0.05;		/* default lower cell weight */
double omega;			/* frequency in circle map or height in tent map */
double incline, decline;	/* slope up and down (if tent) */
char *valinit, *laminit;	/* initial values of array and lambdas */
double **ctrconn;			/* initial values of center connection strength */
double **rgtconn; 			/* initial values of right connection strength */
double **lftconn; 			/* initial values of left connection strength */
double **uprconn; 			/* initial values of upper connection strength */
double **lwrconn; 			/* initial values of lower connection strength */
int periodic=1;				/* periodic boundary conditions */
int boundflag=0;			/* alternate boundary conditions specified */
int A=1;					/* frequency of sin wave of initial lambdas */
int G=1;					/* frequency of sin wave of initial connections */
double **currentgen, **nextgen;	/* the current and next generation */
double **avg;				/* array of averages to be used as parameters */
double **lambda;				/* array of non-linearity parameters */
double *diff;				/* array of 1st differences */
double boundary=0.0;		/* default fixed boundary condition */
int *histogram;				/* array of site histogram values */
int **histarray;			/* array of spatial histogram values */
char *outname = "lds.out";

unsigned long foreground, background, numgen;

Window canvas, hiswin, spahis;

typedef struct {
	int x, y;
} xy_t;

points_t Points, Hpoints;

GC Data_GC[MAXCOLOR];

int	mincolor, numcolors, displayplanes;
Pixmap  pixmap, spamap;
Colormap cmap, spcmap, hicmap;
XColor	Colors[MAXCOLOR];
int	numwheels=MAXWHEELS;
int     rgb_max=65000, stripe_interval=7;
int	savefile=0;
int	run=1, xpoint=0, ypoint=0;
extern double drand48(), sin(), fabs(), floor(), fmod();
extern void srand48();
