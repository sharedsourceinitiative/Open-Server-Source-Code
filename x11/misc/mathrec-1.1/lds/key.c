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
#include <stdlib.h>
#include "x.h"
#include "defines.h"
#include "externals.h"
#include "xexterns.h"

/* external routines called in this file */
extern void save_to_file(), redraw(), init_gen(), FlushBuffer(), Clear();
extern void init_color(), zerospa(), zerohist(), FlushHisbuf(), draw_spa();
extern void Spin(), FlushPixmap();

void
Getkey(event)
XKeyEvent *event;
{
	unsigned char key;
	static int i, spinning=0, spindir=0;
	extern void Cleanup(), write_cmap();
	extern int usedefault;

    if (XLookupString(event, (char *)&key, sizeof(key), (KeySym *)0,
            (XComposeStatus *) 0) > 0)
                switch (key) {
    case '\015': /* write out current colormap to $HOME/.<prog>map */
    		write_cmap(dpy,cmap,Colors,numcolors,"lds","Lds");
			break;
	case '1':					
	case '2':					
	case '3':					
	case '4':					
	case '5':					
	case '6':					
	case '7':					
	case '8':					
	case '9':					/* set frequency of display */
			freq = atoi(&key);
			break;
	case '<':
			freq /= 2;
			if (freq < 1)
				freq = 1;
			break;
	case '>':
			freq *= 2;
			break;
	case '-':
			freq--;
			if (freq < 1)
				freq = 1;
			break;
	case '+':
			freq++;
			break;
	case 'a':
	case 'A': save_to_file(spamap); break;
	case 'B':					/* begin again */
			Clear(canvas);
			if (sflag) {
				Clear(spahis);
				zerospa();
			}
			if (hflag) {
				Clear(hiswin);
				zerohist();
			}
			Cflag=0;
			xpoint = ypoint = 0;
			init_gen();
			break;
	case 'c':					/* toggle curve/cell display */
			cflag = (!cflag);
			Cflag=0;
			Clear(canvas);
			break;
	case 'd':					/* don't erase */
			eflag=0;
			break;
	case 'D': 
			if (cflag)
			  FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,mincolor,numcolors);
			else {
			  FlushPixmap(dpy,pixmap,Data_GC,&Points,mincolor,numcolors);
			  XCopyArea(dpy,pixmap,canvas,Data_GC[0],0,0,width,height,0,0);
			}
			if (sflag)
				FlushHisbuf();
			break;
	case 'e':				/* recalculate and display spatial histogram */
			if (sflag)
				draw_spa();
			break;
	case 'E':				/* clear screen and show one gen at a time */
			Clear(canvas);
			eflag=1;
			break;
	case 'f':
	case 'F': save_to_file(pixmap); break;
	case 'h':					/* toggle spatial histogram tracking */
			sflag = (!sflag);
			zerospa();
			if (sflag) {
				XMapRaised(dpy, spahis);
				if (!usedefault)
				    init_color(dpy,spahis,spcmap,Colors,mincolor,mincolor,numcolors,
						numwheels, "lds", "Lds", 0);
				Clear(spahis);
			}
			else
				XUnmapWindow(dpy, spahis);
			break;
	case 'H':					/* toggle site histogram tracking */
			hflag = (!hflag);
			if (hflag) {
				XMapRaised(dpy, hiswin);
				if (!usedefault)
				    init_color(dpy,hiswin,hicmap,Colors,mincolor,mincolor,numcolors,
						numwheels, "lds", "Lds", 0);
			}
			else {
				XUnmapWindow(dpy, hiswin);
				zerohist();
			}
			break;
	case 'i': if (stripe_interval > 0) {
				stripe_interval--;
		  		if (displayplanes > 1) {
				  if (event->window == canvas)
					if (!usedefault)
					    init_color(dpy,canvas,cmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				  else if (event->window == spahis)
					if (!usedefault)
					    init_color(dpy,spahis,spcmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				  if (event->window == hiswin)
					if (!usedefault)
					    init_color(dpy,hiswin,hicmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				}
		      }
		      break;
	case 'I': stripe_interval++;
		  	  if (displayplanes > 1) {
				  if (event->window == canvas)
					if (!usedefault)
					    init_color(dpy,canvas,cmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				  else if (event->window == spahis)
					if (!usedefault)
					    init_color(dpy,spahis,spcmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				  if (event->window == hiswin)
					if (!usedefault)
					    init_color(dpy,hiswin,hicmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
			  }
		  break;
	case 'p':					/* toggle display phase differences */
			pflag=(!pflag);
			break;
	case 'P':					/* draw points */
			if (!Pflag)
				Clear(canvas);
			Pflag=1;
			break;
	case 'L':					/* draw lines, not points */
			if (Pflag)
				Clear(canvas);
			Pflag=0;
			break;
	case 'm':					/* don't single step */
			sflag=0;
			break;
	case 'R':
			spinning = 0; break;
	case 'S': spinning=1; spindir=(!spindir);
		   Spin(dpy, cmap, Colors, mincolor, numcolors, 0, spindir); 
		   break;
	case 's':					/* single step */
			sflag=1;
			break;
	case 'W': if (numwheels < MAXWHEELS)
			numwheels++;
		   else
			numwheels = 0;
		   if (displayplanes > 1) {
			    if (event->window == canvas)
					if (!usedefault)
					    init_color(dpy,canvas,cmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				else if (event->window == spahis)
					if (!usedefault)
					    init_color(dpy,spahis,spcmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				if (event->window == hiswin)
					if (!usedefault)
					    init_color(dpy,hiswin,hicmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
		   }
		   break;
	case 'w': if (numwheels > 0)
			numwheels--;
		   else
			numwheels = MAXWHEELS;
		   if (displayplanes > 1) {
			    if (event->window == canvas)
					if (!usedefault)
					    init_color(dpy,canvas,cmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				else if (event->window == spahis)
					if (!usedefault)
					    init_color(dpy,spahis,spcmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
				if (event->window == hiswin)
					if (!usedefault)
					    init_color(dpy,hiswin,hicmap,Colors,mincolor,mincolor,
								numcolors, numwheels, "lds", "Lds", 0);
		   }
		   break;
	case 'x': Clear(event->window); break;
	case 'X':					/* toggle complex dynamical systems mode */
			xflag=(!xflag);
			break;
	case 'q':
	case 'Q': Cleanup(); exit(0); break;
	default:  break;
	}
	if (spinning)
		Spin(dpy, cmap, Colors, mincolor, numcolors, 0, spindir); 
}
