/*
 *    @(#) coral.c 1.3 93/11/02 MRINC
 *
 *    Written using CGI 10 Jun 1987 by Ron Record (rr@sco.com)
 *    Rewritten using X11 06 Apr 1993 by Ron Record (rr@sco.com)
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


#include "coral.h"

int col_duration = 0;
int now_x, now_y, max_y;
int width, height;
int nballs = 16;
int coral_hue, count;
int nummaps=1, delay=0;
int demo=0, usedefault=1, useroot=0, full=0, oflag=0, spin=0;
int maxcolor, next;
int **xy;
char *outname;

extern long lrand48();
extern void BufferPoint(), InitBuffer(), FlushBuffer();

void
usage()
{
    printf("Usage: coral [-o file][-dsuFRTV]");
    printf("[-m #][-w width][-h height]\n");
    printf("\t-d indicates demo mode\n");
    printf("\t-s indicates spin color wheel when done computing\n");
    printf("\t-R indicates use the root window\n");
    printf("\t-m # indicates a minimum color index of # (0-255)\n");
    printf("\t-o file will save the output as 'file' in PPM format\n");
    printf("\t-w # indicates a window width of #\n");
    printf("\t-h # indicates a window height of #\n");
    printf("\t-u produces this message\n");
    printf("During display :\n");
    printf("\t'f' or 'F' will save the picture as a PPM file\n");
    printf("\t'+' will increment and '-' decrement the minimum color index\n");
    printf("\t'r' or 's' will spin the color wheel forwards or backwards\n");
    printf("\t'W' will increment and 'w' decrement the color map selection\n");
    printf("\t'?' or 'h' will display the usage message\n");
    printf("\t'q' or 'Q' will quit\n");
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
freemem()
{
    static int i;

    for (i=0;i<=width;i++)
        free(xy[i]);
    free(xy);
}

void
setupmem()
{
    static int i;

    if ((xy=(int **)malloc((width+1)*sizeof(int *))) == (int **)NULL) {
        printf("Error malloc'ing xy.\n");
        exit (-1);
    }
    for (i=0;i<width+1;i++) {
        if ((xy[i]=(int *)malloc(height*sizeof(int)))==(int *)NULL){
            printf("Error malloc'ing xy[%d].\n", i);
            exit (-1);
        }
    }
}

void
init_pts()
{
	static int i, j;
	static int initial_x, initial_y;

	for (i=0; i<width+1; i++)
		for (j=0; j<height; j++)
			xy[i][j] = 0;
	max_y = 2;
	initial_y = 0;
	for (j=0;j<nballs;j++) {
		initial_x = ((int)lrand48() % (width-2)) + 1;
		xy[initial_x][initial_y] = coral_hue;
		BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, coral_hue, 
					initial_x, height - initial_y - 1);
	}
    FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,coral_hue,coral_hue+1);
	count = nballs;
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
    sprintf(str,"        f or F saves coral to a PPM file");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        h or H or ? displays this message");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        n goes on to the next coral");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"        N creates a new replacement coral");
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
    extern void Clear(), midpoint(), event_loop();

    freemem();
    XGetGeometry(dpy,canvas,&r,&x,&y,&new_w,&new_h,&bw,&d);
    if (((int)new_w == width) && ((int)new_h == height)) {
        setupmem();
    	InitBuffer(&Points, maxcolor);
    	Clear();
    	init_pts();
        return;
    }
    width = (int)new_w; height = (int)new_h;
    if (pixmap)
        XFreePixmap(dpy, pixmap);
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), 
            width, height, DefaultDepth(dpy, screen));
    setupmem();
    InitBuffer(&Points, maxcolor);
    Clear();
    init_pts();
    FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
}

void
Cleanup() {
    freemem();
    XCloseDisplay(dpy);
}

/* Store coral growth in PPM format */
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

void
Redraw() {
    static int i, j;
    extern void event_loop();

    FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
	Clear();
    for (i=0; i < width; i++)
    	for (j=0; j < height; j++)
			if (xy[i][j])
				BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, xy[i][j], 
							i, height - j - 1);
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
        			write_cmap(dpy,cmap,Colors,maxcolor,"coral","Coral");
					break;
                case '+': mincolindex += INDEXINC;
                    if (mincolindex > maxcolor)
                        mincolindex = 1;
                    init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"coral","Coral",0);
                    break;
                case '-': mincolindex -= INDEXINC;
                    if (mincolindex < 1)
                        mincolindex = maxcolor - 1;
                    init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"coral","Coral",0);
                    break;
                case 'd': delay -= 25; if (delay < 0) delay = 0; break;
                case 'D': delay += 25; break;
                case 'f':	/* save in PPM format file */
                case 'F': save(); break;
                case 'n':	/* go on to the next coral */
                    next = 1;
                    break;
                case 'N':	/* go on to the next coral */
                    nummaps++;	/* but don't increment the coral counter */
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
            	case '\027': /* (ctrl-W) read palette from $HOME/.coralmap */
                  numwheels = 0;
                  init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"coral","Coral",0);
                  break;
                case 'W': 
                    if (numwheels < MAXWHEELS)
                        numwheels++;
                    else
                        numwheels = 0;
                    init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"coral","Coral",0);
                    break;
                case 'w': 
                    if (numwheels > 0)
                        numwheels--;
                    else
                        numwheels = MAXWHEELS;
                    init_color(dpy,canvas,cmap,Colors,STARTCOLOR,
							mincolindex,maxcolor,numwheels,"coral","Coral",0);
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
				case 'X':	/* restart coral growth with new seeds */
					FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,0,maxcolor);
					InitBuffer(&Points, maxcolor);
					Clear();
					init_pts();
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

    outname = "coral.ppm";
    width = 512; height = 480;
    while((c = getopt(argc, argv, "CdsuFRTVc:h:k:m:n:o:w:D:N:")) != EOF)
    {    switch(c)
        {
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
        case 'k':
            col_duration = atoi(optarg);
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
        case 's':
            ++spin;
            break;
        case 'w':
            width = atoi(optarg);
            break;
        case 'C':
            usedefault--;
            break;
        case 'D':
            delay = atoi(optarg);
            break;
        case 'F':
            full++;
            break;
        case 'N':
            nballs = atoi(optarg);
            break;
        case 'R':
            useroot++;
            break;
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

int
checkit() 
{
    static int i, j;

    for (i=0;i<3;i++)
        for (j=0;j<3;j++)
            if (xy[now_x + i - 1][now_y + j - 1])
                return(0);
    return(1);
}

int
walk(x,y)
int x,y;
{
	now_x = x;
	now_y = y;
	if (checkit() == 0)
		return(1);
	for(;;) {
		event_loop();
		now_x += (((int)lrand48()%3) - 1);
		now_y--;
		if ((now_y <= 0) || (now_x >= width) || (now_x <= 0))
			return(0);
		if (checkit() == 0)
			return(1);
	}
}

aggregate() 
{
	static int enter_x, enter_y;

	enter_x = ((int)lrand48() % (width-2)) + 1;
	enter_y = (max_y > (height - 1)) ? (height - 1) : max_y;
	if (walk(enter_x,enter_y)) {
		count++;
		max_y = (now_y > (max_y - 2)) ? now_y + 2 : max_y;
		if ((count%col_duration) == 0) {
		   FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,coral_hue,coral_hue+1);
		   count=1;
		   coral_hue++;
		   if (coral_hue >= maxcolor)
				coral_hue = STARTCOLOR;
		}
		xy[now_x][now_y] = coral_hue;
		BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, coral_hue, 
					now_x, height - now_y - 1);
        FlushBuffer(dpy,canvas,pixmap,Data_GC,&Points,coral_hue,coral_hue+1);
	}
}

main(argc,argv)
int argc;
char *argv[];
{
    static int i, j;
    XSizeHints hint;
    Atom __SWM_VROOT = None;
    Window rootReturn, parentReturn, *children;
    unsigned int numChildren;
    extern void srand48(), init_color();
    
    parseargs(argc,argv);
    dpy = XOpenDisplay("");
    screen = DefaultScreen(dpy);
    if (full || useroot) {
        width = XDisplayWidth(dpy, screen);
        height = XDisplayHeight(dpy, screen);
    }
    maxcolor  = (int)XDisplayCells(dpy, screen);
	if (maxcolor <= 16) {
		STARTCOLOR = 2; delay = 100;
		INDEXINC = 1; mincolindex = 5;
	}
	maxcolor = Min(maxcolor, MAXCOLOR);
	if (col_duration == 0)
		col_duration = Min(nballs * maxcolor, 4096);
	coral_hue = STARTCOLOR;
    /*
    * Create the pixmap to hold the coral growth
    */
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), width, height, 
                           DefaultDepth(dpy, screen));
    /*
    * Create the window to display the fractal topographic map
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
            unsigned long nitems, bytesafter;
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
        XSetStandardProperties(dpy, canvas, "Coral by Ron Record",
            "Coral", None, argv, argc, &hint);
        XMapRaised(dpy, canvas);
    	XSelectInput(dpy,canvas,KeyPressMask|ExposureMask|StructureNotifyMask);
    }
    XChangeProperty(dpy, canvas, XA_WM_CLASS, XA_STRING, 8, PropModeReplace, 
					(unsigned char *)"coral", strlen("coral"));
    /*
    * Create the window used to display the help info
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
    	XSelectInput(dpy,help,KeyPressMask|ExposureMask);
    	/* Try to write into a new color map */
		if (usedefault)
		  cmap = DefaultColormap(dpy, screen);
		else {
    	  cmap = XCreateColormap(dpy,canvas,DefaultVisual(dpy,screen),AllocAll);
    	  init_color(dpy, canvas, cmap, Colors, STARTCOLOR, mincolindex, maxcolor,
					numwheels,"coral", "Coral", 0);
    	  /* install new color map */
          XSetWindowColormap(dpy, canvas, cmap);
          XSetWindowColormap(dpy, help, cmap);
	    }
	}
    init_contexts();
    setupmem();
    Clear();
    srand48((long)time(0));
    for (i=0; i!=nummaps; i++) {
        next = 0;
        InitBuffer(&Points, maxcolor);
        init_pts();
		for (;;) {
			event_loop();
			aggregate();
			if (now_y >= (height - 1))
				break;
		}
        FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
        if (oflag)
            save();
        if (demo) {
            event_loop();
			DemoSpin(dpy, cmap, Colors, STARTCOLOR, maxcolor, delay, 4);
            event_loop();
			for (j=0; j<=MAXWHEELS; j++) {
				init_color(dpy, canvas, cmap, Colors, STARTCOLOR, mincolindex, 
						maxcolor, i, "coral", "Coral", 0);
				event_loop();
				sleep(1);
			}
        }
		else if (useroot) {
        	XSetWindowBackgroundPixmap(dpy, canvas, pixmap);
			XFreePixmap(dpy, pixmap);
			XClearWindow(dpy, canvas);
			XFlush(dpy);
			Cleanup();
			exit(0);
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
