/*
 * Copyright (c) 1996,1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static const char rcsid[] = "$Id: bitncmp.c,v 1.6 1999/01/08 19:25:20 vixie Exp $";
#endif

#include "port_before.h"

#include <sys/types.h>

#include <string.h>

#include "port_after.h"

#include <isc/misc.h>

/*
 * int
 * bitncmp(l, r, n)
 *	compare bit masks l and r, for n bits.
 * return:
 *	-1, 1, or 0 in the libc tradition.
 * note:
 *	network byte order assumed.  this means 192.5.5.240/28 has
 *	0x11110000 in its fourth octet.
 * author:
 *	Paul Vixie (ISC), June 1996
 */
int
bitncmp(const void *l, const void *r, int n) {
	u_int lb, rb;
	int x, b;

	b = n / 8;
	x = memcmp(l, r, b);
	if (x)
		return (x);

	lb = ((u_char *)l)[b];
	rb = ((u_char *)r)[b];
	for (b = n % 8; b > 0; b--) {
		if ((lb & 0x80) != (rb & 0x80)) {
			if (lb & 0x80)
				return (1);
			return (-1);
		}
		lb <<= 1;
		rb <<= 1;
	}
	return (0);
}
