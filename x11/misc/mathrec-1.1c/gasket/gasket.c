/*
 * gasket - a general serpinski gasket generator
 * usage: gasket [-MCRWiu][-f file][-n numpts][-N numgen][-h high][-w wide]
 * 
 * Originally by Cloister Bell (cloister@milton.u.washington.edu).
 * Rewritten by Ron Record (rr@sco.com).
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <math.h>
#include <stdio.h>
#include <dirent.h>
#include <values.h>

#define TRUE 1
#define FALSE 0
#define MINCOLOR 2
#define MAXCOLOR 64
#define MAXVERT 16
#define SAVEDIR "/usr/local/mathrec/gasket"

#ifdef M_SYSV
#define random rand
#define srandom srand
#endif

char windowname[]= "Serpinski Gaskets";

Display *display;
Window window;
GC gc[MAXCOLOR];
int Colors;
int fflag=0, iflag=0, rflag=0, Cflag=0, Mflag=0, Rflag=0, Qflag=1, Wflag=0;
char *infile;
char foofile[1] = ".";
int   x_coord[20], y_coord[20];
int   allowed[20][20], n;
int   x, y, prev;
int   maxgen=MAXINT, numgen=0, numpts=10000, skipnum=0;
int   width=500, height=500;
double   x_weight=2, y_weight=2;

extern long random();

draw()
{	
  int   next;
  int	i, j;
  int   good;

  for(i=0; i < numpts; i++)
    {
      good = 0;
      while (!good)
        {
	  next = ((int)(random()) % n) + 1;
	  good = allowed[prev][next];
        }
      x = (int)((double)(x + x_coord[next]) / x_weight);
      y = (int)((double)(y + y_coord[next]) / y_weight);
      XDrawPoint(display, window, gc[MINCOLOR + prev], x, y);
      prev = next;
    }
  if (++numgen > maxgen) {
	if ((!Cflag) && (!iflag))
	   Qflag=0;
	return(0);
  }
  return(1);
}

main(argc,argv)
     int argc;
     char **argv;
{
  XEvent event;
  XSizeHints hint;
  int screen;
  unsigned long foreground, background;
  int i, j;
  char text[10];
  int done, interval;
  int D=0, update=50;
  DIR *dir_pt;
  struct dirent *dp;
  
  display = XOpenDisplay("");
  screen = DefaultScreen(display);
  background = BlackPixel(display, screen);
  Colors = XDisplayCells(display, screen);
  Parseargs(argc, argv);
  D = (int) DisplayPlanes(display,screen);
  if (D > 1) {
	foreground = MINCOLOR;
	background = WhitePixel(display,screen);
  }
  else
	Mflag=1;
  if (Mflag) {
	foreground = WhitePixel(display,screen);
    background = BlackPixel(display, screen);
  }
  hint.x = 300;
  hint.y = 400;
  hint.width = width;
  hint.height = height;
  hint.flags = PPosition | PSize;
  init();
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),
			       hint.x, hint.y, hint.width, hint.height,
			       5, foreground, background);
  XSetStandardProperties(display, window, windowname, windowname, None,
			 argv, argc, &hint);
  for (i=0; i<MAXCOLOR; i++) {
  	gc[i] = XCreateGC(display, window, 0, 0);
  	XSetBackground(display, gc[i], background);
	if (Mflag)
  	    XSetForeground(display, gc[i], foreground);
	else
  	    XSetForeground(display, gc[i], (i%(Colors-1))+1);
  }
  XSelectInput(display, window, KeyPressMask | ButtonPressMask | ExposureMask);
  XMapRaised(display, window);
  interval=0;
  if (Cflag) {
      dir_pt = opendir(SAVEDIR);
      while (foofile[0] == '.') {
        dp = readdir(dir_pt);
        foofile[0] = dp->d_name[0];
      }
      if (dp == NULL) {
	perror("gasket: ");
	fprintf(stderr,"No files in save directory\n");
	exit(3);
      }
      getparams(dp);
  }
  if (rflag)
      restparams();
  prev = (random() % n) + 1;
  nodraw();
  while (Qflag) {
    while (draw())
    {   
     if ((interval = XEventsQueued(display, QueuedAfterFlush)) > 0);
	while (interval--) {
    	  XNextEvent(display, &event);
          switch(event.type) 
            {
            case KeyPress:
	      switch (XLookupKeysym(&event, 0)) {
	        case XK_C:
	        case XK_c:
			XClearArea(display, window, 0, 0, 0, 0, TRUE);
			break;
	        case XK_Q:
	        case XK_q:
	        case XK_Cancel:
	        case XK_Break:
		      bye();
	        case XK_f:
	        case XK_F:
	        case XK_w:
	        case XK_W:
			saveparams();
			break;
	      }
	      break;
            case Expose:
	      break;
            case ButtonPress:
	      switch(event.xbutton.button) 
	        {
	        case Button1:
		  XClearArea(display, window, 0, 0, 0, 0, TRUE);
	          break;
	        case Button2:
		  saveparams();
	          break;
	        case Button3:
	          bye();
	          break;
	        }
	      break;
            }
        }
    }
    if (Cflag) {
      dp = readdir(dir_pt);
      if (dp == NULL)
	if (iflag) {
		closedir(dir_pt);
		foofile[0] = '.';
      		dir_pt = opendir(SAVEDIR);
      		while (foofile[0] == '.') {
        		dp = readdir(dir_pt);
        		foofile[0] = dp->d_name[0];
      		}
      		if (dp == NULL) {
			perror("gasket: ");
			fprintf(stderr,"No files in save directory\n");
			exit(3);
      		}
      		getparams(dp);
	}
	else
		Qflag=0;
      else
      	getparams(dp);
    }
    else if (Rflag && iflag) {
	randparams();
	XClearArea(display, window, 0, 0, 0, 0, TRUE);
	numgen=0;
    }
    else if (!iflag)
	Qflag=0;
    x = x_coord[1];
    y = y_coord[1];
    prev = (random() % n) + 1;
    nodraw();
  }
}

Parseargs(ac, av)
int ac;
char **av;
{
	register int c;
	extern int optind;
	extern char *optarg;
	extern double atof();

	while ((c = getopt(ac, av, "N:MCFRS:X:Y:Wf:h:in:ruw:")) != EOF) {
		switch (c) {
		case 'C':	Cflag++; break;
		case 'F':	width = (int)DisplayWidth(display,0) - 20; 
				height = (int)DisplayHeight(display,0) - 30;
				break;
		case 'M':	Mflag++; break;
		case 'N':	maxgen = atoi(optarg); break;
		case 'R':	Rflag++; break;
		case 'S':	skipnum = atoi(optarg); break;
		case 'W':	Wflag++; break;
		case 'X':	x_weight = atof(optarg); break;
		case 'Y':	y_weight = atof(optarg); break;
		case 'f':	infile = optarg; fflag++; break;
		case 'h':	height = atoi(optarg); break;
		case 'i':	iflag++; break;
		case 'n':	numpts = atoi(optarg); break;
		case 'r':	rflag++; break;
		case 'w':	width = atoi(optarg); break;
		case 'u':
		case '?':	usage(); break;
		}
	}
	if ((Cflag && rflag) || (Cflag && fflag) || (rflag && fflag)) {
		fprintf(stderr,"Only one of -C, -f, -r allowed\n");
		usage();
	}
}

init() {

  FILE *file;
  static int i, j; 
  char *tempfile;

  srandom((int)time(0));
  srand48((int)time(0));
  if (fflag) {
      if (infile[0] == '$') {
	tempfile = (char *)strdup(&infile[1]);
	strcpy(infile, SAVEDIR);
	strcat(infile, tempfile);
      }
      file = fopen(infile, "r");
      if (!file) {
	perror("gasket: ");
	fprintf(stderr,"Failed to open %s\n",infile);
	exit(2);
      }
      fscanf(file,"%d",&n);
      for (i = 0; i < n; i++)
	fscanf(file, "%d %d",&x_coord[i+1],&y_coord[i+1]);
      for (i = 0; i < n; i++)
	for (j = 0; j < n; j++)
	  fscanf(file,"%d", &allowed[i+1][j+1]);
      fclose(file);
  }
  else if ((!Cflag) && (!Rflag) && (!rflag)) {
      printf("enter number of vertices: ");
      scanf("%d",&n);
      printf("enter co-ordinates for each vertex, as x y pairs:\n");
      for(i=0; i < n; i++)
	scanf("%d %d",&x_coord[i+1],&y_coord[i+1]);
      printf("enter next-allowed matrix (0's and 1's):\n");
      for (i=0; i < n; i++)
	for (j=0; j < n; j++)
	  scanf("%d", &allowed[i+1][j+1]);
  }
  else if (Rflag)
	randparams();
  x = x_coord[1];
  y = y_coord[1];
}

nodraw()
{
  static int next, good;
  static int i;

  for(i=0; i < skipnum; i++)
    {
      good = 0;
      while (!good)
        {
	  next = ((int)(random()) % n) + 1;
	  good = allowed[prev][next];
        }
      x = (int)((x + x_coord[next]) / 2);
      y = (int)((y + y_coord[next]) / 2);
      prev = next;
    }
}

getparams(dp)
struct dirent *dp;
{
	char infile[128];
  	FILE *file;
  	static int i, j;

	strcpy(infile, SAVEDIR);
	strcat(infile, "/");
	strcat(infile, dp->d_name);
      	file = fopen(infile, "r");
      	if (!file) {
		perror("gasket: ");
		fprintf(stderr,"Failed to open %s\n",infile);
		exit(2);
        }
        fscanf(file,"%d",&n);
        for (i = 0; i < n; i++)
	  fscanf(file, "%d %d",&x_coord[i+1],&y_coord[i+1]);
        for (i = 0; i < n; i++)
	  for (j = 0; j < n; j++)
	    fscanf(file,"%d", &allowed[i+1][j+1]);
        fclose(file);
	XClearArea(display, window, 0, 0, 0, 0, TRUE);
	numgen=0;
}

restparams()
{
	FILE *sf;
	char Savefile[128];
	static int i, j;

	strcpy(Savefile, getenv("HOME"));
	strcat(Savefile, "/.gasket");
	sf = fopen(Savefile, "r");
	if (!sf) {perror("gasket: ~/.gasket"); exit(1); }
        fscanf(sf,"%d",&n);
        for (i = 0; i < n; i++)
	  fscanf(sf, "%d %d",&x_coord[i+1],&y_coord[i+1]);
        for (i = 0; i < n; i++)
	  for (j = 0; j < n; j++)
	    fscanf(sf,"%d", &allowed[i+1][j+1]);
        fclose(sf);
}

saveparams()
{
	FILE *sf;
	char Savefile[128];
	static int i, j;

	strcpy(Savefile, getenv("HOME"));
	strcat(Savefile, "/.gasket");
	sf = fopen(Savefile, "w");
	if (!sf) {perror("gasket: ~/.gasket"); exit(1); }
	fprintf(sf,"%d\n",n);
	for (i=0; i<n; i++)
		fprintf(sf,"%d %d\n", x_coord[i+1], y_coord[i+1]);
	for (i=0; i<n; i++) {
	    for (j=0; j<n; j++)
		fprintf(sf,"%d ", allowed[i+1][j+1]);
	    fprintf(sf,"\n");
	}
	fclose(sf);
}

randparams()
{
        /*
	 * Compute polar coordinates with 0 < r < (width/2) and theta in 
         * successive pie slices of angular width 2*PI/n.
         * Then change back to cartesian coordinates via
         * x=r*cos(theta) and y=r*sin(theta)
         */
	static int i, j, r, good, row;
	static double theta, tpiovern;
	extern double sin(), cos(), drand48();

	n = (random() % MAXVERT) + 4;
	for (i=0; i<n; i++) {
		r = random() % (width/2);
		tpiovern = (2.0 * M_PI) / (double)n;
		theta = (drand48() * tpiovern) + ((double)i * tpiovern);
		x_coord[i+1] = (int)((double)r * cos(theta)) + (width/2);
		y_coord[i+1] = (int)((double)r * sin(theta)) + (height/2);
	}
	good = 0;
	while (!good) {
	  good = 1;
          for (i=0; i < n; i++) {
	    row=0;
	    for (j=0; j < n; j++) {
		allowed[i+1][j+1] = (random() % 3) % 2; /* 1/3 chance of a 1 */
		if ((allowed[i+1][j+1]) && ((i+1) != (j+1))) row=1;
	    }
	    good = good && row; /* force at least one 1 in each matrix row */
	  }
	}
	if (Wflag)
	    saveparams();
}

usage()
{
	fprintf(stderr,"gasket [-MCRWiru][-f file][-n numpts][-N numgen]\n");
	fprintf(stderr,"\twhere:\t-M indicates Monochrome\n");
	fprintf(stderr,"\t\t-C indicates cycle thru saved data files\n");
	fprintf(stderr,"\t\t-R indicates pick parameters at random\n");
	fprintf(stderr,"\t\t-W indicates write out parameters each run\n");
	fprintf(stderr,"\t\t-i indicates infinite mode\n");
	fprintf(stderr,"\t\t-r recalls saved parameters in $HOME/.gasket\n");
	fprintf(stderr,"\t\t-u displays this message\n");
	fprintf(stderr,"\t\tnumpts is the number of points per color\n");
	fprintf(stderr,"\t\tnumgen is the number of color changes\n");
	exit(1);
}

bye()
{
  XDestroyWindow(display, window);
  XCloseDisplay(display);
  exit(0);
}
