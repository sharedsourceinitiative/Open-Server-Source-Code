/*
 *    @(#) xtopo.c 1.5 93/11/12 MRINC
 *
 *    Written using CGI 10 Jun 1987 by Ron Record (rr@sco.com)
 *    Rewritten using X11 21 Oct 1992 by Ron Record (rr@sco.com)
 *
 *  xtopo calculates and displays a fractal artificial landscape.
 *  The landscape is displayed topographically with colors representing height.
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


#include "xtopo.h"

int first_row, last_row;
int nrows=1024, nummaps=1, ranbeta=0, delay=0, xpos=-1, ypos=-1, usedefault=0;
int demo=0, useroot=0, full=0, oflag=0, spin=0, vflag=0, origin[2];
int maxcolor, colors, width, height, offset, next, sea_pixel;
double beta, min, max, rmax=1.0, rmin=0.0;
double sea_level=0.35;
double **xy;
char *outname;

extern double drand48();
extern void BufferPoint(), InitBuffer(), FlushBuffer();

void
usage()
{
    printf("Usage: xtopo [-o file][-dsuFRTV][-S level][-H beta]");
    printf("[-m #][-w width][-h height]\n");
    printf("\nWhere :\n\t0 < beta < 1 (default is 0.5)\n");
    printf("\t0 < level < 1 (default is 0.35)\n");
    printf("\t-d indicates demo mode\n");
    printf("\t-s indicates spin color wheel when done computing\n");
    printf("\t-T indicates use Thin array (512x512)\n");
    printf("\t-R indicates use the root window\n");
    printf("\t-V indicates use Very Thin array (256x256)\n");
    printf("\t-m # indicates a minimum color index of # (0-255)\n");
    printf("\t-o file will save the output as 'file' in PPM format\n");
    printf("\t-w # indicates a window width of #\n");
    printf("\t-h # indicates a window height of #\n");
    printf("\t-u produces this message\n");
    printf("During display :\n");
    printf("\t'f' or 'F' will save the picture as a PPM file\n");
    printf("\t'+' will increment and '-' decrement the minimum color index\n");
    printf("\t'>' will increment and '<' decrement the sea level\n");
    printf("\t'0' - '9' will set the sea level to 0.0 - 0.9\n");
    printf("\t'r' or 's' will spin the color wheel forwards or backwards\n");
    printf("\t'W' will increment and 'w' decrement the color map selection\n");
    printf("\t'?' or 'h' will display the usage message\n");
    printf("\t'q' or 'Q' will quit\n");
}

void
grafline(line, off, size, row)
double *line[];
int off;
int size;
int row;
{
    static int i, cell;
    double peak, breadth, trans;

    breadth = max - min;
    peak = 1.0 - sea_level;

    for (i=0; i<size; i++) {
       trans = (line[row][i+off] - min)/breadth;
       if (trans < sea_level)
         cell = sea_pixel;
       else
         cell=(int)(((trans-sea_level)/peak)*((double)colors-1.0))+STARTCOLOR;
       BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, cell, 
					origin[0] + i, origin[1]);
    }
    origin[1]++;
    if (origin[1] >= XDisplayHeight(dpy, screen))
        origin[1] = 0;
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
    /* set the foreground of the 0th context to white */
    XSetForeground(dpy, Data_GC[0], WhitePixel(dpy, screen));
    Data_GC[1] = XCreateGC(dpy, DefaultRootWindow(dpy),
        (unsigned long) NULL, (XGCValues *) NULL);
    /* set the background to white */
    XSetBackground(dpy,Data_GC[1],WhitePixel(dpy, screen));
    /* set the foreground of the 1st context to black */
    XSetForeground(dpy, Data_GC[1], BlackPixel(dpy,  screen));
    for (i=2; i<maxcolor; i++) {
        Data_GC[i] = XCreateGC(dpy, DefaultRootWindow(dpy),
            (unsigned long) NULL, (XGCValues *) NULL);
        /* set the background to black */
        XSetBackground(dpy,Data_GC[i],BlackPixel(dpy, screen));
        /* set the foreground of the ith context to i */
        XSetForeground(dpy, Data_GC[i], i);
    }
	sea_pixel = BlackPixel(dpy, screen);
}

void
Clear()
{
    XFillRectangle(dpy, pixmap, Data_GC[0], 0, 0, width, height);
    XCopyArea(dpy, pixmap, canvas, Data_GC[0], 0, 0, width, height, 0, 0);
}

void
freemem()
{
    static int i;

    for (i=0;i<=nrows;i++)
        free(xy[i]);
    free(xy);
}

void
setupmem()
{
    static int i;

    if ((xy=(double **)malloc((nrows+1)*sizeof(double *))) == (double **)NULL) {
        printf("Error malloc'ing xy.\n");
        exit (-1);
    }
    for (i=0;i<=nrows;i++) {
        if ((xy[i]=(double *)malloc((nrows+1)*sizeof(double)))==(double *)NULL){
            printf("Error malloc'ing xy[%d].\n", i);
            exit (-1);
        }
    }
}

double
gauss()
{
    return(2.0 * drand48()) - 1.0;
}
    
double
pgauss()
{
    double r;

    r = gauss();
    if (min > r) min = r;
    if (max < r) max = r;
    return(r);
}
    
void
init_pts(line,size)
double *line[];
int size;
{
    int mid;

    mid=size/2;

    line[0][size]=pgauss(); line[0][mid]=pgauss(); line[0][0]=pgauss();
    line[size][size]=pgauss(); line[size][mid]=pgauss();
    line[size][0]=pgauss(); line[mid][size]=pgauss(); 
    line[mid][mid]=pgauss(); line[mid][0]=pgauss();
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

    gc = Data_GC[0];
    XClearWindow(dpy, help);
    y_str = 40;
    sprintf(str,"During run-time, interactive control can be exerted via : ");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    XQueryTextExtents(dpy,(XID)XGContextFromGC(gc),"Hey!",
			4,&dir,&ascent,&descent,&overall);
    spacing = ascent + descent + 5;
    y_str += 2 * spacing;
    sprintf(str,"        < lowers the sea level, > raises it");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        0 - 9 sets the sea level to 0.0 - 0.9");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        { lowers the value of beta, } raises it");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        - lowers the value of mincolindex, + raises it");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        d decreases the delay, D increases it");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        f or F saves landscape to a PPM file");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        h or H or ? displays this message");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        n goes on to the next landscape");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        N creates a new replacement landscape");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        r or s spins the colorwheel");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        R or S stops the colorwheel spin");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        v or V toggles the sea between black and white");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        w decrements, W increments the color wheel index");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        <ctrl>-W reads the colormap in $HOME/.xtopomap");
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
    if (event->window == help)
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
    extern void Clear(), midpoint(), event_loop();

    freemem();
    XGetGeometry(dpy,canvas,&r,&x,&y,&new_w,&new_h,&bw,&d);
    if (((int)new_w == width) && ((int)new_h == height)) {
        setupmem();
        return;
    }
    width = (int)new_w; height = (int)new_h;
    if (pixmap)
        XFreePixmap(dpy, pixmap);
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), 
            width, height, DefaultDepth(dpy, screen));
    while (Max(width, height) > nrows)
        nrows *= 2;
    first_row = (nrows - height) / 2;
    last_row = nrows - first_row;
    setupmem();
    origin[0] = origin[1] = 0;
    InitBuffer(&Points, maxcolor);
    Clear();
    init_pts(xy,nrows);
    midpoint(xy,nrows,beta);
    for (j=first_row; j < last_row; j++) {
        event_loop();
        grafline(xy,offset,width,j);
    }
    FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
}

void
Cleanup() {
	freemem();
	XCloseDisplay(dpy);
}

/* Store artificial landscape in PPM format */
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
    fprintf(outfile,"# sea_level=%f nrows=%d beta=%f\n",sea_level,nrows,beta);
    fprintf(outfile,"%d\n",maxcolor-1);

    for (j=0;j<height;j++)
        for (i=0;i<width;i++) {
            c = (unsigned char)XGetPixel(ximage,i,j);
            fwrite((char *)&colormap[c],sizeof colormap[0],1,outfile);
        }
    fclose(outfile);
    free(colormap);
}

void
redraw() {		/* just redraw the points under sea level */
    static int i, j;
	static double trans;

    for (j=first_row; j < last_row; j++) {
		for (i=0; i<width; i++) {
			trans = (xy[j][i+offset] - min)/(max - min);
			if (trans < sea_level)
				BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, sea_pixel, 
							i, j - first_row);
		}
    }
    FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
}

void
Redraw() {
    static int j;
    extern void event_loop();

    origin[1] = origin[0] = 0;
    for (j=first_row; j < last_row; j++) {
        event_loop();
        grafline(xy,offset,width,j);
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
        			write_cmap(dpy,cmap,Colors,maxcolor,"xtopo","Xtopo");
					break;
                case '+': mincolindex += INDEXINC;
                    if (mincolindex > maxcolor)
                        mincolindex = 1;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"xtopo","Xtopo",0);
                    break;
                case '-': mincolindex -= INDEXINC;
                    if (mincolindex < 1)
                        mincolindex = maxcolor - 1;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"xtopo","Xtopo",0);
                    break;
                case '}':	/* increment beta by 0.1 */
                    beta += 0.1;
                    break;
                case '{':	/* decrement beta by 0.1 */
                    beta -= 0.1;
                    break;
                case '>':
                    sea_level += 0.1;
                    if (sea_level > 1.0) {
                        sea_level -= 0.1;
                        sea_level = (sea_level + 1.0) / 2.0;
                    }
                    Redraw();
                    break;
                case '<':
                    sea_level -= 0.1;
                    if (sea_level < 0.0) {
                        sea_level += 0.1;
                        sea_level = sea_level / 2.0;
                    }
                    Redraw();
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    sea_level = (double)atoi((char *)&key) / 10.0;
                    Redraw();
                    break;
                case 'd': delay -= 25; if (delay < 0) delay = 0; break;
                case 'D': delay += 25; break;
                case 'f':	/* save in PPM format file */
                case 'F': save(); break;
                case 'n':	/* go on to the next landscape */
                    next = 1;
                    break;
                case 'N':	/* go on to the next landscape */
                    nummaps++;	/* but don't increment the landscape counter */
                    next = 1;
                    break;
				case 'S':
                case 'R': spinning=0;
                    break;
                case 'r': spinning=1; spindir=1; 
					Spin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, 1);
                    break;
                case 's': spinning=1; spindir=0;
					Spin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, 0);
                    break;
            	case '\027': /* (ctrl-W) read palette from $HOME/.xtopomap */
                  numwheels = 0;
				  if (!usedefault)
                      init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"xtopo","Xtopo",0);
                  break;
				case 'v':
				case 'V': sea_pixel = (!sea_pixel); redraw(); break;
                case 'W': 
                    if (numwheels < MAXWHEELS)
                        numwheels++;
                    else
                        numwheels = 0;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"xtopo","Xtopo",0);
                    break;
                case 'w': 
                    if (numwheels > 0)
                        numwheels--;
                    else
                        numwheels = MAXWHEELS;
					if (!usedefault)
                        init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"xtopo","Xtopo",0);
                    break;
                case '?':
                case 'h': XGetWindowAttributes(dpy, help, &attr);
                    if (attr.map_state != IsUnmapped)
                        XUnmapWindow(dpy, help);
                    else {
                        XMapRaised(dpy, help);
                        print_help();
                    }
                    break;
                case 'Q':
                case 'q': Cleanup(); exit(0); break;
            }
			if (spinning)
				Spin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, spindir);
}

void
parseargs(argc, argv)
int argc;
char *argv[];
{
    int c;
    extern int optind, getopt();
	extern double atof();
    extern char *optarg;

    outname = "xtopo.ppm";
    beta = 0.5;
    width = 512; height = 480;
    while((c = getopt(argc, argv, "drsuFRTVa:b:c:h:m:n:o:w:D:H:S:X:Y:")) != EOF)
    {    switch(c)
        {
        case 'a':
            rmin = atof(optarg);
            break;
        case 'b':
            rmax = atof(optarg);
            break;
        case 'c':
            numwheels = atoi(optarg);
            if (numwheels > MAXWHEELS)
                numwheels = MAXWHEELS;
            if (numwheels < 0)
                numwheels = 0;
            break;
        case 'd':
            demo++;
            break;
        case 'u':
            usage();
            exit(0);
        case 'h':
            height = atoi(optarg);
            break;
        case 'm':
            mincolindex = atoi(optarg);
            break;
        case 'n':
            nummaps = atoi(optarg);
            break;
        case 'o':
            ++oflag;
            outname = optarg;
            break;
        case 'r':
            ranbeta++;
            break;
        case 's':
            ++spin;
            break;
        case 'w':
            width = atoi(optarg);
            break;
        case 'F':
            full++;
            break;
        case 'D':
            delay = atoi(optarg);
            break;
        case 'H':
            beta = atof(optarg);
            break;
        case 'R':
            useroot++;
            break;
        case 'S':
            sea_level = atof(optarg);
            break;
        case 'T':
            nrows = 512;
            break;
        case 'V':
            nrows = 256;
            vflag++;
            break;
        case 'X':    xpos=atoi(optarg); break;
        case 'Y':    ypos=atoi(optarg); break;
        case '?':
            usage();
            exit(1);
            break;
        }
    }
}

void
event_loop()
{
    int n;
    XEvent event;

    n = XEventsQueued(dpy, QueuedAfterFlush);
    while (n--) {
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
midpoint (w, N, H)
double *w[];
int N;
double H;
{
    int i, j, d, D, col, startcol;
    double std, ratio;

    D = N/2; d = D/2;
    ratio = 1.0/pow(2.0,H); std = ratio;
    while (D > 1) {
        /* use diagonal neighborhood */
        for (i=d; i<N; i += D)
            for (j=d; j<N; j += D) {
                w[i][j]=((w[i+d][j+d]+w[i-d][j-d]+w[i+d][j-d]+w[i-d][j+d])
                        *0.25) + (std*gauss());
                if (min > w[i][j]) min = w[i][j];
                if (max < w[i][j]) max = w[i][j];
            }
        event_loop();
        /* use neighbors to N,E,W,S */
        for (i=d; i<N; i += D) {
            w[i][0]=((w[i-d][0]+w[i+d][0]+w[i][d])/3.0)+std*gauss();
            if (min > w[i][0]) min = w[i][0];
            if (max < w[i][0]) max = w[i][0];
            w[i][N]=((w[i-d][N]+w[i+d][N]+w[i][N-d])/3.0) + std*gauss();
            if (min > w[i][N]) min = w[i][N];
            if (max < w[i][N]) max = w[i][N];
            w[0][i]=((w[0][i-d]+w[0][i+d]+w[d][i])/3.0)+std*gauss();
            if (min > w[0][i]) min = w[0][i];
            if (max < w[0][i]) max = w[0][i];
            w[N][i]=((w[N][i-d]+w[N][i+d]+w[N-d][i])/3.0) + std*gauss();
            if (min > w[N][i]) min = w[N][i];
            if (max < w[N][i]) max = w[N][i];
        }
        event_loop();
        col = 0;
        for (i=d; i<N; i += d) {    /* Fill the interior */
            if (col%2)
                startcol = d;
            else
                startcol = D;
            for (j=startcol; j<N; j += D) {
                w[i][j] = ((w[i][j+d]+w[i][j-d]+w[i+d][j]+w[i-d][j])*0.25)
                            +std*gauss();
                if (min > w[i][j]) min = w[i][j];
                if (max < w[i][j]) max = w[i][j];
            }
            col++;
        }
        D /= 2; d = D/2; std *= ratio;
        event_loop();
    }
}

main(argc,argv)
int argc;
char *argv[];
{
    static int i, j, k;
    XSizeHints hint;
    Atom __SWM_VROOT = None;
    Window rootReturn, parentReturn, *children;
	XVisualInfo *visual_list, visual_template;
    unsigned int numChildren;
    extern void srand48(), init_color();
    
    parseargs(argc,argv);
    dpy = XOpenDisplay("");
    screen = DefaultScreen(dpy);
    if (full || useroot) {
        width = XDisplayWidth(dpy, screen);
        height = XDisplayHeight(dpy, screen);
        while (nrows < Max(width, height))
            nrows *= 2;
    }
    if (width > nrows)
        width = nrows;
    if (height > nrows)
        height = nrows;
    first_row = (nrows - height) / 2;
    last_row = nrows - first_row;
    offset = (nrows - width) / 2;
    maxcolor  = (int)XDisplayCells(dpy, screen);
	if (maxcolor <= 16) {
		STARTCOLOR = 2; delay = 100;
		INDEXINC = 1; mincolindex = 5;
	}
	colors = maxcolor - STARTCOLOR;
	maxcolor = Min(maxcolor, MAXCOLOR);
    origin[0] = origin[1] = 0;
    /*
    * Create the pixmap to hold the fractal topographic map
    */
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), width, height, 
                           DefaultDepth(dpy, screen));
    /*
    * Create the window to display the fractal topographic map
    */
	if (xpos == -1)
    	hint.x = (XDisplayWidth(dpy, screen) - width)/2;
	else
    	hint.x = xpos;
	if (ypos == -1)
    	hint.y = (XDisplayHeight(dpy, screen) - height)/2;
	else
    	hint.y = ypos;
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
            hint.x, hint.y, width, height, 5, 0, 1);
        XSetStandardProperties(dpy, canvas, "Xtopo by Ron Record",
            "Xtopo", None, argv, argc, &hint);
        XSelectInput(dpy,canvas,KeyPressMask|ExposureMask);
        XMapRaised(dpy, canvas);
		if (xpos == -1)
			center_horizontal(dpy, canvas, width);
		if (ypos == -1)
			center_vertical(dpy, canvas, height);
    }
    XChangeProperty(dpy, canvas, XA_WM_CLASS, XA_STRING, 8, PropModeReplace, 
					"xtopo", strlen("xtopo"));
    /*
    * Create the window used to display the help info
    */
    hint.x = XDisplayWidth(dpy, screen) / 4;
    hint.y = XDisplayHeight(dpy, screen) / 4;
    hint.width = hint.x * 2;
    hint.height = hint.y * 2;
    help = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            hint.x, hint.y, hint.width, hint.height, 5, 0, 1);
	XSetWindowBackground(dpy, help, BlackPixel(dpy, screen));
    /* Title */
    XSetStandardProperties(dpy, help, "Help", "Help", None, argv, argc, &hint);
    XSelectInput(dpy,help,KeyPressMask|ExposureMask);
    /* Try to write into a new color map */
	visual_template.class = PseudoColor;
	visual_list = XGetVisualInfo(dpy, VisualClassMask, &visual_template, &i);
	if (! visual_list)
		usedefault = 1;
	if (usedefault)
	  cmap = DefaultColormap(dpy, screen);
	else {
      cmap = XCreateColormap(dpy, canvas, DefaultVisual(dpy, screen), AllocAll);
      init_color(dpy, canvas, cmap, Colors, STARTCOLOR, mincolindex, maxcolor,
				numwheels,"xtopo", "Xtopo", 0);
      /* install new color map */
      if (!useroot) {
        XSetWindowColormap(dpy, canvas, cmap);
        XSetWindowColormap(dpy, help, cmap);
	  }
	}
    init_contexts();
    setupmem();
    Clear();
    srand48((long)time(0));
    for (k=0; k!=nummaps; k++) {
        if (ranbeta)
            beta = (drand48() * (rmax - rmin)) + rmin;
        next = 0; min = max = 0.0;
        origin[0] = origin[1] = 0;
        InitBuffer(&Points, maxcolor);
        init_pts(xy,nrows);
        midpoint(xy,nrows,beta);
        if (!useroot)
            XSelectInput(dpy,canvas,KeyPressMask|ExposureMask|
                                    StructureNotifyMask);
        for (j=first_row; j < last_row; j++) {
            event_loop();
            grafline(xy,offset,width,j);
        }
        FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
        if (oflag)
            save();
        if (demo) {
            event_loop();
			DemoSpin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, 4);
            event_loop();
			for (i=0; i<=MAXWHEELS; i++) {
				if (!usedefault)
				    init_color(dpy, canvas, cmap, Colors, STARTCOLOR, 
					    mincolindex, maxcolor, i, "xtopo", "Xtopo", 0);
				sleep(1);
				event_loop();
			}
        }
		else if (useroot) {
        	XSetWindowBackgroundPixmap(dpy, canvas, pixmap);
			XFreePixmap(dpy, pixmap);
			XClearWindow(dpy, canvas);
			XFlush(dpy);
			XCloseDisplay(dpy);
		}
        else {
            XSync(dpy, True);
            if (spin)
				Spin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, 0);
            for (;;) {
                event_loop();
                if (next) break;
            }
        }
    }
	Cleanup();
    exit(0);
}
