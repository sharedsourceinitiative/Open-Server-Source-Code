/*
**  GNU Pth - The GNU Portable Threads
**  Copyright (c) 1999-2003 Ralf S. Engelschall <rse@engelschall.com>
**
**  This file is part of GNU Pth, a non-preemptive thread scheduling
**  library which can be found at http://www.gnu.org/software/pth/.
**
**  This library is free software; you can redistribute it and/or
**  modify it under the terms of the GNU Lesser General Public
**  License as published by the Free Software Foundation; either
**  version 2.1 of the License, or (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**  Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License along with this library; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
**  USA, or contact Ralf S. Engelschall <rse@engelschall.com>.
**
**  pth_string.c: Pth replacement string functions
*/
                              /* ``A new release is where old bad
                                   assumptions are replaced by new
                                   bad assumptions.''               */

/*
 * Copyright Patrick Powell 1995
 * This code is based on code written by Patrick Powell <papowell@astart.com>
 * It may be used for any purpose as long as this notice remains intact
 * on all source code distributions.
 */

/*
 * This code contains numerious changes and enhancements which were
 * made by lots of contributors over the last years to Patrick Powell's
 * original code:
 *
 * o Patrick Powell <papowell@astart.com>      (1995)
 * o Brandon Long <blong@fiction.net>          (1996, for Mutt)
 * o Thomas Roessler <roessler@guug.de>        (1998, for Mutt)
 * o Michael Elkins <me@cs.hmc.edu>            (1998, for Mutt)
 * o Andrew Tridgell <tridge@samba.org>        (1998, for Samba)
 * o Luke Mewburn <lukem@netbsd.org>           (1999, for LukemFTP)
 * o Ralf S. Engelschall <rse@engelschall.com> (1999, for OSSP)
 */

#include "pth_p.h"

#if HAVE_LONGLONG
#define LLONG long long
#else
#define LLONG long
#endif

#if HAVE_LONGDOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif

static void fmtstr     (char *, size_t *, size_t, char *, int, int, int);
static void fmtint     (char *, size_t *, size_t, LLONG, int, int, int, int);
static void fmtfp      (char *, size_t *, size_t, LDOUBLE, int, int, int);
static void dopr_outch (char *, size_t *, size_t, int);

/* format read states */
#define DP_S_DEFAULT    0
#define DP_S_FLAGS      1
#define DP_S_MIN        2
#define DP_S_DOT        3
#define DP_S_MAX        4
#define DP_S_MOD        5
#define DP_S_CONV       6
#define DP_S_DONE       7

/* format flags - Bits */
#define DP_F_MINUS      (1 << 0)
#define DP_F_PLUS       (1 << 1)
#define DP_F_SPACE      (1 << 2)
#define DP_F_NUM        (1 << 3)
#define DP_F_ZERO       (1 << 4)
#define DP_F_UP         (1 << 5)
#define DP_F_UNSIGNED   (1 << 6)

/* conversion flags */
#define DP_C_SHORT      1
#define DP_C_LONG       2
#define DP_C_LDOUBLE    3
#define DP_C_LLONG      4

/* some handy macros */
#define char_to_int(p) (p - '0')
#define MAX(p,q) ((p >= q) ? p : q)
#define NUL '\0'

static void
dopr(
    char *buffer,
    size_t maxlen,
    size_t *retlen,
    const char *format,
    va_list args)
{
    char ch;
    LLONG value;
    LDOUBLE fvalue;
    char *strvalue;
    int min;
    int max;
    int state;
    int flags;
    int cflags;
    size_t currlen;

    state = DP_S_DEFAULT;
    flags = currlen = cflags = min = 0;
    max = -1;
    ch = *format++;

    if (maxlen == -1)
        /* possible maximum size in a size_t */
        maxlen = (~(1<<((sizeof(size_t)*8)-2)));

    while (state != DP_S_DONE) {
        if ((ch == NUL) || (currlen >= maxlen))
            state = DP_S_DONE;

        switch (state) {
        case DP_S_DEFAULT:
            if (ch == '%')
                state = DP_S_FLAGS;
            else
                dopr_outch(buffer, &currlen, maxlen, ch);
            ch = *format++;
            break;
        case DP_S_FLAGS:
            switch (ch) {
                case '-':
                    flags |= DP_F_MINUS;
                    ch = *format++;
                    break;
                case '+':
                    flags |= DP_F_PLUS;
                    ch = *format++;
                    break;
                case ' ':
                    flags |= DP_F_SPACE;
                    ch = *format++;
                    break;
                case '#':
                    flags |= DP_F_NUM;
                    ch = *format++;
                    break;
                case '0':
                    flags |= DP_F_ZERO;
                    ch = *format++;
                    break;
                default:
                    state = DP_S_MIN;
                    break;
            }
            break;
        case DP_S_MIN:
            if (isdigit((unsigned char)ch)) {
                min = 10 * min + char_to_int(ch);
                ch = *format++;
            } else if (ch == '*') {
                min = va_arg(args, int);
                ch = *format++;
                state = DP_S_DOT;
            } else
                state = DP_S_DOT;
            break;
        case DP_S_DOT:
            if (ch == '.') {
                state = DP_S_MAX;
                ch = *format++;
            } else
                state = DP_S_MOD;
            break;
        case DP_S_MAX:
            if (isdigit((unsigned char)ch)) {
                if (max < 0)
                    max = 0;
                max = 10 * max + char_to_int(ch);
                ch = *format++;
            } else if (ch == '*') {
                max = va_arg(args, int);
                ch = *format++;
                state = DP_S_MOD;
            } else
                state = DP_S_MOD;
            break;
        case DP_S_MOD:
            switch (ch) {
                case 'h':
                    cflags = DP_C_SHORT;
                    ch = *format++;
                    break;
                case 'l':
                    if (*format == 'l') {
                        cflags = DP_C_LLONG;
                        format++;
                    } else
                        cflags = DP_C_LONG;
                    ch = *format++;
                    break;
                case 'q':
                    cflags = DP_C_LLONG;
                    ch = *format++;
                    break;
                case 'L':
                    cflags = DP_C_LDOUBLE;
                    ch = *format++;
                    break;
                default:
                    break;
            }
            state = DP_S_CONV;
            break;
        case DP_S_CONV:
            switch (ch) {
            case 'd':
            case 'i':
                switch (cflags) {
                case DP_C_SHORT:
                    value = (short int)va_arg(args, int);
                    break;
                case DP_C_LONG:
                    value = va_arg(args, long int);
                    break;
                case DP_C_LLONG:
                    value = va_arg(args, LLONG);
                    break;
                default:
                    value = va_arg(args, int);
                    break;
                }
                fmtint(buffer, &currlen, maxlen, value, 10, min, max, flags);
                break;
            case 'X':
                flags |= DP_F_UP;
                /* FALLTHROUGH */
            case 'x':
            case 'o':
            case 'u':
                flags |= DP_F_UNSIGNED;
                switch (cflags) {
                    case DP_C_SHORT:
                        value = (unsigned short int)va_arg(args, unsigned int);
                        break;
                    case DP_C_LONG:
                        value = (LLONG)va_arg(args, unsigned long int);
                        break;
                    case DP_C_LLONG:
                        value = va_arg(args, unsigned LLONG);
                        break;
                    default:
                        value = (LLONG)va_arg(args, unsigned int);
                        break;
                }
                fmtint(buffer, &currlen, maxlen, value,
                       ch == 'o' ? 8 : (ch == 'u' ? 10 : 16),
                       min, max, flags);
                break;
            case 'f':
                if (cflags == DP_C_LDOUBLE)
                    fvalue = va_arg(args, LDOUBLE);
                else
                    fvalue = va_arg(args, double);
                fmtfp(buffer, &currlen, maxlen, fvalue, min, max, flags);
                break;
            case 'E':
                flags |= DP_F_UP;
            case 'e':
                if (cflags == DP_C_LDOUBLE)
                    fvalue = va_arg(args, LDOUBLE);
                else
                    fvalue = va_arg(args, double);
                break;
            case 'G':
                flags |= DP_F_UP;
            case 'g':
                if (cflags == DP_C_LDOUBLE)
                    fvalue = va_arg(args, LDOUBLE);
                else
                    fvalue = va_arg(args, double);
                break;
            case 'c':
                dopr_outch(buffer, &currlen, maxlen, va_arg(args, int));
                break;
            case 's':
                strvalue = va_arg(args, char *);
                if (max < 0)
                    max = maxlen;
                fmtstr(buffer, &currlen, maxlen, strvalue, flags, min, max);
                break;
            case 'p':
                value = (long)va_arg(args, void *);
                fmtint(buffer, &currlen, maxlen, value, 16, min, max, flags);
                break;
            case 'n': /* XXX */
                if (cflags == DP_C_SHORT) {
                    short int *num;
                    num = va_arg(args, short int *);
                    *num = currlen;
                } else if (cflags == DP_C_LONG) { /* XXX */
                    long int *num;
                    num = va_arg(args, long int *);
                    *num = (long int) currlen;
                } else if (cflags == DP_C_LLONG) { /* XXX */
                    LLONG *num;
                    num = va_arg(args, LLONG *);
                    *num = (LLONG) currlen;
                } else {
                    int    *num;
                    num = va_arg(args, int *);
                    *num = currlen;
                }
                break;
            case '%':
                dopr_outch(buffer, &currlen, maxlen, ch);
                break;
            case 'w':
                /* not supported yet, treat as next char */
                ch = *format++;
                break;
            default:
                /* unknown, skip */
                break;
            }
            ch = *format++;
            state = DP_S_DEFAULT;
            flags = cflags = min = 0;
            max = -1;
            break;
        case DP_S_DONE:
            break;
        default:
            break;
        }
    }
    if (currlen >= maxlen - 1)
        currlen = maxlen - 1;
    if (buffer != NULL)
        buffer[currlen] = NUL;
    *retlen = currlen;
    return;
}

static void
fmtstr(
    char *buffer,
    size_t *currlen,
    size_t maxlen,
    char *value,
    int flags,
    int min,
    int max)
{
    int padlen, strln;
    int cnt = 0;

    if (value == 0)
        value = "<NULL>";
    for (strln = 0; value[strln] != '\0'; strln++)
        ;
    padlen = min - strln;
    if (padlen < 0)
        padlen = 0;
    if (flags & DP_F_MINUS)
        padlen = -padlen;

    while ((padlen > 0) && (cnt < max)) {
        dopr_outch(buffer, currlen, maxlen, ' ');
        --padlen;
        ++cnt;
    }
    while (*value && (cnt < max)) {
        dopr_outch(buffer, currlen, maxlen, *value++);
        ++cnt;
    }
    while ((padlen < 0) && (cnt < max)) {
        dopr_outch(buffer, currlen, maxlen, ' ');
        ++padlen;
        ++cnt;
    }
}

static void
fmtint(
    char *buffer,
    size_t *currlen,
    size_t maxlen,
    LLONG value,
    int base,
    int min,
    int max,
    int flags)
{
    int signvalue = 0;
    unsigned LLONG uvalue;
    char convert[20];
    int place = 0;
    int spadlen = 0;
    int zpadlen = 0;
    int caps = 0;

    if (max < 0)
        max = 0;
    uvalue = value;
    if (!(flags & DP_F_UNSIGNED)) {
        if (value < 0) {
            signvalue = '-';
            uvalue = -value;
        } else if (flags & DP_F_PLUS)
            signvalue = '+';
        else if (flags & DP_F_SPACE)
            signvalue = ' ';
    }
    if (flags & DP_F_UP)
        caps = 1;
    do {
        convert[place++] =
            (caps ? "0123456789ABCDEF" : "0123456789abcdef")
            [uvalue % (unsigned) base];
        uvalue = (uvalue / (unsigned) base);
    } while (uvalue && (place < 20));
    if (place == 20)
        place--;
    convert[place] = 0;

    zpadlen = max - place;
    spadlen = min - MAX(max, place) - (signvalue ? 1 : 0);
    if (zpadlen < 0)
        zpadlen = 0;
    if (spadlen < 0)
        spadlen = 0;
    if (flags & DP_F_ZERO) {
        zpadlen = MAX(zpadlen, spadlen);
        spadlen = 0;
    }
    if (flags & DP_F_MINUS)
        spadlen = -spadlen;

    /* spaces */
    while (spadlen > 0) {
        dopr_outch(buffer, currlen, maxlen, ' ');
        --spadlen;
    }

    /* sign */
    if (signvalue)
        dopr_outch(buffer, currlen, maxlen, signvalue);

    /* zeros */
    if (zpadlen > 0) {
        while (zpadlen > 0) {
            dopr_outch(buffer, currlen, maxlen, '0');
            --zpadlen;
        }
    }
    /* digits */
    while (place > 0)
        dopr_outch(buffer, currlen, maxlen, convert[--place]);

    /* left justified spaces */
    while (spadlen < 0) {
        dopr_outch(buffer, currlen, maxlen, ' ');
        ++spadlen;
    }
    return;
}

static LDOUBLE
abs_val(LDOUBLE value)
{
    LDOUBLE result = value;
    if (value < 0)
        result = -value;
    return result;
}

static LDOUBLE
pow10(int exp)
{
    LDOUBLE result = 1;
    while (exp) {
        result *= 10;
        exp--;
    }
    return result;
}

static long
round(LDOUBLE value)
{
    long intpart;
    intpart = (long) value;
    value = value - intpart;
    if (value >= 0.5)
        intpart++;
    return intpart;
}

static void
fmtfp(
    char *buffer,
    size_t *currlen,
    size_t maxlen,
    LDOUBLE fvalue,
    int min,
    int max,
    int flags)
{
    int signvalue = 0;
    LDOUBLE ufvalue;
    char iconvert[20];
    char fconvert[20];
    int iplace = 0;
    int fplace = 0;
    int padlen = 0;
    int zpadlen = 0;
    int caps = 0;
    long intpart;
    long fracpart;

    if (max < 0)
        max = 6;
    ufvalue = abs_val(fvalue);
    if (fvalue < 0)
        signvalue = '-';
    else if (flags & DP_F_PLUS)
        signvalue = '+';
    else if (flags & DP_F_SPACE)
        signvalue = ' ';

    intpart = (long)ufvalue;

    /* sorry, we only support 9 digits past the decimal because of our
       conversion method */
    if (max > 9)
        max = 9;

    /* we "cheat" by converting the fractional part to integer by
       multiplying by a factor of 10 */
    fracpart = round((pow10(max)) * (ufvalue - intpart));

    if (fracpart >= pow10(max)) {
        intpart++;
        fracpart -= pow10(max);
    }

    /* convert integer part */
    do {
        iconvert[iplace++] =
            (caps ? "0123456789ABCDEF"
              : "0123456789abcdef")[intpart % 10];
        intpart = (intpart / 10);
    } while (intpart && (iplace < 20));
    if (iplace == 20)
        iplace--;
    iconvert[iplace] = 0;

    /* convert fractional part */
    do {
        fconvert[fplace++] =
            (caps ? "0123456789ABCDEF"
              : "0123456789abcdef")[fracpart % 10];
        fracpart = (fracpart / 10);
    } while (fracpart && (fplace < 20));
    if (fplace == 20)
        fplace--;
    fconvert[fplace] = 0;

    /* -1 for decimal point, another -1 if we are printing a sign */
    padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0);
    zpadlen = max - fplace;
    if (zpadlen < 0)
        zpadlen = 0;
    if (padlen < 0)
        padlen = 0;
    if (flags & DP_F_MINUS)
        padlen = -padlen;

    if ((flags & DP_F_ZERO) && (padlen > 0)) {
        if (signvalue) {
            dopr_outch(buffer, currlen, maxlen, signvalue);
            --padlen;
            signvalue = 0;
        }
        while (padlen > 0) {
            dopr_outch(buffer, currlen, maxlen, '0');
            --padlen;
        }
    }
    while (padlen > 0) {
        dopr_outch(buffer, currlen, maxlen, ' ');
        --padlen;
    }
    if (signvalue)
        dopr_outch(buffer, currlen, maxlen, signvalue);

    while (iplace > 0)
        dopr_outch(buffer, currlen, maxlen, iconvert[--iplace]);

    /*
     * Decimal point. This should probably use locale to find the correct
     * char to print out.
     */
    if (max > 0) {
        dopr_outch(buffer, currlen, maxlen, '.');

        while (fplace > 0)
            dopr_outch(buffer, currlen, maxlen, fconvert[--fplace]);
    }
    while (zpadlen > 0) {
        dopr_outch(buffer, currlen, maxlen, '0');
        --zpadlen;
    }

    while (padlen < 0) {
        dopr_outch(buffer, currlen, maxlen, ' ');
        ++padlen;
    }
    return;
}

static void
dopr_outch(
    char *buffer,
    size_t *currlen,
    size_t maxlen,
    int c)
{
    if (*currlen < maxlen) {
        if (buffer != NULL)
            buffer[(*currlen)] = (char)c;
        (*currlen)++;
    }
    return;
}

intern int
pth_vsnprintf(
    char *str,
    size_t count,
    const char *fmt,
    va_list args)
{
    size_t retlen;

    if (str != NULL)
        str[0] = NUL;
    dopr(str, count, &retlen, fmt, args);
    return retlen;
}

intern int
pth_snprintf(
    char *str,
    size_t count,
    const char *fmt,
    ...)
{
    va_list ap;
    int rv;

    va_start(ap, fmt);
    rv = pth_vsnprintf(str, count, fmt, ap);
    va_end(ap);
    return rv;
}

intern char *
pth_vasprintf(
    const char *fmt,
    va_list ap)
{
    char *rv;
    int n;

    n = pth_vsnprintf(NULL, -1, fmt, ap);
    if ((rv = (char *)malloc(n+1)) == NULL)
        return NULL;
    pth_vsnprintf(rv, n+1, fmt, ap);
    return rv;
}

intern char *
pth_asprintf(
    const char *fmt,
    ...)
{
    va_list ap;
    char *rv;

    va_start(ap, fmt);
    rv = pth_vasprintf(fmt, ap);
    va_end(ap);
    return rv;
}

