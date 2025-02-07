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

#include <stdio.h>
#include "x.h"
#include "defines.h"
#include "externals.h"
#include "xexterns.h"

int usedefault = 0;
/* external routines called in this file */
extern void parseargs(), init_data(), init_mem(), init_gen(), init_canvas();
extern void init_color(), Clear(), event_loop();

main(argc, argv)
int argc;
char **argv;
{
	XSizeHints hint;
	XVisualInfo *visual_list, visual_template;
	static int i;

	parseargs(argc, argv);
	init_data();
	init_mem();
	/*
 	* Create the window to display the lattice dynamical system
 	*/
	hint.x = 0;
	hint.y = 0;
	hint.width = width;
	hint.height = height;
	hint.flags = PPosition | PSize;
	canvas = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
		hint.x, hint.y, hint.width, hint.height, 5, foreground, background);
	XSetStandardProperties(dpy, canvas, "Lds by Ron Record", 
		"Lds", None, argv, argc, &hint);
    XChangeProperty(dpy, canvas, XA_WM_CLASS, XA_STRING, 8, PropModeReplace, 
					"lds", strlen("lds"));
	XSelectInput(dpy,canvas,KeyPressMask|ExposureMask|StructureNotifyMask);
	XMapRaised(dpy, canvas);
	/*
 	* Create the window to display the spatial histogram
 	*/
	hint.x = (XDisplayWidth(dpy, screen) - width)/2;
	hint.y = (XDisplayHeight(dpy, screen) - height)/2;
	if (hint.x < 0) hint.x = 0;
	if (hint.y < 0) hint.y = 0;
	hint.width = width;
	hint.height = height;
	hint.flags = PPosition | PSize;
	spahis = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
		hint.x, hint.y, hint.width, hint.height, 5, foreground, background);
	XSetStandardProperties(dpy, spahis, "Spatial Histogram", 
		"Lds", None, argv, argc, &hint);
	XSelectInput(dpy,spahis,KeyPressMask|ExposureMask|StructureNotifyMask);
	if (sflag)
		XMapRaised(dpy, spahis);
	/*
 	* Create the window to display the site histogram curve
 	*/
	hint.x = XDisplayWidth(dpy, screen) - width - 50;
	hint.y = XDisplayHeight(dpy, screen) - height - 50;
	if (hint.x < 0) hint.x = 0;
	if (hint.y < 0) hint.y = 0;
	hint.width = width;
	hint.height = height;
	hint.flags = PPosition | PSize;
	hiswin = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
		hint.x, hint.y, hint.width, hint.height, 5, foreground, background);
	XSetStandardProperties(dpy, hiswin, "Site Histogram", 
		"Lds", None, argv, argc, &hint);
	XSelectInput(dpy,hiswin,KeyPressMask|ExposureMask|StructureNotifyMask);
	visual_template.class = PseudoColor;
	visual_list = XGetVisualInfo(dpy, VisualClassMask, &visual_template, &i);
	if (! visual_list)
		usedefault = 1;
	if (usedefault)
	  cmap = spcmap = hicmap = DefaultColormap(dpy, screen);
	else {
	  cmap = XCreateColormap(dpy, canvas, DefaultVisual(dpy, screen), AllocAll);
	  spcmap = XCreateColormap(dpy,spahis,DefaultVisual(dpy, screen), AllocAll);
	  hicmap = XCreateColormap(dpy,hiswin,DefaultVisual(dpy, screen), AllocAll);
	}
	if (hflag)
		XMapRaised(dpy, hiswin);
	if (displayplanes > 1) {
		if (!usedefault)
		    init_color(dpy,canvas,cmap,Colors,mincolor,mincolor,numcolors,
						numwheels, "lds", "Lds", 0);
		if (hflag)
			if (!usedefault)
			    init_color(dpy,hiswin,cmap,Colors,mincolor,mincolor,numcolors,
						numwheels, "lds", "Lds", 0);
		if (sflag)
			if (!usedefault)
			    init_color(dpy,spahis,cmap,Colors,mincolor,mincolor,numcolors,
						numwheels, "lds", "Lds", 0);
	}
	else
		XQueryColors(dpy, DefaultColormap(dpy, DefaultScreen(dpy)), 
				Colors, numcolors);
	pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), 
			hint.width, hint.height, DefaultDepth(dpy, screen));
	spamap = XCreatePixmap(dpy, DefaultRootWindow(dpy), 
			hint.width, hint.height, DefaultDepth(dpy, screen));
	init_canvas();
	Clear(canvas);
	if (hflag)
		Clear(hiswin);
	if (sflag)
		Clear(spahis);
	init_gen();
	for(;;)
	    event_loop();
}
