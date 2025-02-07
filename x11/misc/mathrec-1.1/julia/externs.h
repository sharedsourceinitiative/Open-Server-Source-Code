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
 *	@(#) externs.h 1.2 93/10/08 MRINC
 */
/*
 *	Written by Ron Record (rr@sco.com) 19 Mar 1993.
 */

#include "x.h"
#include "libXrr.h"

extern int bottom, screen;
extern Display*	dpy;
extern char*		displayname;

extern double log();
extern double fabs();
extern long time();
extern int optind;
extern char *optarg;

extern unsigned long foreground, background;

extern Window canvas;

extern void     resize();
extern void     redisplay();
extern void     quit();
extern void     Spin();
extern void     start_iterate();
extern void     stop_iterate();
extern void	 show_defaults();
extern void	 StartRubberBand();
extern void	 TrackRubberBand();
extern void	 EndRubberBand();
extern void	 CreateXorGC();
extern void	 InitBuffer();
extern void	 BufferPoint();
extern void	 FlushBuffer();
extern int		 compjulia();
extern int		 compmandel();

extern points_t Points;
extern image_data_t rubber_data;

extern PFI func;
extern PFI Funcs[];

extern GC Data_GC[], RubberGC;

extern int Aflag, Bflag, aflag, bflag, wflag, hflag, Rflag, second;
extern int maxcolor, startcolor, color_offset;
extern int width, height, res, kmax, M;
extern int symetrical, w2, h2, rem;
extern double center_x, center_y, p, q;
extern double	min_a, min_b, a_range, b_range;
extern double  max_a, max_b;
extern double  a_inc, b_inc, a, b;
extern double xper, yper, delta;
extern int	numcolors, numfreecols, displayplanes, lowrange;
extern xy_t	point;
extern Pixmap  pixmap;
extern Colormap cmap;
extern XColor	Colors[];
extern int  *exponents[];
extern double  a_minimums[], b_minimums[];
extern double  a_maximums[], b_maximums[];
extern double prob;
extern int  mincnt, maxcnt; 
extern int     expind[], resized[];
extern int	numwheels, negative, logindex;
extern int     nostart, stripe_interval;
extern int	save, show, spinlength, savefile, restfile, storefile;
extern int	maxframe, frame, dorecalc, run, demo;
extern char	*savname;
extern char	*outname;
extern char	*inname;
