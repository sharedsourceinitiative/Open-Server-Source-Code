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
 *
 *	Written 4/22 1988 by Ron Record (sco!rr)
 *	Based in part on the Computer Recreations article in the 5/88
 *	Scientific American.
 *	
 */

#include <stdio.h>
#include <values.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <sys/types.h>
#include <macros.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/keysym.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/DrawingA.h>
#include <Xm/ToggleB.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#define ABS(a)	(((a)<0) ? (0-(a)) : (a) )
#define BANDSIZ 7
#define PROB 0.15

#define Min(x,y) ((x < y)?x:y)
#define Max(x,y) ((x > y)?x:y)

XtEventHandler getkey();

int i, j, x_center, y_center, col=1, upper=1;

typedef struct {
	int x, y;
} xy_t;

typedef struct {
	int left, right, top, bottom;
} lrtb_t;

/* maximum number of colors allowed */
#define MAXCOLOR  256

/* minimum (and default) window size */
#define MIN_WIDTH 400
#define MIN_HEIGHT 300

GC Data_GC[MAXCOLOR];

int	width=MIN_WIDTH, height=MIN_HEIGHT;
int	numcolors=16;
int hstart, hend, hmid, xwid, K=0, delay=1;
extern double sin(), cos(), exp(), drand48(), atof();

double slope;
double A;
double stepsiz=0.01;
double THETA=M_PI/12.0;
double a, b, c, d, e, f, g, h;
double rad;
int cflag=0;
int eflag=1;
int Dflag=0;
int Pflag=0;
int heart=0;
int logspiral=0;
int spiral=0;
int rotate=0;
int hcolor, color, color2, color3, color4, color5, color6, color7, color8;
int line_color;
int ncols;
int sflag=0;
int x0, x1, xx, yy;
int xy[4], save_xy[4];
int x_center, y_center;
int prob=0;
static int num_pics=MAXSHORT;
char parameter;
long Radius;
long nsteps=MAXLONG, numloops;
Screen*	 screen;
Display*	dpy;
char*	   displayname = 0;

Widget framework, canvas, button[4];
XtWorkProcId work_proc_id = (XtWorkProcId)NULL;

void	 resize();
void	 redisplay();
void	 clear();
void	 quit();
void	 start_iterate();
void	 next_color();
Boolean  draw_madness();
Boolean appDefaultsFileInstalled=False;

main(argc, argv)
int argc;
char *argv[];
{

	a = 0.3; b = 0.99; c = -0.7; d = 3.01; 
	e = 0.9; f = 1.01; g = 0.1; h = 15.03;
	parseargs(argc, argv);
	init_motif(argc, argv);
	XtMainLoop();
}

setup() {

	if (++K > num_pics) {
		XtCloseDisplay(dpy);
		exit(0);
	}
	line_color = color;
	rad = 0.0;
	if (spiral||logspiral) {
		xy[0] = x_center;
		xy[1] = y_center;
	}
	else if (heart) {
		xy[0] = 0;
		xy[1] = y_center;
	}
	else {
		xy[0]=(((a*sin(b*rad))+(c*cos(d*rad)))*x_center) + x_center;
		xy[1]=(((e*sin(f*rad))+(g*cos(h*rad)))*y_center) + y_center;
	}
	numloops=0;
}

restart() {

	setup();
	if (cflag) {
		switch(parameter) {
	   		case 'a':
	   			a += stepsiz;
	   			c -= stepsiz;
	   			break;
	   		case 'b':
	   			b += stepsiz;
		   		break;
			case 'c':
				c += stepsiz;
		   		a -= stepsiz;
		   		break;
			case 'd':
		 		d += stepsiz;
				break;
			case 'e':
	 			e += stepsiz;
		   		g -= stepsiz;
		   		break;
			case 'f':
				f += stepsiz;
				break;
			case 'g':
				g += stepsiz;
				e -= stepsiz;
				break;
			case 'h':
				h += stepsiz;
				break;
		}
	}
	color++;
	if (color == numcolors)
		color=2;
	if (sflag) {
		setcolors();
	}
	if (eflag)
		clear(canvas, NULL);
}

Boolean
draw_madness() {

	static int i, j;
	xy_t p0, p1;
	double run;

	if (++numloops >= nsteps) {
		restart();
		return TRUE;
	}
	if ((rand() % 100) < prob) {
		hcolor = color = color2 = (rand() % (numcolors-2))+2;
		if (sflag)
			setcolors();
		line_color = color;
	}
	rad = numloops*stepsiz;
	if (spiral) {
		xy[2] = ((a*rad*cos(rad))*x_center) + x_center;
		xy[3] = ((a*rad*sin(rad))*y_center) + y_center;
	}
	else if (logspiral) {
		xy[2] = (((exp(a*rad)-1.0)*cos(rad))*x_center) + x_center;
		xy[3] = (((exp(a*rad)-1.0)*sin(rad))*y_center) + y_center;
	}
	else if (heart) {
		xy[2] = stepsiz + xy[0];
		if (xy[2] >= width - stepsiz) {
			restart();
			return FALSE;
		}
		if ((numloops % BANDSIZ) == 0) {
			color++;
			if (color == numcolors)
				color=2;
			line_color = color;
		}
		if (xy[2] < hstart)
			line_color = hcolor;
		else if (xy[2] > hend)
			line_color = hcolor;
		if ((xy[2] < hstart) || (xy[2] > hend)) {
			A = drand48();
			xy[3]=A*(y_center/4)+y_center-(y_center/8);
		}
		else if (xy[2] < hmid) {
			if (upper)
				A = y_center+circ(xy[2],x0);
			else
				if (xy[2] < (hmid-2*xwid))
					A = y_center-circ(xy[2],x0);
				else
					A = y_center-(xy[2]-hmid)-3*xwid;
			xy[3]=height - A;
			if (drand48() > PROB)
				upper=!upper;
		}
		else {
			if (upper)
				A = y_center+circ(xy[2],x1);
			else
				if (xy[2] > (hmid+2*xwid))
					A = y_center-circ(xy[2],x1);
				else
					A = y_center+(xy[2]-hmid)-3*xwid;
			xy[3]=height - A;
			if (drand48() > PROB)
				upper=!upper;
		}
	}
	else {
		xy[2]=(((a*sin(b*rad))+(c*cos(d*rad)))*x_center) + x_center;
		xy[3]=(((e*sin(f*rad))+(g*cos(h*rad)))*y_center) + y_center;
	}
	if (spiral || logspiral)
		if ((ABS(xy[2]) > width-2) ||
			(ABS(xy[3]) > height) || (min(xy[2],xy[3]) < 0))
			return FALSE;
	if (sflag) {
		if ((run = xy[2]-xy[0]) == 0)
			slope = MAXDOUBLE;
		else
			slope = (xy[3]-xy[1])/run;
		setline();
	}
	if (rotate) {
		save_xy[0] = xy[0]; save_xy[1] = xy[1];
		save_xy[2] = xy[2]; save_xy[3] = xy[3];
		xx = xy[2] - x_center; yy = xy[3] - y_center;
		xy[2] = (xx*cos(K*THETA)) - (yy*sin(K*THETA));
		xy[3] = (xx*sin(K*THETA)) + (yy*cos(K*THETA));
		xy[2] = xy[2] + x_center; xy[3] = xy[3] + y_center;
		xx = xy[0] - x_center; yy = xy[1] - y_center;
		xy[0] = (xx*cos(K*THETA)) - (yy*sin(K*THETA));
		xy[1] = (xx*sin(K*THETA)) + (yy*cos(K*THETA));
		xy[0] = xy[0] + x_center; xy[1] = xy[1] + y_center;
	}
	if (in_window()) {
		p0.x = xy[0]; p0.y = xy[1]; p1.x = xy[2]; p1.y = xy[3];
		XDrawLine(dpy, XtWindow(canvas), Data_GC[line_color], 
				p0.x, p0.y, p1.x, p1.y);
#ifdef USE_DELAY
		Timer(delay);
#endif
	}
	if (rotate) {
		xy[0] = save_xy[0]; xy[1] = save_xy[1];
		xy[2] = save_xy[2]; xy[3] = save_xy[3];
	}
	xy[0] = xy[2]; xy[1] = xy[3];
	if (heart) {
		xy[2] += stepsiz;
		if (xy[2] >= width - stepsiz) {
			restart();
			return FALSE;
		}
		xy[3] = y_center;
		if (rotate) {
			save_xy[0] = xy[0]; save_xy[1] = xy[1];
			save_xy[2] = xy[2]; save_xy[3] = xy[3];
			xx = xy[2] - x_center; yy = xy[3] - y_center;
			xy[2] = (xx*cos(K*THETA)) - (yy*sin(K*THETA));
			xy[3] = (xx*sin(K*THETA)) + (yy*cos(K*THETA));
			xy[2] = xy[2] + x_center; xy[3] = xy[3] + y_center;
			xx = xy[0] - x_center; yy = xy[1] - y_center;
			xy[0] = (xx*cos(K*THETA)) - (yy*sin(K*THETA));
			xy[1] = (xx*sin(K*THETA)) + (yy*cos(K*THETA));
			xy[0] = xy[0] + x_center; xy[1] = xy[1] + y_center;
		}
		if (in_window()) {
			p0.x = xy[0]; p0.y = xy[1]; p1.x = xy[2]; p1.y = xy[3];
			XDrawLine(dpy, XtWindow(canvas), Data_GC[line_color], 
				p0.x, p0.y, p1.x, p1.y);
#ifdef USE_DELAY
			Timer(delay);
#endif
		}
		if (rotate) {
			xy[0] = save_xy[0]; xy[1] = save_xy[1];
			xy[2] = save_xy[2]; xy[3] = save_xy[3];
		}
		xy[0] = xy[2]; xy[1] = xy[3];
	}
	return FALSE;
}

usage()
{
	printf("Usage: madness [-uAELRS] [-C parm] [-n steps] [-I size]\n");
	printf("\t[-N pics] [-T div] [-W width] [-H height]\n");
	printf("\t\t[-abczefgh num] [-s ncols]\n");
	printf("\twhere :\n\t-u\tdisplays this message\n");
	printf("\t-E\tdoesn't erase each picture\n");
	printf("\t-A\tdraws the earthquake relief logo\n");
	printf("\t-L\tdraws logarithmic spirals (r,@) where r = exp(a*@)\n");
	printf("\t-S\tdraws spirals of the form (r,@) where r = a*@\n");
	printf("\t-C parm increments the specified parameter (a-h) each picture\n");
	printf("\t-R\trotates each successive picture by pi/12\n");
	printf("\t-T div\tsets to angle of rotation to M_PI/div\n");
	printf("\tsteps\tindicates the number of steps in each picture\n");
	printf("\tpics\tindicates the number of pictures in each run\n");
	printf("\tsize indicates the parametric increment at each step\n");
	printf("\tncols indicates the number of colors to use (2, 4, or 8)\n");
	printf("\t-abczefgh followed by num sets the parameter value to num\n");
	printf("\t\tin the system of parametric equations\n\n");
	printf("\t\tx = a*sin(b*t) + c*cos(z*t) and\n");
	printf("\t\ty = e*sin(f*t) + g*cos(h*t)\n\n");
}

setcolors()
{
	color2 = color + 1;
	if (color2 == numcolors)
		color2=2;
	color3 = color2 + 1;
	if (color3 == numcolors)
		color3=2;
	color4 = color3 + 1;
	if (color4 == numcolors)
		color4=2;
	color5 = color4 + 1;
	if (color5 == numcolors)
		color5=2;
	color6 = color5 + 1;
	if (color6 == numcolors)
		color6=2;
	color7 = color6 + 1;
	if (color7 == numcolors)
		color7=2;
	color8 = color7 + 1;
	if (color8 == numcolors)
		color8=2;
}

setline()
{
	switch(ncols)
	{
	case 2:
		if (slope > 0)
			line_color = color;
		else
			line_color = color2;
		break;
	case 4:
		if (slope > 1.0)
			line_color = color2;
		else if (slope > 0)
			line_color = color;
		else if (slope < -1.0)
			line_color = color3;
		else
			line_color = color4;
		break;
	case 8:
		if (slope > 2.0)
			line_color = color4;
		else if (slope > 1.0)
			line_color = color3;
		else if (slope > 0.5)
			line_color = color2;
		else if (slope > 0.0)
			line_color = color;
		else if (slope < -2.0)
			line_color = color5;
		else if (slope < -1.0)
			line_color = color6;
		else if (slope < -0.5)
			line_color = color7;
		else
			line_color = color8;
		break;
	default:
		sflag=0;
		line_color = color;
		break;
	}
}

circ(x,rx)
int x, rx;
{
	int a, y;
	static double d;
	extern double sqrt();

	a = ABS(x - rx);
	d = (double)(ABS(Radius-(a*a)));
	y = (int)sqrt(d);
	return(y);
}

XtEventHandler
getkey(w, client_data, event)
Widget w;
caddr_t client_data;
XKeyEvent *event;
{
	switch(XLookupKeysym(event, 0))
	{
	case XK_Q:
	case XK_q:			/* quit */
		XtCloseDisplay(dpy);
		exit(0);
		break;
	case XK_e:			/* clear screen  */
		clear(canvas, NULL);
		break;
	case XK_d:			/* toggle erase */
		eflag=(!eflag);
		break;
	case XK_p:			/* toggle pause */
		Pflag=(!Pflag);
		break;
	}
}

in_window() {

	if (xy[0] > width)
		return(0);
	if (xy[0] < 0)
		return(0);
	if (xy[2] > width)
		return(0);
	if (xy[2] < 0)
		return(0);
	if (xy[1] > height)
		return(0);
	if (xy[1] < 0)
		return(0);
	if (xy[3] > height)
		return(0);
	if (xy[3] < 0)
		return(0);
	return(1);
}

parseargs(argc, argv)
int argc;
char *argv[];
{
	char ch; 
	extern int optind;
	extern long atol();
	extern char *optarg;
	while((ch=getopt(argc,argv,
		"uADELPRSn:T:N:C:H:I:W:Z:a:b:c:z:e:f:g:h:p:s:")) !=EOF)
	{	switch(ch)
		{
		case 's':
			sflag++;
			ncols = atoi(optarg);
			if ((ncols != 2) && (ncols != 4) && (ncols != 8)) {
				printf("madness: -s flag must be followed by 2, 4, or 8\n");
				exit(2);
			}
			break;
		case 'u':
			usage();
			exit(1);
			break;
		case 'n':
			nsteps = atol(optarg);
			break;
		case 'a':
			a = atof(optarg);
			c = 1.0 - ABS(a);
			break;
		case 'b':
			b = atof(optarg);
			break;
		case 'c':
			c = atof(optarg);
			a = 1.0 - ABS(c);
			break;
		case 'z':
			d = atof(optarg);
			break;
		case 'e':
			e = atof(optarg);
			g = 1.0 - ABS(e);
			break;
		case 'f':
			f = atof(optarg);
			break;
		case 'g':
			g = atof(optarg);
			e = 1.0 - ABS(g);
			break;
		case 'h':
			h = atof(optarg);
			break;
		case 'A':
			heart++;
			break;
		case 'C':
			cflag++;
			parameter = *optarg;
			break;
		case 'D':
			Dflag++;
			break;
		case 'E':
			eflag--;
			break;
		case 'H':
			height = atoi(optarg);
			break;
		case 'I':
			stepsiz = atof(optarg);
			break;
		case 'L':
			logspiral++;
			break;
		case 'N':
			num_pics = atoi(optarg);
			break;
		case 'P':
			Pflag++;
			break;
		case 'R':
			rotate++;
			break;
		case 'S':
			spiral++;
			break;
		case 'T':
			THETA = M_PI/(atof(optarg));
			break;
		case 'W':
			width = atoi(optarg);
			break;
#ifdef USE_DELAY
		case 'Z':
			delay = atoi(optarg);
			break;
#endif
		case 'p':
			prob = atoi(optarg);
			break;
		case '?':
			usage();
			exit(1);
			break;
		}
	}
	if (argc-optind != 0) {
		usage();
		exit(1);
	}
	if (Dflag) {
		if (nsteps == MAXLONG)
			nsteps = 10000;
		if (num_pics == MAXSHORT)
			num_pics = 1;
	}
}

void
init_values() {

	x_center = width / 2;
	y_center = height / 2;
	if (heart) {
		hmid = x_center;
		hstart = 2*hmid/3;
		hend = hmid + (hmid/3);
		xwid = (hend - hstart)/(2+2*M_SQRT2);
		Radius = xwid*xwid + xwid*xwid;
		x0 = hmid - xwid;
		x1 = hmid + xwid;
		stepsiz = 1.0;
	}
}

init_motif(argc, argv)
int argc;
char *argv[];
{
	Widget toplevel;
	int i, j;
	char wname[256];
	static XtResource resource[] = {
	{ "appDefaultsFileInstalled" , "AppDefaultsFileInstalled", XtRBoolean,
		sizeof(Boolean), 0, XtRString, "False" } };

	toplevel = XtInitialize(argv[0], "Madness", NULL, 0, &argc, argv);
	framework = XtCreateManagedWidget("framework",
			 xmFormWidgetClass, toplevel, NULL, 0);
	/*
	* Create the canvas widget to display the madness
	*/
	canvas = XtCreateManagedWidget("drawing_canvas",
		   xmDrawingAreaWidgetClass, framework, NULL, 0);
	dpy = XtDisplay(canvas);
	screen = XtScreen(canvas);
	/*
	* Create the pushbutton widgets.
	*/
	button[0] =  XtCreateManagedWidget("go_button",
				xmPushButtonWidgetClass,
				framework, NULL, 0);
	button[1] =  XtCreateManagedWidget("next_button",
				xmPushButtonWidgetClass,
				framework, NULL, 0);
	button[2] =  XtCreateManagedWidget("clear_button",
				xmPushButtonWidgetClass,
				framework, NULL, 0);
	button[3] =  XtCreateManagedWidget("quit_button",
				xmPushButtonWidgetClass,
				framework, NULL, 0);
	init_data();
	init_canvas();
	init_values();
	srand((int)time(0));
	/*
	* Add callbacks.
	*/
	XtAddCallback(button[0],XmNactivateCallback,start_iterate,NULL);
	XtAddCallback(button[1],XmNactivateCallback,next_color,NULL);
	XtAddCallback(button[2], XmNactivateCallback, clear, NULL);
	XtAddCallback(button[3], XmNactivateCallback, quit, NULL);
	XtRealizeWidget(toplevel);
	/* Title */
	sprintf((char *) wname, "Madness by Ron Record");
 	XChangeProperty(dpy, XtWindow(toplevel), XA_WM_NAME, XA_STRING, 8, 
 		PropModeReplace, wname, strlen(wname));
	XtAddCallback(canvas, XmNresizeCallback, resize, NULL);
	XtAddEventHandler(framework, KeyPressMask, FALSE, getkey, NULL);
	XtAddEventHandler(button[0], KeyPressMask, FALSE, getkey, NULL);
	XtAddEventHandler(button[1], KeyPressMask, FALSE, getkey, NULL);
	XtAddEventHandler(button[2], KeyPressMask, FALSE, getkey, NULL);
	XtAddEventHandler(button[3], KeyPressMask, FALSE, getkey, NULL);
	resize(canvas, NULL, NULL);
	work_proc_id = XtAddWorkProc(draw_madness, canvas);
	XtGetApplicationResources(toplevel, &appDefaultsFileInstalled, resource,
			1, NULL, 0);
	if (!appDefaultsFileInstalled)
		fprintf(stderr, 
				"Warning: the Madness app-defaults file is not installed\n");
}

init_data()
{
	long pid;
		extern void srand48();

	numcolors = XDisplayCells(dpy, XDefaultScreen(dpy));
	numcolors = Min(numcolors, MAXCOLOR) - 1;
	pid = getpid();
	srand(pid);
	srand48(pid);
	hcolor = color = (rand() % (numcolors-2))+2;
	if (numcolors < 10)
		sflag--;
	if (sflag)
		setcolors();
}

init_canvas()
{
	Arg wargs[2];
	static int i;

	/*
	* Set the size of the drawing areas.
	*/
	if (width == 0)
		width = XDisplayWidth(dpy, DefaultScreen(dpy));
	if (height == 0)
		height = XDisplayHeight(dpy, DefaultScreen(dpy));
	if (MIN_WIDTH > width)
		width = MIN_WIDTH;
	if (MIN_HEIGHT > height)
		height = MIN_HEIGHT;
	XtSetArg(wargs[0], XtNwidth, width);
	XtSetArg(wargs[1], XtNheight, height);
	XtSetValues(framework, wargs,2);
	/*
	* create default, writable, graphics contexts for the canvas.
	*/
	for (i=0; i<MAXCOLOR; i++) {
		Data_GC[i] = XCreateGC(dpy, DefaultRootWindow(dpy), 0, 
				(XGCValues *)NULL);
		/* set the background to black */
		XSetBackground(dpy, Data_GC[i], 
			BlackPixel(dpy, XDefaultScreen(dpy)));
		/* set the foreground of the i'th context to i */
		XSetForeground(dpy, Data_GC[i], i);
	}
}

void
resize(w, data, call_data)
Widget	 w;
struct image_data	*data;
caddr_t	call_data;
{
	static Arg wargs[2];
	static Dimension W=0, H=0;
	/*
	*   Get the new window size.
	*/
	XtSetArg(wargs[0], XtNwidth,  &W);
	XtSetArg(wargs[1], XtNheight, &H);
	XtGetValues(w, wargs, 2);
	/* recalculate offsets */
	width = (int)W; height = (int)H;
	init_values();
	/*
	* Clear the window.
	*/
	clear(canvas, NULL);
	setup();
}

void
start_iterate(w, call_data)
Widget w;
XmAnyCallbackStruct *call_data;
{
	XmString label;
	Arg wargs[1];

	if (work_proc_id) {
		XtRemoveWorkProc(work_proc_id);
		work_proc_id = (XtWorkProcId)NULL;
		label = XmStringCreate("RESUME",XmSTRING_DEFAULT_CHARSET);
		XtSetArg(wargs[0], XmNlabelString, label);
		XtSetValues(button[0], wargs, 1);
	}
	else {
		/*
		* Register draw_madness() as a WorkProc.
		*/
		work_proc_id = XtAddWorkProc(draw_madness, canvas);
		label = XmStringCreate("PAUSE",XmSTRING_DEFAULT_CHARSET);
		XtSetArg(wargs[0], XmNlabelString, label);
		XtSetValues(button[0], wargs, 1);
	}
}

void
next_color(w, call_data)
Widget w;
XmAnyCallbackStruct  *call_data;
{
		line_color = hcolor = color = (rand() % (numcolors-2))+2;
		if (numcolors < 10)
			sflag--;
		if (sflag)
			setcolors();
}

void
clear(w, call_value)
Widget w;
XmAnyCallbackStruct *call_value;
{
	if(XtIsRealized(canvas))
		XClearArea(dpy, XtWindow(canvas), 0, 0, 0, 0, TRUE);
}

void
quit(w, call_value)
Widget w;
XmAnyCallbackStruct *call_value;
{
	XtCloseDisplay(dpy);
	exit(0);
}
