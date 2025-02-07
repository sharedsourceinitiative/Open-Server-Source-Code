/*
 *    @(#) hop.c 1.3 93/11/02 MRINC
 *
 * MODIFICATION HISTORY
 *	- created 30 Nov 1990 by Ron Record (rr@sco.com)
 *	  based upon an algorithm published in the Sep 86 Scientific American
 *  - added arguments to select between sqrt(), sin(), log(), and linear
 *    functions (original was sqrt()).
 *
 *  - Rewritten using X11 19 Apr 1993 by Ron Record (rr@sco.com)
 */
/*************************************************************************
 *                                                                       *
 *  Copyright (c) 1987-1993 Ronald Joe Record                            *
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

#include "hop.h"

#define MAX_A 100.0
#define MAX_B 100.0
#define MAX_C 100.0
#define MAX_S 4
#define MIN_S 1
#define MAX_P 4000
#define MIN_P 100
#define MAX_MISS 75
#define NUM_MODES 6
#define MAX_H_IND 5
#define M_A 0.2
#define M_B 0.8

int screenx, screeny, xcenter, ycenter, period, num, move;
int hop_hue, width, height, numfreecols, maxcolor, next;
int demo=0, histogram=0, domiss=0, sqflag=1, eflag=0, sinflag=0, Oflag=0;
int bothflag=0, logflag=0, lflag=0, change=0, stop=0, stopping=1, usedefault=0;
int nummaps=1, delay=0, useroot=0, full=0, oflag=0, h_ind=0;
int minhist, maxhist, diff;
int **xy;
long limit = 0;
double scale, a, b, c, d, e, i_inc, j_inc; 
double exponent = 0.5;
char *outname;

/* routines declared in this file */
void event_loop(), usage(), init_contexts(), rehist(), hopalong(), root_exit();
void Clear(), print_help(), set_max_min(), freemem(), setupmem(), initialize();
void redisplay(), resize(), save(), parseargs(), Getkey(), setparams();
int get_hist_index();
/* external routines used in this file */
extern long lrand48();
extern void FlushBuffer(), BufferPoint(), InitBuffer();

void
freemem()
{
    static int i;

    for (i=0;i<=width;i++)
        free(xy[i]);
    free(xy);
}

zeromem()
{
	static int i, j;

	diff = maxhist = 1; minhist = MAXINT;
	for (i=0; i<width+1; i++)
		for (j=0; j<height+1; j++)
			xy[i][j] = 0;
}

void
setupmem()
{
    static int i, j;

    if ((xy=(int **)malloc((width+1)*sizeof(int *))) == (int **)NULL) {
        printf("Error malloc'ing xy.\n");
        exit (-1);
    }
    for (i=0;i<width+1;i++) {
        if ((xy[i]=(int *)malloc((height+1)*sizeof(int)))==(int *)NULL){
            printf("Error malloc'ing xy[%d].\n", i);
            exit (-1);
        }
    }
	zeromem();
}

void
set_max_min(n)
int n;
{
	if (n > maxhist)
		maxhist = n;
	if (n < minhist)
		minhist = n;
	diff = maxhist - minhist;
}

void
hopalong()
{
	int miss;
	double inext, jnext, dtmp;
	extern double pow(), fabs(), sqrt(), sin(), log();

	miss = 0;
	screenx = xcenter + i_inc * scale;
	screeny = ycenter + j_inc * scale;

	if ((screenx < width) && (screeny <= height) && 
		(screenx >= 0) && (screeny >= 0)) {
		if (histogram) {
			xy[screenx][screeny]++;
			set_max_min(xy[screenx][screeny]);
			hop_hue = get_hist_index(xy[screenx][screeny]) + STARTCOLOR;
		}
		BufferPoint(dpy,canvas,pixmap,Data_GC,&Points,hop_hue,screenx,screeny);
		miss=0;
	}
	else
		if (++miss > MAX_MISS)
			if (domiss)
				return;
	if (sqflag)
		dtmp = sqrt(fabs(b*i_inc-c));
	else if (eflag)
		dtmp = pow(fabs(b*i_inc-c), exponent);
	else if (sinflag) {
		dtmp = fabs(b*i_inc-c);
		dtmp = (dtmp / 100.0) * sin(dtmp);
	}
	else if (bothflag) {
		dtmp = fabs(b*i_inc-c);
		dtmp = sqrt(dtmp) * sin(dtmp);
	}
	else if (logflag) {
		dtmp = fabs(b*i_inc-c) + 1.0;
		dtmp = log(dtmp);
	}
	else if (lflag)
		dtmp = fabs(b*i_inc-c) / 50.0;
	inext = d + j_inc - (i_inc < 0 ? -1 : 1) * dtmp;
	jnext = a - i_inc + (j_inc < 0 ? -1 : 1) * (e*j_inc);

	i_inc = inext;
	j_inc = jnext;
}

void
setparams()
{
	extern double drand48();

	scale = MIN_S + (lrand48() % MAX_S);
	period = MIN_P + (lrand48() % MAX_P);
	if (change) {
		switch (rand() % NUM_MODES) {
		case '0':	sqflag = 1;
			eflag = lflag = sinflag = logflag = bothflag = 0;
			break;
		case '1':	eflag=1; exponent=drand48();
			sqflag = lflag = sinflag = logflag = bothflag = 0;
			break;
		case '2':	lflag = 1;
			eflag = sqflag = sinflag = logflag = bothflag = 0;
			break;
		case '3':	sinflag = 1;
			eflag = sqflag = lflag = logflag = bothflag = 0;
			break;
		case '4':	logflag = 1;
			eflag = sqflag = sinflag = lflag = bothflag = 0;
			break;
		case '5':	bothflag = 1;
			eflag = sqflag = sinflag = lflag = logflag = 0;
			break;
		case '?':
			usage();
			break;
		}
	}
	if (sqflag || eflag) {
		a = drand48() * MAX_A;
		b = drand48() * MAX_B;
		c = drand48() * MAX_C;
	}
	else if (sinflag) {
		a = drand48() * (MAX_A / 5);
		b = drand48() * (MAX_B / 10);
		c = drand48() * (MAX_C / 5);
	}
	else if (bothflag) {
		a = drand48() * (MAX_A / 10);
		b = drand48() * (MAX_B / 10);
		c = drand48() * (MAX_C / 10);
	}
	else if (logflag || lflag) {
		a = drand48() * MAX_A;
		b = MAX_A - a;
		c = drand48() * MAX_C;
	}
}

void
parseargs(ac, av)
int ac;
char *av[];
{
	register int c;
	int aset=1, bset=1, cset=1, pset=1, sset=1;
	extern int optind;
	extern char *optarg;
	extern double atof();
	extern long atol();

    outname = "hop.ppm";
    width = 0; height = 0;
	while ((c = getopt(ac, av, "AFHLORSZmuC:D:E:a:b:c:d:e:h:l:o:p:s:w:"))!=EOF){
		switch (c) {
		case 'A':	lflag=0; eflag=0; sqflag=0; sinflag=0; bothflag=0;
				logflag=1;
				if (aset)
					a=10; 
				if (bset)
					b=5; 
				if (cset)
					c=15;
				if (pset)
					period=1000;
				break;
        case 'C':
            numwheels = atoi(optarg);
            if (numwheels > MAXWHEELS)
                numwheels = MAXWHEELS;
            if (numwheels < 0)
                numwheels = 0;
            break;
		case 'D':	demo++; domiss=1; 	
				nummaps=atoi(optarg);
				break;
		case 'E':	exponent=atof(optarg);	
				eflag=1; sqflag=0;
				break;
		case 'F':	stopping = 0;
				break;
		case 'H':	histogram++;
				break;
		case 'L':	logflag=0; eflag=0; sqflag=0; sinflag=0; bothflag=0;
				lflag=1;
				if (aset)
					a=10; 
				if (bset)
					b=5; 
				if (cset)
					c=15;
				if (pset)
					period=1000;
				break;
		case 'O': /* Use the default parameters and map */
				Oflag++;
				break;
		case 'R': useroot++;
				break;
		case 'S':	logflag=0; eflag=0; sqflag=0; lflag=0; bothflag=0;
				sinflag=1;
				if (aset)
					a=20; 
				if (bset)
					b=1; 
				if (cset)
					c=5;
				if (sset)
					scale=2.0;
				if (pset)
					period=1000;
				break;
		case 'Z':	logflag=0; eflag=0; sqflag=0; lflag=0; sinflag=0;
				bothflag=1;
				if (aset)
					a=3; 
				if (bset)
					b=2; 
				if (cset)
					c=1;
				if (sset)
					scale=0.1;
				if (pset)
					period=1000;
				break;
		case 'a':	a=atof(optarg); aset=0;	
				break;
		case 'b':	b=atof(optarg);	bset=0; 
				break;
		case 'c':	c=atof(optarg);	cset=0; 
				break;
		case 'd':	d=atof(optarg);	
				break;
		case 'e':	e=atof(optarg);	
				break;
        case 'h':
            height = atoi(optarg);
            break;
		case 'l':	limit=atol(optarg);	
				break;
		case 'm':	domiss=1;		
				break;
		case 'o':	outname = optarg;		
				break;
		case 'p':	period=atoi(optarg);	
				pset=0;
				break;
		case 's':	scale=atof(optarg);
				sset=0; 
				break;
        case 'w':
            width = atoi(optarg);
            break;
		case 'u':
		case '?':	usage();		
				break;
		}
	}
	if (sqflag && demo)
		change=1;
}

void
usage()
{
    fprintf(stderr,"usage: hop [-ALSZDmu][-E#][-a#][-b#][-c#][-d#][-e#][-l#][-p#][-s#]\n");
    fprintf(stderr,"[-w width][-h height]\n");
    fprintf(stderr, "\tWhere a, b, c, d and e are the dynamical parameters\n");
    fprintf(stderr,"\tl is the limit, p the period, and s the scale\n");
    fprintf(stderr,"\tThe algorithm in hop is an iterated function system :\n");
    fprintf(stderr,"\n\t\tinext = d + j - (i<0 ? -1:1) * sqrt(fabs(b*c-i))\n");
    fprintf(stderr,"\t\tjnext = a - (j<0 ? -1:1) * (e*i)\n\n");
    fprintf(stderr, "\t-D indicates run in demo mode\n");
    fprintf(stderr, "\t-E# uses pow() rather than sqrt() (# must be < 0.9)\n");
    fprintf(stderr, "\t-m turns on missing mode (exits after 50 misses)\n");
    fprintf(stderr, "\t-A uses log() rather than sqrt()\n");
    fprintf(stderr, "\t-L uses a linear map rather than sqrt()\n");
    fprintf(stderr, "\t-S uses a sin() map rather than sqrt()\n");
    fprintf(stderr, "\t-Z uses both sin() and sqrt()\n");
    fprintf(stderr, "\t-u produces this message\n");
    fprintf(stderr, "\tDefaults are a=80 b=40 c=60 d=0 e=0 E=0.5 p=1024 s=4\n");
    fprintf(stderr, "\t-A defaults are a=10 b=5 c=15 p=1000 s=4\n");
    fprintf(stderr, "\t-L defaults are a=10 b=5 c=15 p=1000 s=4\n");
    fprintf(stderr, "\t-S defaults are a=20 b=1 c=5 p=1000 s=2\n");
    fprintf(stderr, "\t-Z defaults are a=3 b=2 c=1 p=1000 s=0.1\n");
    exit(1);
}

void
init_contexts()
{
    static int i;

    /*
     * create default, writable, graphics contexts for the canvas.
     */
    Data_GC[0] = XCreateGC(dpy, DefaultRootWindow(dpy),
        (unsigned long) NULL, (XGCValues *) NULL);
    /* set the background to black */
    XSetBackground(dpy,Data_GC[0],BlackPixel(dpy, screen));
    /* set the foreground of the 0th context to black */
    XSetForeground(dpy, Data_GC[0], BlackPixel(dpy, screen));
    Data_GC[1] = XCreateGC(dpy, DefaultRootWindow(dpy),
        (unsigned long) NULL, (XGCValues *) NULL);
    /* set the background to black */
    XSetBackground(dpy,Data_GC[1],BlackPixel(dpy, screen));
    /* set the foreground of the 1st context to white */
    XSetForeground(dpy, Data_GC[1], WhitePixel(dpy,  screen));
    for (i=2; i<maxcolor; i++) {
        Data_GC[i] = XCreateGC(dpy, DefaultRootWindow(dpy),
            (unsigned long) NULL, (XGCValues *) NULL);
        /* set the background to black */
        XSetBackground(dpy,Data_GC[i],BlackPixel(dpy, screen));
        /* set the foreground of the ith context to i */
        XSetForeground(dpy, Data_GC[i], i);
    }
}

void
Clear()
{
    XFillRectangle(dpy, pixmap, Data_GC[0], 0, 0, width, height);
    XCopyArea(dpy, pixmap, canvas, Data_GC[0], 0, 0, width, height, 0, 0);
}

void
initialize()
{
    InitBuffer(&Points, maxcolor);
	i_inc = j_inc = 0.0;
	if (Oflag)
		Oflag=0;
	else
		setparams();
    Clear();
	num = 1;
}

#define x_str 10

void
print_help() 
{
    static char str[80];
    static int y_str, spacing;
    static int ascent, descent, dir;
    static XCharStruct overall;
    static GC gc;

    gc = Data_GC[1];
    XClearWindow(dpy, help);
    y_str = 60;
    sprintf(str,"During run-time, interactive control can be exerted via : ");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    XQueryTextExtents(dpy,(XID)XGContextFromGC(gc),"Hey!",
			4,&dir,&ascent,&descent,&overall);
    spacing = ascent + descent + 5;
    y_str += 2 * spacing;
    sprintf(str,"        - lowers the value of mincolindex, + raises it");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        f or F saves hop to a PPM file");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        h or H or ? displays this message");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        n goes on to the next hop");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        N creates a new replacement hop");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        r or s spins the colorwheel");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        w decrements, W increments the color wheel index");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        q or Q exits");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += 2*spacing;
    sprintf(str,"Press 'h', 'H', or '?' to unmap the help window");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
}

void
redisplay (event)
XExposeEvent    *event;
{
    if ((event->window == help) && (!useroot))
        print_help();
    else {
        /*
        * Extract the exposed area from the event and copy
        * from the saved pixmap to the window.
        */
        XCopyArea(dpy, pixmap, canvas, Data_GC[0], event->x, event->y, 
            event->width, event->height, event->x, event->y);
    }
}

void
resize()
{
    Window r;
    int j; 
    int x, y;
    unsigned int bw, d, new_w, new_h;

    XGetGeometry(dpy,canvas,&r,&x,&y,&new_w,&new_h,&bw,&d);
    if (((int)new_w == width) && ((int)new_h == height)) {
        return;
    }
	if (histogram)
		freemem();
    width = (int)new_w; height = (int)new_h;
    if (pixmap)
        XFreePixmap(dpy, pixmap);
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), 
            width, height, DefaultDepth(dpy, screen));
	if (histogram)
		setupmem();
	next = 1;
	stop = 0;
	nummaps++;
	initialize();
}

Cleanup() {
    if (histogram)
        freemem();
    XCloseDisplay(dpy);
}

/* Store hop growth in PPM format */
void
save()
{
    FILE *outfile;
    unsigned char c;
    XImage *ximage;
    static int i,j;
    struct Colormap {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };
    struct Colormap *colormap=NULL;

    if ((colormap=
        (struct Colormap *)malloc(sizeof(struct Colormap)*maxcolor))
            == NULL) {
        fprintf(stderr,"Error malloc'ing colormap array\n");
        Cleanup();
        exit(-1);
    }
    outfile = fopen(outname,"w");
    if(!outfile) {
        perror(outname);
        Cleanup();
        exit(-1);
    }

    ximage=XGetImage(dpy, pixmap, 0, 0, width, height, AllPlanes, ZPixmap);

    for (i=0;i<maxcolor;i++) {
        colormap[i].red=(unsigned char)(Colors[i].red >> 8);
        colormap[i].green=(unsigned char)(Colors[i].green >> 8);
        colormap[i].blue =(unsigned char)(Colors[i].blue >> 8);
    }
    fprintf(outfile,"P%d %d %d\n",6,width,height);
    fprintf(outfile,"%d\n",maxcolor-1);

    for (j=0;j<height;j++)
        for (i=0;i<width;i++) {
            c = (unsigned char)XGetPixel(ximage,i,j);
            fwrite((char *)&colormap[c],sizeof colormap[0],1,outfile);
        }
    fclose(outfile);
    free(colormap);
}

int
get_hist_index(n)
int n;
{
	static double p;
	extern double asin(), pow();

	if (h_ind == 0)
		return(n % numfreecols);
	else if (h_ind == 1)
		return((int)(pow((double)(numfreecols-1),
				1.0-((double)(n-minhist)/(double)diff))));
	else if (h_ind == 2) { 
		/* two lines (0,0)-(M_A,m*M_B) and (A,m*M_B)-(1,m) */
		p = (double)(n - minhist) / (double)diff;
		if (p < M_A)
			return((int)((numfreecols-1)*M_B*p/M_A));
		else
			return((int)((numfreecols-1)*(((1.0-M_B)*(p-1.0))+1.0)));
	}
	else if (h_ind == 3) {
		p = (double)(n - minhist) / (double)diff;
		return((int)((0.5 + (asin((p*2.0) - 1.0)/M_PI))*(numfreecols-1)));
	}
	else if (h_ind == 4)
		return(n*(numfreecols-1)/maxhist);
	else
		return((n-minhist)*(numfreecols-1)/diff);
}

void
rehist()
{
	static int i, j;

	Clear();
	minhist = MAXINT; maxhist = 1;
	for (i=0; i<width; i++)
		for (j=0; j<height; j++) {
			if (xy[i][j])
				set_max_min(xy[i][j]);
		}
	if (maxhist == minhist)
		diff = 1;
	else
		diff = maxhist - minhist;
	if (diff < 0)
		diff = 1;
	FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
	for (i=0; i<width; i++)
		for (j=0; j<height; j++) {
			if (xy[i][j])
				BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, 
							get_hist_index(xy[i][j])+STARTCOLOR,i,j);
		}
	FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
}

void
Getkey(event)
XKeyEvent *event;
{
    char key;
	static int spinning=0, spindir=0;
    static XWindowAttributes attr;
	extern void init_color(), write_cmap();

    if (XLookupString(event, (char *)&key, sizeof(key), (KeySym *)0,
       (XComposeStatus *) 0) > 0)
            switch (key) {
            	case '\015': /*write out current colormap to $HOME/.<prog>map*/
        			write_cmap(dpy,cmap,Colors,maxcolor,"hop","Hop");
					break;
                case '+': mincolindex += INDEXINC;
                    if (mincolindex > maxcolor)
                        mincolindex = 1;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"hop","Hop",0);
                    break;
                case '-': mincolindex -= INDEXINC;
                    if (mincolindex < 1)
                        mincolindex = maxcolor - 1;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"hop","Hop",0);
                    break;
				case 'a': /* use the log function with new parameters */
				case 'A':	lflag=0; eflag=0; sqflag=0; sinflag=0; bothflag=0;
					next = logflag=1;
					nummaps++;
					change=0;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
					break;
				case 'b': /* toggle infinite doily */
				case 'B': stopping = (!stopping);
					break;
				case 'c':
				case 'C': Clear();
    				InitBuffer(&Points, maxcolor);
					num = 1;
					break;
                case 'd': delay -= 25; if (delay < 0) delay = 0; break;
                case 'D': delay += 25; break;
                case 'f':	/* save in PPM format file */
                case 'F': save(); break;
				case 'H': histogram = (!histogram);
					if (histogram)
						setupmem();
					else
						freemem();
					break;
				case 'j': /* randomly change the map */
				case 'J': next = 1;
					nummaps++;
					change=1;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
					break;
				case 'k': /* use the square root function with new parameters */
				case 'K': logflag=0; eflag=0; lflag=0; sinflag=0; bothflag=0;
					next = sqflag=1;
					nummaps++;
					change=0;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
					break;
				case 'l': /* use the linear function with new parameters */
				case 'L': logflag=0; eflag=0; sqflag=0; sinflag=0; bothflag=0;
					next = lflag=1;
					nummaps++;
					change=0;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
					break;
				case 'm': /* use the sin function with new parameters */
				case 'M':	logflag=0; eflag=0; sqflag=0; lflag=0; bothflag=0;
					next = sinflag=1;
					nummaps++;
					change=0;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
					break;
                case 'n':	/* go on to the next hop, same function */
                    next = 1;
					change = 0;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
                    break;
                case 'N':	/* go on to the next hop, different function */
                    next = change = 1;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
                    break;
				case 'r': h_ind--;
					if (h_ind < 0)
						h_ind = MAX_H_IND;
					if (histogram) rehist();
					break;
				case 'R': h_ind++;
					if (h_ind > MAX_H_IND)
						h_ind = 0;
					if (histogram) rehist();
					break;
				case 'S': spinning=0;
                    break;
                case 's': spinning=1; spindir=(!spindir);
					Spin(dpy,cmap,Colors,STARTCOLOR,maxcolor,delay,spindir);
                    break;
            	case '\027': /* (ctrl-W) read palette from $HOME/.hopmap */
                  numwheels = 0;
				  if (!usedefault)
                      init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"hop","Hop",0);
                  break;
                case 'W': 
                    if (numwheels < MAXWHEELS)
                        numwheels++;
                    else
                        numwheels = 0;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"hop","Hop",0);
                    break;
                case 'w': 
                    if (numwheels > 0)
                        numwheels--;
                    else
                        numwheels = MAXWHEELS;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"hop","Hop",0);
                    break;
                case '?':
                case 'h': 
					if (!useroot) {
						XGetWindowAttributes(dpy, help, &attr);
                    	if (attr.map_state != IsUnmapped)
                        	XUnmapWindow(dpy, help);
                    	else {
                        	XMapRaised(dpy, help);
                        	print_help();
                    	}
					}
                    break;
				case 'X':	/* create new hops, erasing the old */
						next = stop = 1;
						nummaps++;
						FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
										0, maxcolor);
						initialize();
						if (histogram) {
							freemem();
							setupmem();
						}
						break;
				case 'y': /* use known ok parameters with the sqrt function */
				case 'Y':	logflag=0; eflag=0; bothflag=0; lflag=0; sinflag=0;
					next = sqflag=1;
					nummaps++;
					scale = 1.0;
					period = 1024;
					a = 80.0; b = 40.0; c = 60.0; d = 0.0; e = 0.0;
					i_inc = j_inc = 0.0;
    				InitBuffer(&Points, maxcolor);
					if (histogram) {
						freemem();
						setupmem();
					}
					Clear();
					num = 1;
					break;
				case 'z': /* use sqrt & sin functions with new parameters */
				case 'Z':	logflag=0; eflag=0; sqflag=0; lflag=0; sinflag=0;
					next = bothflag=1;
					nummaps++;
					change=0;
					if (histogram) {
						freemem();
						setupmem();
					}
					initialize();
					break;
                case 'Q':
                case 'q': Cleanup(); exit(0); break;
            }
			if (spinning)
				Spin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, spindir);
}

void
event_loop()
{
    int n;
    XEvent event;

    n = XEventsQueued(dpy, QueuedAfterFlush);
    while (n-- > 0) {
        XNextEvent(dpy, &event);
        switch(event.type) {
            case KeyPress:
                Getkey(&event);
                break;
            case Expose:
                redisplay(&event);
                break;
            case ConfigureNotify:
                resize();
                break;
        }
    }
}

void
root_exit() 
{
	static int i;

	if (histogram) {
		h_ind = 2;
		rehist();
	}
	XSetWindowBackgroundPixmap(dpy, canvas, pixmap);
	for (i=0; i<maxcolor; i++)
		XFreeGC(dpy, Data_GC[i]);
	XFreePixmap(dpy, pixmap);
	XClearWindow(dpy, canvas);
	XFlush(dpy);
        Cleanup();
	exit(0);
}

main(argc,argv)
int argc;
char *argv[];
{
    static int i, j;
	static int count;
    XSizeHints hint;
    Atom __SWM_VROOT = None;
    Window rootReturn, parentReturn, *children;
	XVisualInfo *visual_list, visual_template;
    unsigned int numChildren;
    extern void srand48(), init_color();
    
	i_inc = j_inc = 0.0;
	scale = 1.0;
	period = 1024;
	a = 80.0; b = 40.0; c = 60.0; d = 0.0; e = 0.0;
    parseargs(argc,argv);
    srand48((long)time(0));
    dpy = XOpenDisplay("");
    screen = DefaultScreen(dpy);
	if (useroot) {
		nummaps = 1;
		signal(SIGINT, root_exit);
	}
    if (full || useroot) {
        width = XDisplayWidth(dpy, screen);
        height = XDisplayHeight(dpy, screen);
    }
	if (width == 0)
        width = XDisplayWidth(dpy, screen);
	if (height == 0)
        height = XDisplayHeight(dpy, screen);
	xcenter = width / 2;
	ycenter = height / 2;
	if (limit == 0)
		limit = width * height;
	if (histogram)
		setupmem();
    maxcolor  = (int)XDisplayCells(dpy, screen);
	if (maxcolor <= 16) {
		STARTCOLOR = 2; delay = 100;
		INDEXINC = 1; mincolindex = 5;
	}
	maxcolor = Min(maxcolor, MAXCOLOR);
	numfreecols = maxcolor - STARTCOLOR;
	hop_hue = STARTCOLOR;
    /*
    * Create the pixmap to hold the hop growth
    */
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), width, height, 
                           DefaultDepth(dpy, screen));
    /*
    * Create the window to display the hop pattern
    */
    hint.x = 0;
    hint.y = 0;
    hint.width = width;
    hint.height = height;
    hint.flags = PPosition | PSize;
    if (useroot) {
        canvas = DefaultRootWindow(dpy);
        /* search for virtual root (from ssetroot by Tom LaStrange) */
        __SWM_VROOT = XInternAtom(dpy, "__SWM_VROOT", False);
        XQueryTree(dpy,canvas,&rootReturn,&parentReturn,&children,&numChildren);
        for (j = 0; j < numChildren; j++) {
            Atom actual_type;
            int actual_format;
            long nitems, bytesafter;
            Window *newRoot = NULL;

            if (XGetWindowProperty (dpy, children[j], __SWM_VROOT,0,1, False, 
                XA_WINDOW, &actual_type, &actual_format, &nitems, &bytesafter,
                (unsigned char **) &newRoot) == Success && newRoot) {
                canvas = *newRoot;
                break;
            }
        }
    }
    else {
        canvas = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            0, 0, width, height, 5, 0, 1);
        XSetStandardProperties(dpy, canvas, "Hop by Ron Record",
            "Hop", None, argv, argc, &hint);
        XMapRaised(dpy, canvas);
    }
    XChangeProperty(dpy, canvas, XA_WM_CLASS, XA_STRING, 8, PropModeReplace, 
					"hop", strlen("hop"));
    /*
    * Create the window used to display the help info (if not running on root)
    */
	if (!useroot) {
    	hint.x = XDisplayWidth(dpy, screen) / 4;
    	hint.y = XDisplayHeight(dpy, screen) / 4;
    	hint.width = hint.x * 2;
    	hint.height = hint.y * 2;
    	help = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            	hint.x, hint.y, hint.width, hint.height, 5, 0, 1);
		XSetWindowBackground(dpy, help, BlackPixel(dpy, screen));
    	/* Title */
    	XSetStandardProperties(dpy,help,"Help","Help",None,argv,argc,&hint);
    	/* Try to write into a new color map */
		visual_template.class = PseudoColor;
		visual_list = XGetVisualInfo(dpy,VisualClassMask,&visual_template,&i);
		if (! visual_list)
			usedefault = 1;
		if (usedefault)
		  cmap = DefaultColormap(dpy, screen);
		else {
    	  cmap = XCreateColormap(dpy,canvas,DefaultVisual(dpy,screen),AllocAll);
    	  init_color(dpy, canvas, cmap, Colors, STARTCOLOR, mincolindex, maxcolor,
				numwheels,"hop", "Hop", 0);
    	/* install new color map */
          XSetWindowColormap(dpy, canvas, cmap);
          XSetWindowColormap(dpy, help, cmap);
	    }
	}
    init_contexts();
	if (useroot)
    	XSelectInput(dpy,canvas,ExposureMask);
	else {
    	XSelectInput(dpy,canvas,KeyPressMask|ExposureMask|StructureNotifyMask);
    	XSelectInput(dpy,help,KeyPressMask|ExposureMask);
	}
    for (i=0; i!=nummaps; i++) {
        next = 0;
		if (!stop)	/* true 1st time thru */
			initialize();
		stop=0;
		if (!histogram)
			FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,hop_hue,hop_hue+1);
		for (;;) {
			event_loop();
			hopalong();
			if (stop)
			  break;
			if ((num % period) == 0) {
				if (histogram)
			  		FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,0,maxcolor);
				else {
			  		FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,
							hop_hue,hop_hue+1);
			  		hop_hue++; 
			  		if (hop_hue >= maxcolor)
						hop_hue = STARTCOLOR;
				}
			}
			if ((++num > limit) && stopping)
				stop = 1;
		}
        FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,0,maxcolor);
        if (oflag)
            save();
        if (demo) {
            event_loop();
			DemoSpin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, 4);
            event_loop();
			for (j=0; j<=MAXWHEELS; j++) {
				if (!usedefault)
				    init_color(dpy, canvas, cmap, Colors, STARTCOLOR, 
						mincolindex, maxcolor, j, "hop", "Hop", 0);
				event_loop();
				sleep(1);
			}
        }
		else if (useroot)
			root_exit();
        else {
            XSync(dpy, True);
			Spin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, 0);
            for (;;) {
                event_loop();
                if (next) break;
            }
			if (histogram) {
				freemem();
				setupmem();
			}
        }
    }
    Cleanup();
    exit(0);
}
