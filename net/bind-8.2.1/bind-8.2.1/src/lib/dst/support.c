static char rcsid[] = "$Header: /proj/cvs/isc/bind/src/lib/dst/support.c,v 1.5 1999/02/23 05:26:19 vixie Exp $";


/*
 * Portions Copyright (c) 1995-1998 by Trusted Information Systems, Inc.
 *
 * Permission to use, copy modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND TRUSTED INFORMATION SYSTEMS
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL
 * TRUSTED INFORMATION SYSTEMS BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THE SOFTWARE.
 */

#include "port_before.h"

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include "dst_internal.h"

#include "port_after.h"
#ifdef NO_GETPASS
static int dst_s_ttyecho(const int fd, const int echo);
#endif

/*
 * dst_s_conv_bignum_u8_to_b64
 *	This function converts binary data stored as a u_char[] to a
 *	base-64 string.  Leading zeroes are discarded.  If a header is
 *	supplied, it is prefixed to the input prior to encoding.  The
 *	output is \n\0 terminated (the \0 is not included in output length).
 * Parameters
 *     out_buf   binary data to convert
 *     header    character string to prefix to the output (label)
 *     bin_data  binary data
 *     bin_len   size of binary data
 * Return
 *     -1     not enough space in output work area
 *	0     no output
 *     >0     number of bytes written to output work area
 */

int
dst_s_conv_bignum_u8_to_b64(char *out_buf, const int out_len,
			    const char *header, const u_char *bin_data,
			    const int bin_len)
{
	const u_char *bp = bin_data;
	char *op = out_buf;
	int lenh = 0, len64 = 0;
	int local_in_len = bin_len;
	int local_out_len = out_len;

	if (bin_data == NULL || bin_len <= 0)	/* no data no */
		return (0);

	if (out_buf == NULL || out_len <= 0)	/* no output_work area */
		return (-1);

	/* suppress leading \0  */
	for (; (*bp == 0x0) && (local_in_len > 0); local_in_len--)
		bp++;

	if (header) {		/* add header to output string */
		lenh = strlen(header);
		if (lenh < out_len)
			memcpy(op, header, lenh);
		else
			return (-1);
		local_out_len -= lenh;
		op += lenh;
	}
	len64 = b64_ntop(bp, local_in_len, op, local_out_len - 2);
	if (len64 < 0)
		return (-1);
	op += len64++;
	*(op++) = '\n';		/* put CR in the output */
	*op = '\0';		/* make sure output is 0 terminated */
	return (lenh + len64);
}


/*
 * dst_s_verify_str()
 *     Validate that the input string(*str) is at the head of the input
 *     buffer(**buf).  If so, move the buffer head pointer (*buf) to
 *     the first byte of data following the string(*str).
 * Parameters
 *     buf     Input buffer.
 *     str     Input string.
 * Return
 *	0       *str is not the head of **buff
 *	1       *str is the head of **buff, *buf is is advanced to
 *	the tail of **buf.
 */

int
dst_s_verify_str(const char **buf, const char *str)
{
	int b, s;
	if (*buf == NULL)	/* error checks */
		return (0);
	if (str == NULL || *str == '\0')
		return (1);

	b = strlen(*buf);	/* get length of strings */
	s = strlen(str);
	if (s > b || strncmp(*buf, str, s))	/* check if same */
		return (0);	/* not a match */
	(*buf) += s;		/* advance pointer */
	return (1);
}


/*
 * dst_s_conv_bignum_b64_to_u8
 *     Read a line of base-64 encoded string from the input buffer,
 *     convert it to binary, and store it in an output area.  The
 *     input buffer is read until reaching a newline marker or the
 *     end of the buffer.  The binary data is stored in the last X
 *     number of bytes of the output area where X is the size of the
 *     binary output.  If the operation is successful, the input buffer
 *     pointer is advanced.  This procedure does not do network to host
 *     byte order conversion.
 * Parameters
 *     buf     Pointer to encoded input string. Pointer is updated if
 *	   function is successfull.
 *     loc     Output area.
 *     loclen  Size in bytes of output area.
 * Return
 *	>0      Return = number of bytes of binary data stored in loc.
 *	 0      Failure.
 */

int
dst_s_conv_bignum_b64_to_u8(const char **buf, u_char *loc, const int loclen)
{
	int blen;
	char *bp;
	u_char bstr[RAW_KEY_SIZE];

	if (buf == NULL || *buf == NULL) {	/* error checks */
		EREPORT(("dst_s_conv_bignum_b64_to_u8: null input buffer.\n"));
		return (0);
	}
	bp = strchr(*buf, '\n');	/* find length of input line */
	if (bp != NULL)
		*bp = (u_char) NULL;

	blen = b64_pton(*buf, bstr, sizeof(bstr));
	if (blen <= 0) {
		EREPORT(("dst_s_conv_bignum_b64_to_u8: decoded value is null.\n"));
		return (0);
	}
	else if (loclen < blen) {
		EREPORT(("dst_s_conv_bignum_b64_to_u8: decoded value is longer than output buffer.\n"));
		return (0);
	}
	if (bp)
		*buf = bp;	/* advancing buffer past \n */
	memset(loc, 0, loclen - blen);	/* clearing unused output area */
	memcpy(loc + loclen - blen, bstr, blen);	/* write last blen bytes  */
	return (blen);
}


/*
 * dst_s_calculate_bits
 *     Given a binary number represented in a u_char[], determine
 *     the number of significant bits used.
 * Parameters
 *     str       An input character string containing a binary number.
 *     max_bits The maximum possible significant bits.
 * Return
 *     N       The number of significant bits in str.
 */

int
dst_s_calculate_bits(const u_char *str, const int max_bits)
{
	const u_char *p = str;
	u_char i, j = 0x80;
	int bits;
	for (bits = max_bits; *p == 0x00 && bits > 0; p++)
		bits -= 8;
	for (i = *p; (i & j) != j; j >>= 1)
		bits--;
	return (bits);
}


/*
 * calculates a checksum used in kmt for a id.
 * takes an array of bytes and a length.
 * returns a 16  bit checksum.
 */
u_int16_t
dst_s_id_calc(const u_char *key, const int keysize)
{
	u_int32_t ac;
	const u_char *kp = key;
	int size = keysize;

	if (!key || (keysize <= 0))
		return (-1);
 
	for (ac = 0; size > 1; size -= 2, kp += 2)
		ac += ((*kp) << 8) + *(kp + 1);

	if (size > 0)
		ac += ((*kp) << 8);
	ac += (ac >> 16) & 0xffff;

	return (ac & 0xffff);
}

/* 
 * dst_s_dns_key_id() Function to calculated DNSSEC footprint from KEY reocrd
 *   rdata (all of  record)
 * Input:
 *	dns_key_rdata: the raw data in wire format 
 *      rdata_len: the size of the input data 
 * Output:
 *      the key footprint/id calcuated from the key data 
 */ 
u_int16_t
dst_s_dns_key_id(const u_char *dns_key_rdata, const int rdata_len)
{
	int key_data = 4;

	if (!dns_key_rdata || (rdata_len < key_data))
		return 0;

	/* check the extended parameters bit in the DNS Key RR flags */
	if (dst_s_get_int16(dns_key_rdata) & DST_EXTEND_FLAG)
		key_data += 2;

	/* compute id */
	if (dns_key_rdata[3] == KEY_RSA)	/* Algorithm RSA */
		return dst_s_get_int16((u_char *)
				       &dns_key_rdata[rdata_len - 3]);
	else
		/* compute a checksum on the key part of the key rr */
		return dst_s_id_calc(&dns_key_rdata[key_data],
				     (rdata_len - key_data));
}

static int parity[256] =
{
	128, 1, 2, 131, 4, 133, 134, 7, 8, 137, 138, 11, 140, 13,
	14, 143, 16, 145, 146, 19, 148, 21, 22, 151, 152, 25, 26, 155,
	28, 157, 158, 31, 32, 161, 162, 35, 164, 37, 38, 167, 168, 41,
	42, 171, 44, 173, 174, 47, 176, 49, 50, 179, 52, 181, 182, 55,
	56, 185, 186, 59, 188, 61, 62, 191, 64, 193, 194, 67, 196, 69,
	70, 199, 200, 73, 74, 203, 76, 205, 206, 79, 208, 81, 82, 211,
	84, 213, 214, 87, 88, 217, 218, 91, 220, 93, 94, 223, 224, 97,
	98, 227, 100, 229, 230, 103, 104, 233, 234, 107, 236, 109, 110, 239,
	112, 241, 242, 115, 244, 117, 118, 247, 248, 121, 122, 251, 124, 253,
	254, 127, 128, 1, 2, 131, 4, 133, 134, 7, 8, 137, 138, 11,
	140, 13, 14, 143, 16, 145, 146, 19, 148, 21, 22, 151, 152, 25,
	26, 155, 28, 157, 158, 31, 32, 161, 162, 35, 164, 37, 38, 167,
	168, 41, 42, 171, 44, 173, 174, 47, 176, 49, 50, 179, 52, 181,
	182, 55, 56, 185, 186, 59, 188, 61, 62, 191, 64, 193, 194, 67,
	196, 69, 70, 199, 200, 73, 74, 203, 76, 205, 206, 79, 208, 81,
	82, 211, 84, 213, 214, 87, 88, 217, 218, 91, 220, 93, 94, 223,
	224, 97, 98, 227, 100, 229, 230, 103, 104, 233, 234, 107, 236, 109,
	110, 239, 112, 241, 242, 115, 244, 117, 118, 247, 248, 121, 122, 251,
	124, 253, 254, 127};


/*
 * dst_s_get_int16
 *     This routine extracts a 16 bit integer from a two byte character
 *     string.  The character string is assumed to be in network byte
 *     order and may be unaligned.  The number returned is in host order.
 * Parameter
 *     buf     A two byte character string.
 * Return
 *     The converted integer value.
 */

u_int16_t
dst_s_get_int16(const u_char *buf)
{
	register u_int16_t a = 0;
	a = ((u_int16_t)(buf[0] << 8)) | ((u_int16_t)(buf[1]));
	return (a);
}


/*
 * dst_s_get_int32
 *     This routine extracts a 32 bit integer from a four byte character
 *     string.  The character string is assumed to be in network byte
 *     order and may be unaligned.  The number returned is in host order.
 * Parameter
 *     buf     A four byte character string.
 * Return
 *     The converted integer value.
 */

u_int32_t
dst_s_get_int32(const u_char *buf)
{
	register u_int32_t a = 0;
	a = ((u_int32_t)(buf[0] << 24)) | ((u_int32_t)(buf[1] << 16)) |
		((u_int32_t)(buf[2] << 8)) | ((u_int32_t)(buf[3]));
	return (a);
}


/*
 * dst_s_put_int16
 *     Take a 16 bit integer and store the value in a two byte
 *     character string.  The integer is assumed to be in network
 *     order and the string is returned in host order.
 *
 * Parameters
 *     buf     Storage for a two byte character string.
 *     val     16 bit integer.
 */

void
dst_s_put_int16(u_int8_t *buf, const u_int16_t val)
{
	buf[0] = (u_int8_t)(val >> 8);
	buf[1] = (u_int8_t)(val);
}


/*
 * dst_s_put_int32
 *     Take a 32 bit integer and store the value in a four byte
 *     character string.  The integer is assumed to be in network
 *     order and the string is returned in host order.
 *
 * Parameters
 *     buf     Storage for a four byte character string.
 *     val     32 bit integer.
 */

void
dst_s_put_int32(u_int8_t *buf, const u_int32_t val)
{
	buf[0] = (u_int8_t)(val >> 24);
	buf[1] = (u_int8_t)(val >> 16);
	buf[2] = (u_int8_t)(val >> 8);
	buf[3] = (u_int8_t)(val);
}


/*
 *  dst_s_filename_length
 *
 *	This function returns the number of bytes needed to hold the
 *	filename for a key file.  '/', '\' and ':' are not allowed.
 *	form:  K<keyname>+<alg>+<id>.<suffix>
 *
 *	Returns 0 if the filename would contain either '\', '/' or ':'
 */
size_t
dst_s_filename_length(const char *name, const char *suffix)
{
	if (name == NULL)
		return (0);
	if (strrchr(name, '\\'))
		return (0);
	if (strrchr(name, '/'))
		return (0);
	if (strrchr(name, ':'))
		return (0);
	if (suffix == NULL)
		return (0);
	if (strrchr(suffix, '\\'))
		return (0);
	if (strrchr(suffix, '/'))
		return (0);
	if (strrchr(suffix, ':'))
		return (0);
	return (1 + strlen(name) + 6 + strlen(suffix));
}


/*
 *  dst_s_build_filename ()
 *	Builds a key filename from the key name, it's id, and a
 *	suffix.  '\', '/' and ':' are not allowed. fA filename is of the
 *	form:  K<keyname><id>.<suffix>
 *	form: K<keyname>+<alg>+<id>.<suffix>
 *
 *	Returns -1 if the conversion fails:
 *	  if the filename would be too long for space allotted
 *	  if the filename would contain a '\', '/' or ':'
 *	Returns 0 on success
 */

int
dst_s_build_filename(char *filename, const char *name, u_int16_t id,
		     int alg, const char *suffix, size_t filename_length)
{
	u_int32_t my_id;
	if (filename == NULL)
		return (-1);
	memset(filename, 0, filename_length);
	if (name == NULL)
		return (-1);
	if (suffix == NULL)
		return (-1);
	if (filename_length < 1 + strlen(name) + 4 + 6 + 1 + strlen(suffix))
		return (-1);
	my_id = id;
	sprintf(filename, "K%s+%03d+%05d.%s", name, alg, my_id,
		(char *) suffix);
	if (strrchr(filename, '/'))
		return (-1);
	if (strrchr(filename, '\\'))
		return (-1);
	if (strrchr(filename, ':'))
		return (-1);
	return (0);
}

/*
 *  dst_s_fopen ()
 *     Open a file in the dst_path directory.  If perm is specified, the
 *     file is checked for existence first, and not opened if it exists.
 *  Parameters
 *     filename  File to open
 *     mode       Mode to open the file (passed directly to fopen)
 *     perm       File permission, if creating a new file.
 *  Returns
 *     NULL       Failure
 *     NON-NULL  (FILE *) of opened file.
 */
FILE *
dst_s_fopen(const char *filename, const char *mode, int perm)
{
	FILE *fp;
	char pathname[PATH_MAX];
	int plen = sizeof(pathname);

	if (*dst_path != '\0') {
		strcpy(pathname, dst_path);
		plen -= strlen(pathname);
	}
	else 
		pathname[0] = '\0';

	if (plen > strlen(filename))
		strncpy(&pathname[PATH_MAX - plen], filename, plen-1);
	else 
		return (NULL);
	
	fp = fopen(pathname, mode);
	if (perm)
		chmod(pathname, perm);
	return (fp);
}

void
dst_s_dump(const int mode, const u_char *data, const int size, 
	    const char *msg)
{
	if (size > 0) {
#ifdef LONG_TEST
		static u_char scratch[1000];
		int n ;
		n = b64_ntop(data, scratch, size, sizeof(scratch));
		printf("%s: %x %d %s\n", msg, mode, n, scratch);
#else
		printf("%s,%x %d\n", msg, mode, size);
#endif
	}
}
