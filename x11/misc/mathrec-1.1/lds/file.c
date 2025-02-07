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

void
Cleanup() {
	freemem();
	XCloseDisplay(dpy);
}

/* Store color pics in PPM format and monochrome in PGM */
void
save_to_file(map) 
Pixmap map;
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

	if (colormap)
		free(colormap);
	if ((colormap=
		(struct Colormap *)malloc(sizeof(struct Colormap)*numcolors))
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

	ximage=XGetImage(dpy, map, 0, 0, width, height, AllPlanes, ZPixmap);

	if (displayplanes > 1) {
		for (i=0;i<numcolors;i++) {
			colormap[i].red=(unsigned char)(Colors[i].red >> 8);
			colormap[i].green=(unsigned char)(Colors[i].green >> 8);
			colormap[i].blue =(unsigned char)(Colors[i].blue >> 8);
		}
		fprintf(outfile,"P%d %d %d\n",6,width,height);
	}
	else
		fprintf(outfile,"P%d %d %d\n",5,width,height);
	fprintf(outfile,"%d\n",numcolors-1);

	for (j=0;j<height;j++)
	    for (i=0;i<width;i++) {
		c = (unsigned char)XGetPixel(ximage,i,j);
		if (displayplanes > 1)
		    fwrite((char *)&colormap[c],sizeof colormap[0],1,outfile);
		else
		    fwrite((char *)&c,sizeof c,1,outfile);
	    }
	fclose(outfile);
}
