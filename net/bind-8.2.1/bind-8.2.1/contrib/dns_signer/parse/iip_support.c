/* $Header: /proj/cvs/isc/bind/contrib/dns_signer/parse/iip_support.c,v 1.1 1999/01/05 03:02:29 vixie Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/nameser.h>
#include "iip_support.h"

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

void print_hex_field (u_int8_t field[], int length, int width, char *pref)
{
	/* Prints an arbitrary bit field, from one address for some number of
		bytes.  Output is formatted via the width, and includes the raw
		hex value and (if printable) the printed value underneath.  "pref"
		is a string used to start each line, e.g., "   " to indent.

		This is very useful in gdb to see what's in a memory field.
	*/
	int		i, start, stop;

	start=0;
	do
	{
		stop=(start+width)<length?(start+width):length;
		printf (pref);
 		for (i = start; i < stop; i++)
			printf ("%02x ", (u_char) field[i]);
		printf ("\n");

		printf (pref);
		for (i = start; i < stop; i++)
			if (isprint(field[i]))
				printf (" %c ", (u_char) field[i]);
			else
				printf ("   ");
			printf ("\n");

		start = stop;
	} while (start < length);
}

void print_hex (u_int8_t field[], int length)
{
	/* Prints the hex values of a field...not as pretty as the print_hex_field.
	*/
	int		i, start, stop;

	start=0;
	do
	{
		stop=length;
 		for (i = start; i < stop; i++)
			printf ("%02x ", (u_char) field[i]);
		start = stop;
		if (start < length) printf ("\n");
	} while (start < length);
}

int complete_read (int sock, void* field, int length)
{
	int bytes;
	int bytes_read = 0;
	memset (field, '\0', length);

	do
	{
		bytes = read (sock, field+bytes_read, length-bytes_read);
		if (bytes == -1) return -1;
		if (bytes == 0) return -1;
		bytes_read += bytes;
	} while (bytes_read < length);
	return length;
}

static int seq_number = 0;
FILE		*log = NULL;

void my_free (void *p, char *filename, int lineno)
{
	if (log==NULL)
		log = fopen ("memory_log", "w");

	fprintf (log, "0x%08lx %5d bFREE %-20s %5d\n", (u_long) p, seq_number++,
								filename, lineno);
	fflush (log);
	free (p);
}

void *my_malloc (size_t t, char *filename, int lineno)
{
	void *p = malloc (t);

	if (log==NULL)
		log = fopen ("memory_log", "w");

	fprintf (log, "0x%08lx %5d aMALL %-20s %5d size=%6d\n", (u_long) p, seq_number++,
								filename, lineno, (u_int)t);
	fflush (log);

	return p;
}

char *my_strdup (const char *str, char *filename, int lineno)
{
	char *p = strdup (str);
	if (log==NULL)
		log = fopen ("memory_log", "w");

	fprintf (log, "0x%08lx %5d aSTRD %-20s %5d\n", (u_long) p, seq_number++,
								filename, lineno);
	fflush (log);

	return p;
}

/* Portions Copyright (c) 1995,1996,1997,1998 by Trusted Information Systems, Inc.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND TRUSTED INFORMATION SYSTEMS DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL TRUSTED INFORMATION 
 * SYSTEMS BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 *
 * Trusted Information Systems, Inc. has received approval from the
 * United States Government for export and reexport of TIS/DNSSEC
 * software from the United States of America under the provisions of
 * the Export Administration Regulations (EAR) General Software Note
 * (GSN) license exception for mass market software.  Under the
 * provisions of this license, this software may be exported or
 * reexported to all destinations except for the embargoed countries of
 * Cuba, Iran, Iraq, Libya, North Korea, Sudan and Syria.  Any export
 * or reexport of TIS/DNSSEC software to the embargoed countries
 * requires additional, specific licensing approval from the United
 * States Government. 
 */




