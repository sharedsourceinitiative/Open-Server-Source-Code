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
#include <string.h>
#include <sys/types.h>
#include "defines.h"
#include "externals.h"

/* external routines called in this file */
extern void usage();

void
parseargs(ac, av)
int ac;
char **av;
{
	int c;
	extern int optind;
	extern char *optarg;

	valinit = strdup("xxx");
	while ((c = getopt(ac, av,
		"2Ax:cvepmPqsub:B:r:C:D:E:R:F:h:H:w:W:n:o:i:L:N:M:T:U:"))!=EOF)
	{	switch(c)
		{
		case '2':
			++two_dim;
			break;
		case 'u':
			usage();
			exit(0);
		case 'c':
			++cflag;
			break;
		case 'C':
			if (*optarg == 'r')
				rancon++;				/* random connection strengths */
			else if (*optarg == 'l')
				lincon++;				/* linear connection strengths */
			else if (*optarg == 'p') {
				optarg++;
				G = atoi(optarg);
				percon++;				/* periodic connection strengths */
			}
			else
				EC = atof(optarg);
			break;
		case 'D':
			ED = atof(optarg);
			break;
		case 'E':
			++Eflag;
			erate = atof(optarg);
			break;
		case 'p':
			++pflag;
			break;
		case 's':
			++sflag;
			break;
		case 'A':
			++hflag;
			break;
		case 'o':
			outname = optarg;
			oflag++;
			break;
		case 'e':
			++eflag;
			break;
		case 'm':
			++mflag;
			break;
		case 'P':
			++Pflag;
			break;
		case 'L':
			EL = atof(optarg);
			break;
		case 'R':
			ER = atof(optarg);
			break;
		case 'T':
			tent=1;
			logistic=0;
			omega=atof(optarg);
			if (!lamvalset)
				lamval=0.5;
			break;
		case 'U':
			EU = atof(optarg);
			break;
		case 'M':
			logistic=0;
			circle=1;
			if (!lamvalset)
				lamval=0.1;
			omega = atof(optarg);
			break;
		case 'r':
			RANGE = atof(optarg);
			ORANGE = 1.0 - RANGE;
			break;
		case 'v':
			vflag++;
			break;
		case 'W':
			width = atoi(optarg);
			break;
		case 'w':
			wide = atoi(optarg);
			break;
		case 'H':
			height = atoi(optarg);
			break;
		case 'h':
			high = atoi(optarg);
			break;
		case 'x':
			xflag++;	/* use complex dynamical system mode */
			xavg = atoi(optarg);
			break;		/* state of cell to left is control parameter */
		case 'B':
			if (*optarg == 'r') {
				periodic=0;
				boundflag=2;
			}
			else if (*optarg == 'f') {
				periodic=0;
				boundflag=1;
			}
			else {
				boundary = atof(optarg);
				boundflag=3;
				periodic=0;
			}
			break;
		case 'n':
			lamvalset++;
			if (*optarg == 'r')
				ranlam++;
			else if (*optarg == 'l')
				linlam++;
			else if (*optarg == 'p') {
				laminit = optarg;
				perlam++;
				laminit++;
				if (*laminit == '\0')
					A = 1;
				else
					while (*laminit != '\0')
			  			A = (A*10)+(*laminit++ - '0');
			}
			else
				lamval = atof(optarg);
			break;
		case 'i':
			iflag++;
			valinit = optarg;
			break;
		case 'b':
			begin = atoi(optarg);
			break;
		case 'F':
			freq = atoi(optarg);
			break;
		case 'q':
			quilt = 0;
			break;
		case '?':
			usage();
			exit(1);
			break;
		}
	}
	if (ac-optind != 0) {
		usage();
		exit(1);
	}
}
