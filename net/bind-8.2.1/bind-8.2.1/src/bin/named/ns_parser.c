#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93 (BSDI)";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYEMPTY (-1)
#define YYLEX yylex()
#define yyclearin (yychar=YYEMPTY)
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 2 "ns_parser.y"
#if !defined(lint) && !defined(SABER)
static char rcsid[] = "$Id: ns_parser.y,v 8.38 1999/06/18 03:03:02 vixie Exp $";
#endif /* not lint */

/*
 * Copyright (c) 1996-1999 by Internet Software Consortium.
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

/* Global C stuff goes here. */

#include "port_before.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>

#include <isc/eventlib.h>
#include <isc/logging.h>

#include <isc/dst.h>

#include "port_after.h"

#include "named.h"
#include "ns_parseutil.h"
#include "ns_lexer.h"

#define SYM_ZONE	0x010000
#define SYM_SERVER	0x020000
#define SYM_KEY		0x030000
#define SYM_ACL		0x040000
#define SYM_CHANNEL	0x050000
#define SYM_PORT	0x060000

#define SYMBOL_TABLE_SIZE 29989		/* should always be prime */
static symbol_table symtab;

#define AUTH_TABLE_SIZE 397		/* should always be prime */
static symbol_table authtab = NULL;

static zone_config current_zone;
static int should_install;

static options current_options;
static int seen_options;

static controls current_controls;

static topology_config current_topology;
static int seen_topology;

static server_config current_server;
static int seen_server;

static char *current_algorithm;
static char *current_secret;

static log_config current_logging;
static int current_category;
static int chan_type;
static int chan_level;
static u_int chan_flags;
static int chan_facility;
static char *chan_name;
static int chan_versions;
static u_long chan_max_size;

static log_channel lookup_channel(char *);
static void define_channel(char *, log_channel);
static char *canonical_name(char *);

extern const struct res_sym __p_type_syms[];

 int yyparse();
	
#line 104 "ns_parser.y"
typedef union {
	char *			cp;
	int			s_int;
	long			num;
	u_long			ul_int;
	u_int16_t		us_int;
	struct in_addr		ip_addr;
	ip_match_element	ime;
	ip_match_list		iml;
	rrset_order_list	rol;
	rrset_order_element	roe;
	struct dst_key *	keyi;
	enum axfr_format	axfr_fmt;
} YYSTYPE;
#line 131 "y.tab.c"
#define L_EOS 257
#define L_IPADDR 258
#define L_NUMBER 259
#define L_STRING 260
#define L_QSTRING 261
#define L_END_INCLUDE 262
#define T_INCLUDE 263
#define T_OPTIONS 264
#define T_DIRECTORY 265
#define T_PIDFILE 266
#define T_NAMED_XFER 267
#define T_DUMP_FILE 268
#define T_STATS_FILE 269
#define T_MEMSTATS_FILE 270
#define T_FAKE_IQUERY 271
#define T_RECURSION 272
#define T_FETCH_GLUE 273
#define T_QUERY_SOURCE 274
#define T_LISTEN_ON 275
#define T_PORT 276
#define T_ADDRESS 277
#define T_RRSET_ORDER 278
#define T_ORDER 279
#define T_NAME 280
#define T_CLASS 281
#define T_CONTROLS 282
#define T_INET 283
#define T_UNIX 284
#define T_PERM 285
#define T_OWNER 286
#define T_GROUP 287
#define T_ALLOW 288
#define T_DATASIZE 289
#define T_STACKSIZE 290
#define T_CORESIZE 291
#define T_DEFAULT 292
#define T_UNLIMITED 293
#define T_FILES 294
#define T_VERSION 295
#define T_HOSTSTATS 296
#define T_DEALLOC_ON_EXIT 297
#define T_TRANSFERS_IN 298
#define T_TRANSFERS_OUT 299
#define T_TRANSFERS_PER_NS 300
#define T_TRANSFER_FORMAT 301
#define T_MAX_TRANSFER_TIME_IN 302
#define T_ONE_ANSWER 303
#define T_MANY_ANSWERS 304
#define T_NOTIFY 305
#define T_AUTH_NXDOMAIN 306
#define T_MULTIPLE_CNAMES 307
#define T_USE_IXFR 308
#define T_MAINTAIN_IXFR_BASE 309
#define T_CLEAN_INTERVAL 310
#define T_INTERFACE_INTERVAL 311
#define T_STATS_INTERVAL 312
#define T_MAX_LOG_SIZE_IXFR 313
#define T_HEARTBEAT 314
#define T_USE_ID_POOL 315
#define T_MAX_NCACHE_TTL 316
#define T_HAS_OLD_CLIENTS 317
#define T_RFC2308_TYPE1 318
#define T_LAME_TTL 319
#define T_LOGGING 320
#define T_CATEGORY 321
#define T_CHANNEL 322
#define T_SEVERITY 323
#define T_DYNAMIC 324
#define T_FILE 325
#define T_VERSIONS 326
#define T_SIZE 327
#define T_SYSLOG 328
#define T_DEBUG 329
#define T_NULL_OUTPUT 330
#define T_PRINT_TIME 331
#define T_PRINT_CATEGORY 332
#define T_PRINT_SEVERITY 333
#define T_SORTLIST 334
#define T_TOPOLOGY 335
#define T_SERVER 336
#define T_LONG_AXFR 337
#define T_BOGUS 338
#define T_TRANSFERS 339
#define T_KEYS 340
#define T_SUPPORT_IXFR 341
#define T_ZONE 342
#define T_IN 343
#define T_CHAOS 344
#define T_HESIOD 345
#define T_TYPE 346
#define T_MASTER 347
#define T_SLAVE 348
#define T_STUB 349
#define T_RESPONSE 350
#define T_HINT 351
#define T_MASTERS 352
#define T_TRANSFER_SOURCE 353
#define T_PUBKEY 354
#define T_ALSO_NOTIFY 355
#define T_DIALUP 356
#define T_FILE_IXFR 357
#define T_IXFR_TMP 358
#define T_TRUSTED_KEYS 359
#define T_ACL 360
#define T_ALLOW_UPDATE 361
#define T_ALLOW_QUERY 362
#define T_ALLOW_TRANSFER 363
#define T_ALLOW_RECURSION 364
#define T_BLACKHOLE 365
#define T_SEC_KEY 366
#define T_ALGID 367
#define T_SECRET 368
#define T_CHECK_NAMES 369
#define T_WARN 370
#define T_FAIL 371
#define T_IGNORE 372
#define T_FORWARD 373
#define T_FORWARDERS 374
#define T_ONLY 375
#define T_FIRST 376
#define T_IF_NO_ANSWER 377
#define T_IF_NO_DOMAIN 378
#define T_YES 379
#define T_TRUE 380
#define T_NO 381
#define T_FALSE 382
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,   31,   31,   32,   32,   32,   32,   32,   32,   32,
   32,   32,   32,   32,   32,   33,   42,   34,   43,   43,
   44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
   44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
   44,   44,   44,   44,   46,   44,   44,   44,   44,   44,
   44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
   44,   44,   44,   44,   44,   44,   44,   35,   51,   51,
   52,   52,   52,   52,   15,   15,   12,   12,   13,   13,
   14,   14,   16,    6,    6,    5,    5,    4,    4,   53,
   54,   48,   48,   48,   48,    2,    2,    3,    3,   29,
   29,   29,   29,   29,   27,   27,   27,   28,   28,   28,
   45,   45,   45,   45,   49,   49,   49,   49,   26,   26,
   26,   26,   50,   50,   50,   47,   47,   55,   55,   56,
   57,   36,   58,   58,   58,   60,   59,   62,   59,   64,
   64,   64,   64,   65,   65,   66,   67,   67,   67,   67,
   67,   68,   10,   10,   11,   11,   69,   70,   70,   70,
   70,   70,   70,   70,   63,   63,   63,    9,    9,   71,
   61,   61,   61,    8,    8,    8,    7,   72,   37,   73,
   73,   74,   74,   74,   74,   74,   74,   20,   20,   18,
   18,   18,   17,   17,   17,   17,   17,   19,   23,   76,
   75,   75,   75,   77,   41,   78,   78,   78,   24,   25,
   40,   80,   38,   79,   79,   21,   21,   22,   22,   22,
   22,   22,   81,   81,   82,   82,   82,   82,   82,   82,
   82,   82,   82,   82,   82,   85,   82,   82,   82,   82,
   82,   82,   82,   82,   82,   83,   83,   88,   87,   87,
   89,   89,   90,   84,   84,   86,   86,   91,   91,   92,
   39,   93,   93,   94,    1,   30,   30,
};
short yylen[] = {                                         2,
    1,    1,    2,    1,    2,    2,    2,    2,    2,    2,
    2,    2,    1,    2,    2,    3,    0,    5,    2,    3,
    0,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    3,
    2,    2,    5,    2,    0,    5,    2,    4,    4,    4,
    4,    4,    4,    1,    1,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    4,    1,    4,    2,    3,
    0,    8,    8,    1,    2,    3,    0,    2,    0,    2,
    0,    2,    5,    1,    1,    1,    1,    1,    1,    2,
    2,    1,    1,    2,    2,    0,    2,    0,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    2,    2,    2,    1,    1,
    1,    1,    2,    2,    2,    0,    1,    2,    3,    1,
    0,    5,    2,    3,    1,    0,    6,    0,    6,    1,
    1,    2,    1,    2,    2,    2,    0,    1,    1,    2,
    2,    3,    1,    1,    0,    1,    2,    1,    1,    1,
    2,    2,    2,    2,    2,    3,    1,    1,    1,    1,
    2,    3,    1,    1,    1,    1,    1,    0,    6,    2,
    3,    2,    2,    2,    2,    4,    1,    2,    3,    1,
    2,    2,    1,    3,    3,    1,    3,    1,    1,    1,
    2,    3,    1,    0,    6,    2,    2,    1,    3,    3,
    5,    0,    5,    0,    3,    0,    1,    1,    1,    1,
    1,    1,    2,    3,    2,    2,    2,    2,    5,    2,
    2,    4,    4,    4,    2,    0,    5,    2,    2,    2,
    2,    5,    4,    2,    1,    2,    3,    1,    0,    1,
    2,    3,    1,    1,    1,    0,    1,    2,    3,    1,
    4,    2,    3,    5,    1,    1,    1,
};
short yydefred[] = {                                      0,
    0,   13,    0,   17,    0,  131,    0,    0,    0,    0,
  204,    0,    0,    2,    4,    0,    0,    0,    0,    0,
    0,    0,    0,   14,   15,    0,    0,    0,    0,  178,
    0,    0,  266,  267,    0,    0,    3,    5,    6,    7,
    8,    9,   10,   11,   12,   16,    0,   74,    0,    0,
    0,    0,    0,    0,  212,  217,    0,    0,    0,    0,
    0,   67,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   45,    0,    0,   54,   55,   86,   87,    0,    0,   68,
    0,   69,  135,    0,    0,    0,    0,    0,    0,    0,
  261,    0,  262,    0,    0,    0,    0,    0,  190,    0,
  196,    0,  198,    0,   23,   25,   24,   28,   26,   27,
  104,  100,  101,  102,  103,   29,   30,   31,    0,    0,
   47,    0,    0,    0,    0,    0,  120,  121,  122,  115,
  119,  116,  117,  118,   22,   33,   34,  123,  124,  125,
   84,   85,   56,   57,   32,   38,   39,   35,   36,   58,
   59,   60,   61,   64,   41,   62,   37,   42,   63,    0,
    0,   65,    0,    0,    0,    0,  105,  106,  107,    0,
  111,  112,  113,  114,   44,    0,   18,    0,   19,    0,
    0,   70,  175,  176,  136,  177,  174,  169,  138,  168,
  132,    0,  133,  187,    0,    0,    0,    0,    0,    0,
    0,    0,  213,    0,  263,    0,    0,  192,    0,  191,
  188,  211,    0,  208,    0,    0,    0,    0,    0,  265,
   89,   88,   91,   90,   94,   95,   97,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  108,  109,
  110,   40,    0,   20,    0,    0,    0,    0,  134,  185,
  182,  184,    0,  183,  179,    0,  180,  245,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  236,    0,    0,    0,
  194,  195,  197,  189,    0,    0,  206,  207,  205,    0,
   78,    0,    0,   66,    0,   75,   51,   53,   48,   50,
   49,   52,  130,    0,    0,    0,    0,    0,    0,    0,
  203,  200,  199,    0,    0,  181,  238,  240,  241,  239,
  226,  218,  219,  221,  220,  222,  225,    0,    0,  230,
    0,    0,  244,  227,  228,    0,    0,    0,  231,  254,
  255,  235,    0,  215,    0,  223,  264,  209,  210,   43,
   80,    0,    0,   76,   46,    0,  128,    0,    0,  173,
  170,    0,    0,  167,    0,    0,    0,  160,    0,    0,
    0,    0,  158,  159,    0,  186,    0,  201,   99,    0,
    0,  253,    0,    0,    0,    0,    0,    0,    0,  224,
   82,    0,  129,    0,    0,  137,    0,  171,  143,    0,
  140,  161,    0,  154,  156,  157,  153,  162,  163,  164,
  139,    0,  165,  202,  248,    0,    0,    0,  243,    0,
  251,  232,  233,  234,  260,    0,    0,    0,   83,   72,
   73,  172,  142,    0,    0,    0,    0,  152,  166,  229,
    0,  246,  242,  252,  237,    0,  258,  144,  145,  146,
  150,  151,  247,  259,
};
short yydgoto[] = {                                      12,
  262,  165,  369,  263,  118,  183,  225,  226,  401,  445,
  446,  270,  333,  393,  271,  272,  139,  140,  141,  142,
   55,  367,  352,  257,  258,  170,  210,  282,  156,  143,
   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
   23,   27,  112,  113,  215,  216,  344,  161,  114,  115,
   51,   52,  162,  163,  345,  346,   29,  126,  127,  287,
  402,  288,  412,  442,  476,  477,  478,  413,  414,  415,
  403,   54,  240,  241,  354,  355,   36,  259,  243,  129,
  318,  319,  456,  382,  383,  466,  423,  457,  424,  425,
  467,  468,   58,   59,
};
short yysindex[] = {                                    367,
 -149,    0, -243,    0, -103,    0, -230, -227,  -85, -104,
    0,    0,  367,    0,    0, -210, -204, -196, -194, -183,
 -166, -164, -162,    0,    0, -160,  -24, -197,  -18,    0,
 -104, -209,    0,    0,  -14, -104,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  299,    0,  -31, -145,
 -101, -139, -211,    8,    0,    0, -147, -109, -124,   52,
   12,    0, -107,  -99,  -97,  -95,  -93,  -87,  -48,  -48,
  -48,  -89,  -68,   14, -171, -171, -171, -171,  -70,  -48,
  -48,  -83,  -80,  -59,  -66,  -46,  -48,  -48,  -48,  -48,
  -48,  -38,  -15,   -3,    5,   25,  -48,   65,  -48,  -48,
   66,   37,   49,  -48,   95,  105,  118,  203, -151, -128,
    0,  189,   70,    0,    0,    0,    0,   54,   43,    0,
   78,    0,    0, -185, -224,  -82,   79, -237,  215,   81,
    0,   85,    0,  296,  300,   86,   52,   57,    0,   91,
    0,  -29,    0, -239,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -37,  -31,
    0,   74,   72,   98,  235,   80,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   52,
   52,    0,   52,   52,   52,   52,    0,    0,    0, -225,
    0,    0,    0,    0,    0,  236,    0,  106,    0,   98,
  110,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  107,    0,    0,  -66,  -48,  111,  248,  -48, -115,
  115,  323,    0,  117,    0,  120,  121,    0,  -25,    0,
    0,    0,  126,    0, -104, -104,    6,   17,  263,    0,
    0,    0,    0,    0,    0,    0,    0,   52, -104,   45,
 -111,  132,   -6,    2,    7,   11,   15,   19,    0,    0,
    0,    0,  136,    0,  102,  119,  273,  278,    0,    0,
    0,    0, -178,    0,    0,  146,    0,    0,  145,  -48,
  -48,  148,  147,  -28,  133,  -31,  152,  289,  -48,  154,
  155,  290,  294,  297, -225,  -94,    0,   73,  164,  161,
    0,    0,    0,    0,  166,  167,    0,    0,    0,   27,
    0, -104,  153,    0,  175,    0,    0,    0,    0,    0,
    0,    0,    0,  312,  136,  181,  316,  182, -206,  143,
    0,    0,    0,  -76,  183,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   98,  320,    0,
  185,  190,    0,    0,    0,   52,   52,   52,    0,    0,
    0,    0,  326,    0,  193,    0,    0,    0,    0,    0,
    0,  191,  172,    0,    0,  196,    0,   52,  194,    0,
    0,  -41,  212,    0, -181,  209, -189,    0,  -48,  -48,
  -48, -116,    0,    0,  220,    0,  225,    0,    0,  214,
  233,    0,  368,  190,  252,   36,   40,   44,  253,    0,
    0,  250,    0,   48,  254,    0,  255,    0,    0,  256,
    0,    0,  -36,    0,    0,    0,    0,    0,    0,    0,
    0,  257,    0,    0,    0, -100,  259,  258,    0,  260,
    0,    0,    0,    0,    0,  393,  253,  264,    0,    0,
    0,    0,    0, -201, -171,  195,  199,    0,    0,    0,
  269,    0,    0,    0,    0,  270,    0,    0,    0,    0,
    0,    0,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  520,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  271,    0,    0,
 -108,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  272,    0,    0,    0,
  271,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  272,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  274,    0,
    0,    0,    0,  275,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  276,  277,    0,    0, -223,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   13,
 -223,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  413,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  261,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  412,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  281,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  286,
    0,    0,  287,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  421,    0,    0,    0,
    0,    0,    0,    0,    0,  291,  292,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
 -158,    0,    0,    0, -153,  315,    0,    0,  422,    0,
    0,    0,    0,    0,    0,  285,  419,  -72,    0,  -11,
    0,    0,    0,  301,  303,  -75,    0,  246,  -58,  -10,
    0,  562,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  464,    0,    0,    0,    0,    0,    0,
    0,  527,  418,  420,    0,  238,    0,    0,  454,    0,
    0,    0,    0,    0,  108,  116,    0,    0,    0,  174,
  200,    0,    0,  344,    0,  237,    0,    0,    0,    0,
    0,  302,    0,    0,    0,    0,    0,  131,    0,  179,
    0,  157,    0,  561,
};
#define YYTABLESIZE 733
short yytable[] = {                                      35,
  172,  173,  174,  138,  261,  267,  264,  138,  451,  295,
  117,  157,  158,  334,  216,  131,  254,   26,  234,   28,
   56,  176,  177,  120,  480,   61,  138,   30,  185,  186,
  187,  188,  189,   31,  138,   33,   34,   32,  195,  138,
  197,  198,  231,  138,  123,  202,   38,  138,  416,  400,
   57,  138,   39,   33,   34,   77,   77,  488,   48,  138,
   40,  285,   41,  235,  171,  171,  171,  171,  138,  253,
   33,   34,  138,   42,   33,   34,  138,  351,   33,   34,
  138,   33,   34,  436,  138,   49,   50,  167,   33,   34,
   43,  489,   44,  137,   45,  252,   46,  137,   47,  323,
  236,  237,  238,  239,   53,  228,  223,   24,   60,  124,
  125,  130,   25,  227,  230,  119,  137,  122,  337,  224,
  168,  169,   77,  228,  137,  249,  338,  255,  256,  137,
  128,  339,  133,  137,  144,  340,  166,  137,  444,  341,
  234,  137,  439,  342,  279,  280,  281,  440,  216,  137,
   57,  390,  370,  145,   48,   33,   34,  455,  137,  200,
  462,  146,  137,  147,  463,  148,  137,  149,  464,  269,
  137,  201,  470,  150,  137,  178,  253,  291,  179,  137,
  294,   49,   50,   33,   34,  235,  159,  160,  273,  274,
  175,  275,  276,  277,  278,  207,  208,  384,  209,  180,
  253,  253,  253,  253,  253,  253,  405,  164,  406,  419,
  151,  407,  184,  408,  409,  410,  411,  203,   33,   34,
  190,  260,  236,  237,  238,  239,  116,  204,  134,  135,
   33,   34,  134,  135,   33,   34,  181,  182,  124,  125,
  205,  358,  359,  191,  325,  326,  211,  212,  213,  214,
  373,  134,  135,   33,   34,  192,  330,  253,  331,  134,
  135,   33,   34,  193,  134,  135,   33,   34,  134,  135,
   33,   34,  134,  135,   33,   34,  134,  135,   33,   34,
  380,  381,  353,  194,  134,  135,   33,   34,  228,  474,
  475,   79,   79,  134,  135,   33,   34,  134,  135,   33,
   34,  134,  135,   33,   34,  134,  135,   33,   34,  134,
  135,   33,   34,  217,  134,  135,   33,   34,  362,  363,
  364,  391,  365,  196,  199,  206,  219,  221,  298,  220,
  152,  153,  154,  155,  222,  233,  136,  242,  230,  244,
  136,  245,  246,  353,  366,  248,  247,  251,  160,  159,
  448,  449,  450,  253,  253,  253,  260,  268,  283,  136,
  269,  253,  284,  289,  426,  427,  428,  136,  286,  292,
  293,  297,  136,  256,  299,  320,  136,  300,  321,  322,
  136,  301,  324,  255,  136,  302,  434,  329,  336,  347,
  332,  230,  136,  343,  441,  349,  447,  303,  404,  490,
  350,  136,  356,  357,  348,  136,  360,  361,  368,  136,
  371,  372,  376,  136,  374,  375,  377,  136,  304,  378,
  386,  387,  388,  389,  305,  306,  307,  308,  309,  310,
  311,  394,  392,  312,  313,  314,  395,  397,  398,  418,
  399,  315,  420,  421,   62,  316,  317,  422,  429,  430,
  432,  431,  433,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,  171,  405,   74,  406,  438,  443,
  407,  455,  408,  409,  410,  411,  453,   75,   76,   77,
  435,  454,   78,   79,   80,   81,   82,   83,   84,   85,
   86,  458,  459,   87,   88,   89,   90,   91,   92,   93,
   94,   95,   96,   97,   98,   99,  100,  101,  461,  469,
  465,  472,  471,  479,  473,  482,  484,  485,  483,    1,
  487,  475,  102,  103,  474,  493,  494,   71,   21,   96,
  214,  193,   92,   93,  126,   98,  127,  155,  249,   81,
  250,  256,  141,  147,  104,  257,  229,  148,  149,  290,
  105,  106,  107,  108,   62,  335,  250,  109,  328,  327,
  379,  110,  111,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   37,  218,   74,  121,  298,  232,
  266,  265,  396,  296,  492,  452,  481,   75,   76,   77,
  417,  491,   78,   79,   80,   81,   82,   83,   84,   85,
   86,  437,  460,   87,   88,   89,   90,   91,   92,   93,
   94,   95,   96,   97,   98,   99,  100,  101,  132,  385,
    0,    0,    1,  486,  299,    0,    0,  300,    2,    3,
    4,  301,  102,  103,    0,  302,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  303,    5,    0,
    0,    0,    0,    0,  104,    0,    0,    0,    0,    0,
  105,  106,  107,  108,    0,    0,    0,  109,  304,    0,
    0,  110,  111,    0,  305,  306,  307,  308,  309,  310,
  311,    0,    0,  312,  313,  314,    6,    0,    0,    0,
    0,  315,    0,    0,    0,  316,  317,    0,    0,    0,
    0,    0,    7,    0,    0,    0,    0,    0,    8,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    9,   10,    0,    0,    0,
    0,    0,   11,
};
short yycheck[] = {                                      10,
   76,   77,   78,   33,   42,  164,  160,   33,  125,  125,
   42,   70,   71,  125,  123,  125,  256,  261,  256,  123,
   31,   80,   81,  125,  125,   36,   33,  258,   87,   88,
   89,   90,   91,  261,   33,  260,  261,  123,   97,   33,
   99,  100,  125,   33,  256,  104,  257,   33,  125,  256,
  260,   33,  257,  260,  261,  279,  280,  259,  256,   33,
  257,  220,  257,  301,   75,   76,   77,   78,   33,  142,
  260,  261,   33,  257,  260,  261,   33,  256,  260,  261,
   33,  260,  261,  125,   33,  283,  284,  259,  260,  261,
  257,  293,  257,  123,  257,  125,  257,  123,  123,  125,
  338,  339,  340,  341,  123,  330,  292,  257,  123,  321,
  322,  259,  262,  124,  125,  261,  123,  257,  125,  305,
  292,  293,  346,  330,  123,  137,  125,  367,  368,  123,
  123,  125,  257,  123,  123,  125,  123,  123,  328,  125,
  256,  123,  324,  125,  370,  371,  372,  329,  257,  123,
  260,  125,  306,  261,  256,  260,  261,  258,  123,  123,
  125,  261,  123,  261,  125,  261,  123,  261,  125,  281,
  123,  123,  125,  261,  123,  259,  249,  236,  259,  123,
  239,  283,  284,  260,  261,  301,  276,  277,  200,  201,
  261,  203,  204,  205,  206,  347,  348,  125,  350,  259,
  273,  274,  275,  276,  277,  278,  323,  276,  325,  368,
  259,  328,  259,  330,  331,  332,  333,  123,  260,  261,
  259,  259,  338,  339,  340,  341,  258,  123,  258,  259,
  260,  261,  258,  259,  260,  261,  303,  304,  321,  322,
  123,  300,  301,  259,  255,  256,  375,  376,  377,  378,
  309,  258,  259,  260,  261,  259,  268,  330,  269,  258,
  259,  260,  261,  259,  258,  259,  260,  261,  258,  259,
  260,  261,  258,  259,  260,  261,  258,  259,  260,  261,
  375,  376,  293,  259,  258,  259,  260,  261,  330,  326,
  327,  279,  280,  258,  259,  260,  261,  258,  259,  260,
  261,  258,  259,  260,  261,  258,  259,  260,  261,  258,
  259,  260,  261,  125,  258,  259,  260,  261,  347,  348,
  349,  332,  351,  259,  259,  123,  257,  285,  256,  276,
  379,  380,  381,  382,  257,  257,  366,  123,  349,  259,
  366,  257,   47,  354,  373,  260,   47,  257,  277,  276,
  409,  410,  411,  426,  427,  428,  259,  123,  123,  366,
  281,  434,  257,  257,  376,  377,  378,  366,  259,  259,
  123,  257,  366,  368,  302,  259,  366,  305,  259,  259,
  366,  309,  257,  367,  366,  313,  398,  125,  257,  288,
  346,  402,  366,  258,  405,  123,  407,  325,  256,  475,
  123,  366,  257,  259,  286,  366,  259,  261,  276,  366,
  259,  123,  123,  366,  261,  261,  123,  366,  346,  123,
  257,  261,  257,  257,  352,  353,  354,  355,  356,  357,
  358,  257,  280,  361,  362,  363,  125,  257,  123,  257,
  259,  369,  123,  259,  256,  373,  374,  258,  123,  257,
  279,  261,  257,  265,  266,  267,  268,  269,  270,  271,
  272,  273,  274,  275,  475,  323,  278,  325,  257,  261,
  328,  258,  330,  331,  332,  333,  257,  289,  290,  291,
  287,  257,  294,  295,  296,  297,  298,  299,  300,  301,
  302,  259,  125,  305,  306,  307,  308,  309,  310,  311,
  312,  313,  314,  315,  316,  317,  318,  319,  257,  260,
  258,  257,  259,  257,  259,  257,  257,  125,  261,    0,
  257,  327,  334,  335,  326,  257,  257,  257,  257,  123,
  257,  257,  257,  257,  125,  123,  125,  257,  125,  279,
  125,  125,  257,  257,  356,  125,  125,  257,  257,  235,
  362,  363,  364,  365,  256,  271,  138,  369,  258,  257,
  315,  373,  374,  265,  266,  267,  268,  269,  270,  271,
  272,  273,  274,  275,   13,  112,  278,   51,  256,  126,
  163,  162,  345,  240,  477,  412,  456,  289,  290,  291,
  354,  476,  294,  295,  296,  297,  298,  299,  300,  301,
  302,  402,  424,  305,  306,  307,  308,  309,  310,  311,
  312,  313,  314,  315,  316,  317,  318,  319,   58,  318,
   -1,   -1,  256,  467,  302,   -1,   -1,  305,  262,  263,
  264,  309,  334,  335,   -1,  313,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  325,  282,   -1,
   -1,   -1,   -1,   -1,  356,   -1,   -1,   -1,   -1,   -1,
  362,  363,  364,  365,   -1,   -1,   -1,  369,  346,   -1,
   -1,  373,  374,   -1,  352,  353,  354,  355,  356,  357,
  358,   -1,   -1,  361,  362,  363,  320,   -1,   -1,   -1,
   -1,  369,   -1,   -1,   -1,  373,  374,   -1,   -1,   -1,
   -1,   -1,  336,   -1,   -1,   -1,   -1,   -1,  342,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,   -1,   -1,
   -1,   -1,  366,
};
#define YYFINAL 12
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 382
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,0,0,0,0,0,"'*'",0,0,0,0,"'/'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"L_EOS",
"L_IPADDR","L_NUMBER","L_STRING","L_QSTRING","L_END_INCLUDE","T_INCLUDE",
"T_OPTIONS","T_DIRECTORY","T_PIDFILE","T_NAMED_XFER","T_DUMP_FILE",
"T_STATS_FILE","T_MEMSTATS_FILE","T_FAKE_IQUERY","T_RECURSION","T_FETCH_GLUE",
"T_QUERY_SOURCE","T_LISTEN_ON","T_PORT","T_ADDRESS","T_RRSET_ORDER","T_ORDER",
"T_NAME","T_CLASS","T_CONTROLS","T_INET","T_UNIX","T_PERM","T_OWNER","T_GROUP",
"T_ALLOW","T_DATASIZE","T_STACKSIZE","T_CORESIZE","T_DEFAULT","T_UNLIMITED",
"T_FILES","T_VERSION","T_HOSTSTATS","T_DEALLOC_ON_EXIT","T_TRANSFERS_IN",
"T_TRANSFERS_OUT","T_TRANSFERS_PER_NS","T_TRANSFER_FORMAT",
"T_MAX_TRANSFER_TIME_IN","T_ONE_ANSWER","T_MANY_ANSWERS","T_NOTIFY",
"T_AUTH_NXDOMAIN","T_MULTIPLE_CNAMES","T_USE_IXFR","T_MAINTAIN_IXFR_BASE",
"T_CLEAN_INTERVAL","T_INTERFACE_INTERVAL","T_STATS_INTERVAL",
"T_MAX_LOG_SIZE_IXFR","T_HEARTBEAT","T_USE_ID_POOL","T_MAX_NCACHE_TTL",
"T_HAS_OLD_CLIENTS","T_RFC2308_TYPE1","T_LAME_TTL","T_LOGGING","T_CATEGORY",
"T_CHANNEL","T_SEVERITY","T_DYNAMIC","T_FILE","T_VERSIONS","T_SIZE","T_SYSLOG",
"T_DEBUG","T_NULL_OUTPUT","T_PRINT_TIME","T_PRINT_CATEGORY","T_PRINT_SEVERITY",
"T_SORTLIST","T_TOPOLOGY","T_SERVER","T_LONG_AXFR","T_BOGUS","T_TRANSFERS",
"T_KEYS","T_SUPPORT_IXFR","T_ZONE","T_IN","T_CHAOS","T_HESIOD","T_TYPE",
"T_MASTER","T_SLAVE","T_STUB","T_RESPONSE","T_HINT","T_MASTERS",
"T_TRANSFER_SOURCE","T_PUBKEY","T_ALSO_NOTIFY","T_DIALUP","T_FILE_IXFR",
"T_IXFR_TMP","T_TRUSTED_KEYS","T_ACL","T_ALLOW_UPDATE","T_ALLOW_QUERY",
"T_ALLOW_TRANSFER","T_ALLOW_RECURSION","T_BLACKHOLE","T_SEC_KEY","T_ALGID",
"T_SECRET","T_CHECK_NAMES","T_WARN","T_FAIL","T_IGNORE","T_FORWARD",
"T_FORWARDERS","T_ONLY","T_FIRST","T_IF_NO_ANSWER","T_IF_NO_DOMAIN","T_YES",
"T_TRUE","T_NO","T_FALSE",
};
char *yyrule[] = {
"$accept : config_file",
"config_file : statement_list",
"statement_list : statement",
"statement_list : statement_list statement",
"statement : include_stmt",
"statement : options_stmt L_EOS",
"statement : controls_stmt L_EOS",
"statement : logging_stmt L_EOS",
"statement : server_stmt L_EOS",
"statement : zone_stmt L_EOS",
"statement : trusted_keys_stmt L_EOS",
"statement : acl_stmt L_EOS",
"statement : key_stmt L_EOS",
"statement : L_END_INCLUDE",
"statement : error L_EOS",
"statement : error L_END_INCLUDE",
"include_stmt : T_INCLUDE L_QSTRING L_EOS",
"$$1 :",
"options_stmt : T_OPTIONS $$1 '{' options '}'",
"options : option L_EOS",
"options : options option L_EOS",
"option :",
"option : T_VERSION L_QSTRING",
"option : T_DIRECTORY L_QSTRING",
"option : T_NAMED_XFER L_QSTRING",
"option : T_PIDFILE L_QSTRING",
"option : T_STATS_FILE L_QSTRING",
"option : T_MEMSTATS_FILE L_QSTRING",
"option : T_DUMP_FILE L_QSTRING",
"option : T_FAKE_IQUERY yea_or_nay",
"option : T_RECURSION yea_or_nay",
"option : T_FETCH_GLUE yea_or_nay",
"option : T_NOTIFY yea_or_nay",
"option : T_HOSTSTATS yea_or_nay",
"option : T_DEALLOC_ON_EXIT yea_or_nay",
"option : T_USE_IXFR yea_or_nay",
"option : T_MAINTAIN_IXFR_BASE yea_or_nay",
"option : T_HAS_OLD_CLIENTS yea_or_nay",
"option : T_AUTH_NXDOMAIN yea_or_nay",
"option : T_MULTIPLE_CNAMES yea_or_nay",
"option : T_CHECK_NAMES check_names_type check_names_opt",
"option : T_USE_ID_POOL yea_or_nay",
"option : T_RFC2308_TYPE1 yea_or_nay",
"option : T_LISTEN_ON maybe_port '{' address_match_list '}'",
"option : T_FORWARD forward_opt",
"$$2 :",
"option : T_FORWARDERS $$2 '{' opt_forwarders_list '}'",
"option : T_QUERY_SOURCE query_source",
"option : T_ALLOW_QUERY '{' address_match_list '}'",
"option : T_ALLOW_RECURSION '{' address_match_list '}'",
"option : T_ALLOW_TRANSFER '{' address_match_list '}'",
"option : T_SORTLIST '{' address_match_list '}'",
"option : T_BLACKHOLE '{' address_match_list '}'",
"option : T_TOPOLOGY '{' address_match_list '}'",
"option : size_clause",
"option : transfer_clause",
"option : T_TRANSFER_FORMAT transfer_format",
"option : T_MAX_TRANSFER_TIME_IN L_NUMBER",
"option : T_CLEAN_INTERVAL L_NUMBER",
"option : T_INTERFACE_INTERVAL L_NUMBER",
"option : T_STATS_INTERVAL L_NUMBER",
"option : T_MAX_LOG_SIZE_IXFR L_NUMBER",
"option : T_MAX_NCACHE_TTL L_NUMBER",
"option : T_LAME_TTL L_NUMBER",
"option : T_HEARTBEAT L_NUMBER",
"option : T_DIALUP yea_or_nay",
"option : T_RRSET_ORDER '{' rrset_ordering_list '}'",
"option : error",
"controls_stmt : T_CONTROLS '{' controls '}'",
"controls : control L_EOS",
"controls : controls control L_EOS",
"control :",
"control : T_INET maybe_wild_addr T_PORT in_port T_ALLOW '{' address_match_list '}'",
"control : T_UNIX L_QSTRING T_PERM L_NUMBER T_OWNER L_NUMBER T_GROUP L_NUMBER",
"control : error",
"rrset_ordering_list : rrset_ordering_element L_EOS",
"rrset_ordering_list : rrset_ordering_list rrset_ordering_element L_EOS",
"ordering_class :",
"ordering_class : T_CLASS any_string",
"ordering_type :",
"ordering_type : T_TYPE any_string",
"ordering_name :",
"ordering_name : T_NAME L_QSTRING",
"rrset_ordering_element : ordering_class ordering_type ordering_name T_ORDER L_STRING",
"transfer_format : T_ONE_ANSWER",
"transfer_format : T_MANY_ANSWERS",
"maybe_wild_addr : L_IPADDR",
"maybe_wild_addr : '*'",
"maybe_wild_port : in_port",
"maybe_wild_port : '*'",
"query_source_address : T_ADDRESS maybe_wild_addr",
"query_source_port : T_PORT maybe_wild_port",
"query_source : query_source_address",
"query_source : query_source_port",
"query_source : query_source_address query_source_port",
"query_source : query_source_port query_source_address",
"maybe_port :",
"maybe_port : T_PORT in_port",
"maybe_zero_port :",
"maybe_zero_port : T_PORT in_port",
"yea_or_nay : T_YES",
"yea_or_nay : T_TRUE",
"yea_or_nay : T_NO",
"yea_or_nay : T_FALSE",
"yea_or_nay : L_NUMBER",
"check_names_type : T_MASTER",
"check_names_type : T_SLAVE",
"check_names_type : T_RESPONSE",
"check_names_opt : T_WARN",
"check_names_opt : T_FAIL",
"check_names_opt : T_IGNORE",
"forward_opt : T_ONLY",
"forward_opt : T_FIRST",
"forward_opt : T_IF_NO_ANSWER",
"forward_opt : T_IF_NO_DOMAIN",
"size_clause : T_DATASIZE size_spec",
"size_clause : T_STACKSIZE size_spec",
"size_clause : T_CORESIZE size_spec",
"size_clause : T_FILES size_spec",
"size_spec : any_string",
"size_spec : L_NUMBER",
"size_spec : T_DEFAULT",
"size_spec : T_UNLIMITED",
"transfer_clause : T_TRANSFERS_IN L_NUMBER",
"transfer_clause : T_TRANSFERS_OUT L_NUMBER",
"transfer_clause : T_TRANSFERS_PER_NS L_NUMBER",
"opt_forwarders_list :",
"opt_forwarders_list : forwarders_in_addr_list",
"forwarders_in_addr_list : forwarders_in_addr L_EOS",
"forwarders_in_addr_list : forwarders_in_addr_list forwarders_in_addr L_EOS",
"forwarders_in_addr : L_IPADDR",
"$$3 :",
"logging_stmt : T_LOGGING $$3 '{' logging_opts_list '}'",
"logging_opts_list : logging_opt L_EOS",
"logging_opts_list : logging_opts_list logging_opt L_EOS",
"logging_opts_list : error",
"$$4 :",
"logging_opt : T_CATEGORY category $$4 '{' channel_list '}'",
"$$5 :",
"logging_opt : T_CHANNEL channel_name $$5 '{' channel_opt_list '}'",
"channel_severity : any_string",
"channel_severity : T_DEBUG",
"channel_severity : T_DEBUG L_NUMBER",
"channel_severity : T_DYNAMIC",
"version_modifier : T_VERSIONS L_NUMBER",
"version_modifier : T_VERSIONS T_UNLIMITED",
"size_modifier : T_SIZE size_spec",
"maybe_file_modifiers :",
"maybe_file_modifiers : version_modifier",
"maybe_file_modifiers : size_modifier",
"maybe_file_modifiers : version_modifier size_modifier",
"maybe_file_modifiers : size_modifier version_modifier",
"channel_file : T_FILE L_QSTRING maybe_file_modifiers",
"facility_name : any_string",
"facility_name : T_SYSLOG",
"maybe_syslog_facility :",
"maybe_syslog_facility : facility_name",
"channel_syslog : T_SYSLOG maybe_syslog_facility",
"channel_opt : channel_file",
"channel_opt : channel_syslog",
"channel_opt : T_NULL_OUTPUT",
"channel_opt : T_SEVERITY channel_severity",
"channel_opt : T_PRINT_TIME yea_or_nay",
"channel_opt : T_PRINT_CATEGORY yea_or_nay",
"channel_opt : T_PRINT_SEVERITY yea_or_nay",
"channel_opt_list : channel_opt L_EOS",
"channel_opt_list : channel_opt_list channel_opt L_EOS",
"channel_opt_list : error",
"channel_name : any_string",
"channel_name : T_NULL_OUTPUT",
"channel : channel_name",
"channel_list : channel L_EOS",
"channel_list : channel_list channel L_EOS",
"channel_list : error",
"category_name : any_string",
"category_name : T_DEFAULT",
"category_name : T_NOTIFY",
"category : category_name",
"$$6 :",
"server_stmt : T_SERVER L_IPADDR $$6 '{' server_info_list '}'",
"server_info_list : server_info L_EOS",
"server_info_list : server_info_list server_info L_EOS",
"server_info : T_BOGUS yea_or_nay",
"server_info : T_SUPPORT_IXFR yea_or_nay",
"server_info : T_TRANSFERS L_NUMBER",
"server_info : T_TRANSFER_FORMAT transfer_format",
"server_info : T_KEYS '{' key_list '}'",
"server_info : error",
"address_match_list : address_match_element L_EOS",
"address_match_list : address_match_list address_match_element L_EOS",
"address_match_element : address_match_simple",
"address_match_element : '!' address_match_simple",
"address_match_element : T_SEC_KEY L_STRING",
"address_match_simple : L_IPADDR",
"address_match_simple : L_IPADDR '/' L_NUMBER",
"address_match_simple : L_NUMBER '/' L_NUMBER",
"address_match_simple : address_name",
"address_match_simple : '{' address_match_list '}'",
"address_name : any_string",
"key_ref : any_string",
"key_list_element : key_ref",
"key_list : key_list_element L_EOS",
"key_list : key_list key_list_element L_EOS",
"key_list : error",
"$$7 :",
"key_stmt : T_SEC_KEY $$7 any_string '{' key_definition '}'",
"key_definition : algorithm_id secret",
"key_definition : secret algorithm_id",
"key_definition : error",
"algorithm_id : T_ALGID any_string L_EOS",
"secret : T_SECRET any_string L_EOS",
"acl_stmt : T_ACL any_string '{' address_match_list '}'",
"$$8 :",
"zone_stmt : T_ZONE L_QSTRING optional_class $$8 optional_zone_options_list",
"optional_zone_options_list :",
"optional_zone_options_list : '{' zone_option_list '}'",
"optional_class :",
"optional_class : any_string",
"zone_type : T_MASTER",
"zone_type : T_SLAVE",
"zone_type : T_HINT",
"zone_type : T_STUB",
"zone_type : T_FORWARD",
"zone_option_list : zone_option L_EOS",
"zone_option_list : zone_option_list zone_option L_EOS",
"zone_option : T_TYPE zone_type",
"zone_option : T_FILE L_QSTRING",
"zone_option : T_FILE_IXFR L_QSTRING",
"zone_option : T_IXFR_TMP L_QSTRING",
"zone_option : T_MASTERS maybe_zero_port '{' master_in_addr_list '}'",
"zone_option : T_TRANSFER_SOURCE maybe_wild_addr",
"zone_option : T_CHECK_NAMES check_names_opt",
"zone_option : T_ALLOW_UPDATE '{' address_match_list '}'",
"zone_option : T_ALLOW_QUERY '{' address_match_list '}'",
"zone_option : T_ALLOW_TRANSFER '{' address_match_list '}'",
"zone_option : T_FORWARD zone_forward_opt",
"$$9 :",
"zone_option : T_FORWARDERS $$9 '{' opt_zone_forwarders_list '}'",
"zone_option : T_MAX_TRANSFER_TIME_IN L_NUMBER",
"zone_option : T_MAX_LOG_SIZE_IXFR L_NUMBER",
"zone_option : T_NOTIFY yea_or_nay",
"zone_option : T_MAINTAIN_IXFR_BASE yea_or_nay",
"zone_option : T_PUBKEY L_NUMBER L_NUMBER L_NUMBER L_QSTRING",
"zone_option : T_ALSO_NOTIFY '{' opt_notify_in_addr_list '}'",
"zone_option : T_DIALUP yea_or_nay",
"zone_option : error",
"master_in_addr_list : master_in_addr L_EOS",
"master_in_addr_list : master_in_addr_list master_in_addr L_EOS",
"master_in_addr : L_IPADDR",
"opt_notify_in_addr_list :",
"opt_notify_in_addr_list : notify_in_addr_list",
"notify_in_addr_list : notify_in_addr L_EOS",
"notify_in_addr_list : notify_in_addr_list notify_in_addr L_EOS",
"notify_in_addr : L_IPADDR",
"zone_forward_opt : T_ONLY",
"zone_forward_opt : T_FIRST",
"opt_zone_forwarders_list :",
"opt_zone_forwarders_list : zone_forwarders_in_addr_list",
"zone_forwarders_in_addr_list : zone_forwarders_in_addr L_EOS",
"zone_forwarders_in_addr_list : zone_forwarders_in_addr_list zone_forwarders_in_addr L_EOS",
"zone_forwarders_in_addr : L_IPADDR",
"trusted_keys_stmt : T_TRUSTED_KEYS '{' trusted_keys_list '}'",
"trusted_keys_list : trusted_key L_EOS",
"trusted_keys_list : trusted_keys_list trusted_key L_EOS",
"trusted_key : L_STRING L_NUMBER L_NUMBER L_NUMBER L_QSTRING",
"in_port : L_NUMBER",
"any_string : L_STRING",
"any_string : L_QSTRING",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
struct yystack {
    short *ssp;
    YYSTYPE *vsp;
    short *ss;
    YYSTYPE *vs;
    int stacksize;
    short *sslim;
};
int yychar; /* some people use this, so we copy it in & out */
int yyerrflag; /* must be global for yyerrok & YYRECOVERING */
YYSTYPE yylval;
#line 1704 "ns_parser.y"

static char *
canonical_name(char *name) {
	char canonical[MAXDNAME];
	
	if (strlen(name) >= MAXDNAME)
		return (NULL);
	strcpy(canonical, name);
	if (makename(canonical, ".", sizeof canonical) < 0)
		return (NULL);
	return (savestr(canonical, 0));
}

static void
init_acls() {
	ip_match_element ime;
	ip_match_list iml;
	struct in_addr address;

	/* Create the predefined ACLs */

	address.s_addr = 0U;

	/* ACL "any" */
	ime = new_ip_match_pattern(address, 0);
	iml = new_ip_match_list();
	add_to_ip_match_list(iml, ime);
	define_acl(savestr("any", 1), iml);

	/* ACL "none" */
	ime = new_ip_match_pattern(address, 0);
	ip_match_negate(ime);
	iml = new_ip_match_list();
	add_to_ip_match_list(iml, ime);
	define_acl(savestr("none", 1), iml);

	/* ACL "localhost" */
	ime = new_ip_match_localhost();
	iml = new_ip_match_list();
	add_to_ip_match_list(iml, ime);
	define_acl(savestr("localhost", 1), iml);

	/* ACL "localnets" */
	ime = new_ip_match_localnets();
	iml = new_ip_match_list();
	add_to_ip_match_list(iml, ime);
	define_acl(savestr("localnets", 1), iml);
}

static void
free_sym_value(int type, void *value) {
	ns_debug(ns_log_parser, 99, "free_sym_value: type %06x value %p",
		 type, value);
	type &= ~0xffff;
	switch (type) {
	case SYM_ACL:
		free_ip_match_list(value);
		break;
	case SYM_KEY:
		free_key_info(value);
		break;
	default:
		ns_panic(ns_log_parser, 1,
			 "unhandled case in free_sym_value()");
		/* NOTREACHED */
		break;
	}
}

static log_channel
lookup_channel(char *name) {
	symbol_value value;

	if (lookup_symbol(symtab, name, SYM_CHANNEL, &value))
		return ((log_channel)(value.pointer));
	return (NULL);
}

static void
define_channel(char *name, log_channel channel) {
	symbol_value value;

	value.pointer = channel;  
	define_symbol(symtab, name, SYM_CHANNEL, value, SYMBOL_FREE_KEY);
}

static void
define_builtin_channels() {
	define_channel(savestr("default_syslog", 1), syslog_channel);
	define_channel(savestr("default_debug", 1), debug_channel);
	define_channel(savestr("default_stderr", 1), stderr_channel);
	define_channel(savestr("null", 1), null_channel);
}

static void
parser_setup() {
	seen_options = 0;
	seen_topology = 0;
	symtab = new_symbol_table(SYMBOL_TABLE_SIZE, NULL);
	if (authtab != NULL)
		free_symbol_table(authtab);
	authtab = new_symbol_table(AUTH_TABLE_SIZE, free_sym_value);
	init_acls();
	define_builtin_channels();
	INIT_LIST(current_controls);
}

static void
parser_cleanup() {
	if (symtab != NULL)
		free_symbol_table(symtab);
	symtab = NULL;
	/*
	 * We don't clean up authtab here because the ip_match_lists are in
	 * use.
	 */
}

/*
 * Public Interface
 */

ip_match_list
lookup_acl(char *name) {
	symbol_value value;

	if (lookup_symbol(authtab, name, SYM_ACL, &value))
		return ((ip_match_list)(value.pointer));
	return (NULL);
}

void
define_acl(char *name, ip_match_list iml) {
	symbol_value value;

	INSIST(name != NULL);
	INSIST(iml != NULL);

	value.pointer = iml;
	define_symbol(authtab, name, SYM_ACL, value,
		      SYMBOL_FREE_KEY|SYMBOL_FREE_VALUE);
	ns_debug(ns_log_parser, 7, "acl %s", name);
	dprint_ip_match_list(ns_log_parser, iml, 2, "allow ", "deny ");
}

struct dst_key *
lookup_key(char *name) {
	symbol_value value;

	if (lookup_symbol(authtab, name, SYM_KEY, &value))
		return ((struct dst_key *)(value.pointer));
	return (NULL);
}

void
define_key(char *name, struct dst_key *dst_key) {
	symbol_value value;

	INSIST(name != NULL);
	INSIST(dst_key != NULL);

	value.pointer = dst_key;
	define_symbol(authtab, name, SYM_KEY, value, SYMBOL_FREE_VALUE);
	dprint_key_info(dst_key);
}

void
parse_configuration(const char *filename) {
	FILE *config_stream;

	config_stream = fopen(filename, "r");
	if (config_stream == NULL)
		ns_panic(ns_log_parser, 0, "can't open '%s'", filename);

	lexer_setup();
	parser_setup();
	lexer_begin_file(filename, config_stream);
	(void)yyparse();
	lexer_end_file();
	parser_cleanup();
}

void
parser_initialize(void) {
	lexer_initialize();
}

void
parser_shutdown(void) {
	if (authtab != NULL)
		free_symbol_table(authtab);
	lexer_shutdown();
}
#line 1179 "y.tab.c"
/* allocate initial stack */
#if defined(__STDC__) || defined(__cplusplus)
static int yyinitstack(struct yystack *sp)
#else
static int yyinitstack(sp)
    struct yystack *sp;
#endif
{
    int newsize;
    short *newss;
    YYSTYPE *newvs;

    newsize = YYINITSTACKSIZE;
    newss = (short *)malloc(newsize * sizeof *newss);
    newvs = (YYSTYPE *)malloc(newsize * sizeof *newvs);
    sp->ss = sp->ssp = newss;
    sp->vs = sp->vsp = newvs;
    if (newss == NULL || newvs == NULL) return -1;
    sp->stacksize = newsize;
    sp->sslim = newss + newsize - 1;
    return 0;
}

/* double stack size, up to YYMAXDEPTH */
#if defined(__STDC__) || defined(__cplusplus)
static int yygrowstack(struct yystack *sp)
#else
static int yygrowstack(sp)
    struct yystack *sp;
#endif
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = sp->stacksize) >= YYMAXDEPTH) return -1;
    if ((newsize *= 2) > YYMAXDEPTH) newsize = YYMAXDEPTH;
    i = sp->ssp - sp->ss;
    if ((newss = (short *)realloc(sp->ss, newsize * sizeof *newss)) == NULL)
        return -1;
    sp->ss = newss;
    sp->ssp = newss + i;
    if ((newvs = (YYSTYPE *)realloc(sp->vs, newsize * sizeof *newvs)) == NULL)
        return -1;
    sp->vs = newvs;
    sp->vsp = newvs + i;
    sp->stacksize = newsize;
    sp->sslim = newss + newsize - 1;
    return 0;
}

#define YYFREESTACK(sp) { free((sp)->ss); free((sp)->vs); }

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate, yych;
    register YYSTYPE *yyvsp;
    YYSTYPE yyval;
    struct yystack yystk;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = yych = YYEMPTY;

    if (yyinitstack(&yystk)) goto yyoverflow;
    *yystk.ssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yych < 0)
    {
        if ((yych = YYLEX) < 0) yych = 0;
        yychar = yych;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yych <= YYMAXTOKEN) yys = yyname[yych];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yych, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yych) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yych)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystk.ssp >= yystk.sslim && yygrowstack(&yystk))
            goto yyoverflow;
        *++yystk.ssp = yystate = yytable[yyn];
        *++yystk.vsp = yylval;
        yychar = yych = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yych) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yych)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystk.ssp]) &&
                    (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystk.ssp, yytable[yyn]);
#endif
                if (yystk.ssp >= yystk.sslim && yygrowstack(&yystk))
                    goto yyoverflow;
                *++yystk.ssp = yystate = yytable[yyn];
                *++yystk.vsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystk.ssp);
#endif
                if (yystk.ssp <= yystk.ss) goto yyabort;
                --yystk.ssp;
                --yystk.vsp;
            }
        }
    }
    else
    {
        if (yych == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yych <= YYMAXTOKEN) yys = yyname[yych];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yych, yys);
        }
#endif
        yychar = yych = YYEMPTY;
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyvsp = yystk.vsp; /* for speed in code under switch() */
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 241 "ns_parser.y"
{
		if (EMPTY(current_controls))
			ns_ctl_defaults(&current_controls);
		ns_ctl_install(&current_controls);
	}
break;
case 16:
#line 266 "ns_parser.y"
{ lexer_begin_file(yyvsp[-1].cp, NULL); }
break;
case 17:
#line 274 "ns_parser.y"
{
		if (seen_options)
			parser_error(0, "cannot redefine options");
		current_options = new_options();
	}
break;
case 18:
#line 280 "ns_parser.y"
{
		if (!seen_options)
			set_options(current_options, 0);
		else
			free_options(current_options);
		current_options = NULL;
		seen_options = 1;
	}
break;
case 22:
#line 296 "ns_parser.y"
{
		if (current_options->version != NULL)
			freestr(current_options->version);
		current_options->version = yyvsp[0].cp;
	}
break;
case 23:
#line 302 "ns_parser.y"
{
		if (current_options->directory != NULL)
			freestr(current_options->directory);
		current_options->directory = yyvsp[0].cp;
	}
break;
case 24:
#line 308 "ns_parser.y"
{
		if (current_options->named_xfer != NULL)
			freestr(current_options->named_xfer);
		current_options->named_xfer = yyvsp[0].cp;
	}
break;
case 25:
#line 314 "ns_parser.y"
{
		if (current_options->pid_filename != NULL)
			freestr(current_options->pid_filename);
		current_options->pid_filename = yyvsp[0].cp;
	}
break;
case 26:
#line 320 "ns_parser.y"
{
		if (current_options->stats_filename != NULL)
			freestr(current_options->stats_filename);
		current_options->stats_filename = yyvsp[0].cp;
	}
break;
case 27:
#line 326 "ns_parser.y"
{
		if (current_options->memstats_filename != NULL)
			freestr(current_options->memstats_filename);
		current_options->memstats_filename = yyvsp[0].cp;
	}
break;
case 28:
#line 332 "ns_parser.y"
{
		if (current_options->dump_filename != NULL)
			freestr(current_options->dump_filename);
		current_options->dump_filename = yyvsp[0].cp;
	}
break;
case 29:
#line 338 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_FAKE_IQUERY, yyvsp[0].num);
	}
break;
case 30:
#line 343 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_NORECURSE, !yyvsp[0].num);
	}
break;
case 31:
#line 348 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_NOFETCHGLUE, !yyvsp[0].num);
	}
break;
case 32:
#line 353 "ns_parser.y"
{
		set_global_boolean_option(current_options, 
			OPTION_NONOTIFY, !yyvsp[0].num);
	}
break;
case 33:
#line 358 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_HOSTSTATS, yyvsp[0].num);
	}
break;
case 34:
#line 363 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_DEALLOC_ON_EXIT, yyvsp[0].num);
	}
break;
case 35:
#line 368 "ns_parser.y"
{
		set_global_boolean_option(current_options, OPTION_USE_IXFR, yyvsp[0].num);
	}
break;
case 36:
#line 372 "ns_parser.y"
{
		set_global_boolean_option(current_options,
					  OPTION_MAINTAIN_IXFR_BASE, yyvsp[0].num);
	}
break;
case 37:
#line 377 "ns_parser.y"
{
		set_global_boolean_option(current_options,
					  OPTION_MAINTAIN_IXFR_BASE, yyvsp[0].num);
		set_global_boolean_option(current_options,
					  OPTION_NORFC2308_TYPE1, yyvsp[0].num);
		set_global_boolean_option(current_options,
					  OPTION_NONAUTH_NXDOMAIN, !yyvsp[0].num);
	}
break;
case 38:
#line 386 "ns_parser.y"
{
		set_global_boolean_option(current_options, OPTION_NONAUTH_NXDOMAIN,
				   !yyvsp[0].num);
	}
break;
case 39:
#line 391 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_MULTIPLE_CNAMES, yyvsp[0].num);
	}
break;
case 40:
#line 396 "ns_parser.y"
{
		current_options->check_names[yyvsp[-1].s_int] = (enum severity)yyvsp[0].s_int;
	}
break;
case 41:
#line 400 "ns_parser.y"
{
		set_global_boolean_option(current_options,
					  OPTION_USE_ID_POOL, yyvsp[0].num);
	}
break;
case 42:
#line 405 "ns_parser.y"
{
		set_global_boolean_option(current_options,
                        		  OPTION_NORFC2308_TYPE1, !yyvsp[0].num);
	}
break;
case 43:
#line 410 "ns_parser.y"
{
		char port_string[10];
		symbol_value value;

		(void)sprintf(port_string, "%u", yyvsp[-3].us_int);
		if (lookup_symbol(symtab, port_string, SYM_PORT, NULL))
			parser_error(0,
				     "cannot redefine listen-on for port %u",
				     ntohs(yyvsp[-3].us_int));
		else {
			add_listen_on(current_options, yyvsp[-3].us_int, yyvsp[-1].iml);
			value.pointer = NULL;
			define_symbol(symtab, savestr(port_string, 1),
				      SYM_PORT, value, SYMBOL_FREE_KEY);
		}

	}
break;
case 45:
#line 429 "ns_parser.y"
{
		if (current_options->fwdtab) {
			free_forwarders(current_options->fwdtab);
			current_options->fwdtab = NULL;
		}
	}
break;
case 48:
#line 438 "ns_parser.y"
{
		if (current_options->query_acl) {
			parser_warning(0,
			      "options allow-query acl already set; skipping");
			free_ip_match_list(yyvsp[-1].iml);
		} else 
			current_options->query_acl = yyvsp[-1].iml;
	}
break;
case 49:
#line 447 "ns_parser.y"
{
		if (current_options->recursion_acl) {
			parser_warning(0,
			      "options allow-recursion acl already set; skipping");
			free_ip_match_list(yyvsp[-1].iml);
		} else
			current_options->recursion_acl = yyvsp[-1].iml;
	}
break;
case 50:
#line 456 "ns_parser.y"
{
		if (current_options->transfer_acl) {
			parser_warning(0,
			   "options allow-transfer acl already set; skipping");
			free_ip_match_list(yyvsp[-1].iml);
		} else 
			current_options->transfer_acl = yyvsp[-1].iml;
	}
break;
case 51:
#line 465 "ns_parser.y"
{
		if (current_options->sortlist) {
			parser_warning(0,
			      "options sortlist already set; skipping");
			free_ip_match_list(yyvsp[-1].iml);
		} else
			current_options->sortlist = yyvsp[-1].iml;
	}
break;
case 52:
#line 474 "ns_parser.y"
{
		if (current_options->blackhole_acl) {
			parser_warning(0,
			      "options blackhole already set; skipping");
			free_ip_match_list(yyvsp[-1].iml);
		} else
			current_options->blackhole_acl = yyvsp[-1].iml;
	}
break;
case 53:
#line 483 "ns_parser.y"
{
		if (current_options->topology) {
			parser_warning(0,
			      "options topology already set; skipping");
			free_ip_match_list(yyvsp[-1].iml);
		} else
			current_options->topology = yyvsp[-1].iml;
	}
break;
case 54:
#line 492 "ns_parser.y"
{
		/* To get around the $$ = $1 default rule. */
	}
break;
case 56:
#line 497 "ns_parser.y"
{
		current_options->transfer_format = yyvsp[0].axfr_fmt;
	}
break;
case 57:
#line 501 "ns_parser.y"
{
		current_options->max_transfer_time_in = yyvsp[0].num * 60;
	}
break;
case 58:
#line 505 "ns_parser.y"
{
		current_options->clean_interval = yyvsp[0].num * 60;
	}
break;
case 59:
#line 509 "ns_parser.y"
{
		current_options->interface_interval = yyvsp[0].num * 60;
	}
break;
case 60:
#line 513 "ns_parser.y"
{
		current_options->stats_interval = yyvsp[0].num * 60;
	}
break;
case 61:
#line 517 "ns_parser.y"
{
		current_options->max_log_size_ixfr = yyvsp[0].num;
	}
break;
case 62:
#line 521 "ns_parser.y"
{
		current_options->max_ncache_ttl = yyvsp[0].num;
	}
break;
case 63:
#line 525 "ns_parser.y"
{
		current_options->lame_ttl = yyvsp[0].num;
	}
break;
case 64:
#line 529 "ns_parser.y"
{
		current_options->heartbeat_interval = yyvsp[0].num * 60;
	}
break;
case 65:
#line 533 "ns_parser.y"
{
		set_global_boolean_option(current_options,
                                          OPTION_NODIALUP, !yyvsp[0].num);
	}
break;
case 66:
#line 538 "ns_parser.y"
{
		if (current_options->ordering)
			free_rrset_order_list(current_options->ordering);
		current_options->ordering = yyvsp[-1].rol;
	}
break;
case 72:
#line 559 "ns_parser.y"
{
		ns_ctl_add(&current_controls, ns_ctl_new_inet(yyvsp[-6].ip_addr, yyvsp[-4].us_int, yyvsp[-1].iml));
	}
break;
case 73:
#line 563 "ns_parser.y"
{
		ns_ctl_add(&current_controls, ns_ctl_new_unix(yyvsp[-6].cp, yyvsp[-4].num, yyvsp[-2].num, yyvsp[0].num));
	}
break;
case 75:
#line 570 "ns_parser.y"
{
		rrset_order_list rol;

		rol = new_rrset_order_list();
		if (yyvsp[-1].roe != NULL) {
			add_to_rrset_order_list(rol, yyvsp[-1].roe);
		}
		
		yyval.rol = rol;
	}
break;
case 76:
#line 581 "ns_parser.y"
{
		if (yyvsp[-1].roe != NULL) {
			add_to_rrset_order_list(yyvsp[-2].rol, yyvsp[-1].roe);
		}
		yyval.rol = yyvsp[-2].rol;
	}
break;
case 77:
#line 590 "ns_parser.y"
{
		yyval.s_int = C_ANY;
	}
break;
case 78:
#line 594 "ns_parser.y"
{
		symbol_value value;

		if (lookup_symbol(constants, yyvsp[0].cp, SYM_CLASS, &value))
			yyval.s_int = value.integer;
		else {
			parser_error(0, "unknown class '%s'; using ANY", yyvsp[0].cp);
			yyval.s_int = C_ANY;
		}
		freestr(yyvsp[0].cp);
	}
break;
case 79:
#line 608 "ns_parser.y"
{
		yyval.s_int = ns_t_any;
	}
break;
case 80:
#line 612 "ns_parser.y"
{
		int success;

		if (strcmp(yyvsp[0].cp, "*") == 0) {
			yyval.s_int = ns_t_any;
		} else {
			yyval.s_int = __sym_ston(__p_type_syms, yyvsp[0].cp, &success);
			if (success == 0) {
				yyval.s_int = ns_t_any;
				parser_error(0,
					     "unknown type '%s'; assuming ANY",
					     yyvsp[0].cp);
			}
		}
		freestr(yyvsp[0].cp);
	}
break;
case 81:
#line 630 "ns_parser.y"
{
		yyval.cp = savestr("*", 1);
	}
break;
case 82:
#line 634 "ns_parser.y"
{
		if (strcmp(".",yyvsp[0].cp) == 0 || strcmp("*.",yyvsp[0].cp) == 0) {
			yyval.cp = savestr("*", 1);
			freestr(yyvsp[0].cp);
		} else {
			yyval.cp = yyvsp[0].cp ;
		}
		/* XXX Should do any more name validation here? */
	}
break;
case 83:
#line 646 "ns_parser.y"
{
		enum ordering o;

		if (strlen(yyvsp[0].cp) == 0) {
			parser_error(0, "null order name");
			yyval.roe = NULL ;
		} else {
			o = lookup_ordering(yyvsp[0].cp);
			if (o == unknown_order) {
				o = (enum ordering)DEFAULT_ORDERING;
				parser_error(0,
					     "invalid order name '%s'; using %s",
					     yyvsp[0].cp, p_order(o));
			}
			
			freestr(yyvsp[0].cp);
			
			yyval.roe = new_rrset_order_element(yyvsp[-4].s_int, yyvsp[-3].s_int, yyvsp[-2].cp, o);
		}
	}
break;
case 84:
#line 669 "ns_parser.y"
{
		yyval.axfr_fmt = axfr_one_answer;
	}
break;
case 85:
#line 673 "ns_parser.y"
{
		yyval.axfr_fmt = axfr_many_answers;
	}
break;
case 86:
#line 678 "ns_parser.y"
{ yyval.ip_addr = yyvsp[0].ip_addr; }
break;
case 87:
#line 679 "ns_parser.y"
{ yyval.ip_addr.s_addr = htonl(INADDR_ANY); }
break;
case 88:
#line 682 "ns_parser.y"
{ yyval.us_int = yyvsp[0].us_int; }
break;
case 89:
#line 683 "ns_parser.y"
{ yyval.us_int = htons(0); }
break;
case 90:
#line 687 "ns_parser.y"
{
		current_options->query_source.sin_addr = yyvsp[0].ip_addr;
	}
break;
case 91:
#line 693 "ns_parser.y"
{
		current_options->query_source.sin_port = yyvsp[0].us_int;
	}
break;
case 96:
#line 704 "ns_parser.y"
{ yyval.us_int = htons(NS_DEFAULTPORT); }
break;
case 97:
#line 705 "ns_parser.y"
{ yyval.us_int = yyvsp[0].us_int; }
break;
case 98:
#line 708 "ns_parser.y"
{ yyval.us_int = htons(0); }
break;
case 99:
#line 709 "ns_parser.y"
{ yyval.us_int = yyvsp[0].us_int; }
break;
case 100:
#line 714 "ns_parser.y"
{ 
		yyval.num = 1;	
	}
break;
case 101:
#line 718 "ns_parser.y"
{ 
		yyval.num = 1;	
	}
break;
case 102:
#line 722 "ns_parser.y"
{ 
		yyval.num = 0;	
	}
break;
case 103:
#line 726 "ns_parser.y"
{ 
		yyval.num = 0;	
	}
break;
case 104:
#line 730 "ns_parser.y"
{ 
		if (yyvsp[0].num == 1 || yyvsp[0].num == 0) {
			yyval.num = yyvsp[0].num;
		} else {
			parser_warning(0,
				       "number should be 0 or 1; assuming 1");
			yyval.num = 1;
		}
	}
break;
case 105:
#line 742 "ns_parser.y"
{
		yyval.s_int = primary_trans;
	}
break;
case 106:
#line 746 "ns_parser.y"
{
		yyval.s_int = secondary_trans;
	}
break;
case 107:
#line 750 "ns_parser.y"
{
		yyval.s_int = response_trans;
	}
break;
case 108:
#line 756 "ns_parser.y"
{
		yyval.s_int = warn;
	}
break;
case 109:
#line 760 "ns_parser.y"
{
		yyval.s_int = fail;
	}
break;
case 110:
#line 764 "ns_parser.y"
{
		yyval.s_int = ignore;
	}
break;
case 111:
#line 770 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_FORWARD_ONLY, 1);
	}
break;
case 112:
#line 775 "ns_parser.y"
{
		set_global_boolean_option(current_options,
			OPTION_FORWARD_ONLY, 0);
	}
break;
case 113:
#line 780 "ns_parser.y"
{
		parser_warning(0, "forward if-no-answer is unimplemented");
	}
break;
case 114:
#line 784 "ns_parser.y"
{
		parser_warning(0, "forward if-no-domain is unimplemented");
	}
break;
case 115:
#line 790 "ns_parser.y"
{
		current_options->data_size = yyvsp[0].ul_int;
	}
break;
case 116:
#line 794 "ns_parser.y"
{
		current_options->stack_size = yyvsp[0].ul_int;
	}
break;
case 117:
#line 798 "ns_parser.y"
{
		current_options->core_size = yyvsp[0].ul_int;
	}
break;
case 118:
#line 802 "ns_parser.y"
{
		current_options->files = yyvsp[0].ul_int;
	}
break;
case 119:
#line 808 "ns_parser.y"
{
		u_long result;

		if (unit_to_ulong(yyvsp[0].cp, &result))
			yyval.ul_int = result;
		else {
			parser_error(0, "invalid unit string '%s'", yyvsp[0].cp);
			/* 0 means "use default" */
			yyval.ul_int = 0;
		}
		freestr(yyvsp[0].cp);
	}
break;
case 120:
#line 821 "ns_parser.y"
{	
		yyval.ul_int = (u_long)yyvsp[0].num;
	}
break;
case 121:
#line 825 "ns_parser.y"
{
		yyval.ul_int = 0;
	}
break;
case 122:
#line 829 "ns_parser.y"
{
		yyval.ul_int = ULONG_MAX;
	}
break;
case 123:
#line 835 "ns_parser.y"
{
		current_options->transfers_in = (u_long) yyvsp[0].num;
	}
break;
case 124:
#line 839 "ns_parser.y"
{
		current_options->transfers_out = (u_long) yyvsp[0].num;
	}
break;
case 125:
#line 843 "ns_parser.y"
{
		current_options->transfers_per_ns = (u_long) yyvsp[0].num;
	}
break;
case 128:
#line 853 "ns_parser.y"
{
		/* nothing */
	}
break;
case 129:
#line 857 "ns_parser.y"
{
		/* nothing */
	}
break;
case 130:
#line 863 "ns_parser.y"
{
	  	add_global_forwarder(current_options, yyvsp[0].ip_addr);
	}
break;
case 131:
#line 873 "ns_parser.y"
{
		current_logging = begin_logging();
	}
break;
case 132:
#line 877 "ns_parser.y"
{
		end_logging(current_logging, 1);
		current_logging = NULL;
	}
break;
case 136:
#line 889 "ns_parser.y"
{
		current_category = yyvsp[0].s_int;
	}
break;
case 138:
#line 894 "ns_parser.y"
{
		chan_type = log_null;
		chan_flags = 0;
		chan_level = log_info;
	}
break;
case 139:
#line 900 "ns_parser.y"
{
		log_channel current_channel = NULL;

		if (lookup_channel(yyvsp[-4].cp) != NULL) {
			parser_error(0, "can't redefine channel '%s'", yyvsp[-4].cp);
			freestr(yyvsp[-4].cp);
		} else {
			switch (chan_type) {
			case log_file:
				current_channel =
					log_new_file_channel(chan_flags,
							     chan_level,
							     chan_name, NULL,
							     chan_versions,
							     chan_max_size);
				freestr(chan_name);
				chan_name = NULL;
				break;
			case log_syslog:
				current_channel =
					log_new_syslog_channel(chan_flags,
							       chan_level,
							       chan_facility);
				break;
			case log_null:
				current_channel = log_new_null_channel();
				break;
			default:
				ns_panic(ns_log_parser, 1,
					 "unknown channel type: %d",
					 chan_type);
			}
			if (current_channel == NULL)
				ns_panic(ns_log_parser, 0,
					 "couldn't create channel");
			define_channel(yyvsp[-4].cp, current_channel);
		}
	}
break;
case 140:
#line 941 "ns_parser.y"
{
		symbol_value value;

		if (lookup_symbol(constants, yyvsp[0].cp, SYM_LOGGING, &value)) {
			chan_level = value.integer;
		} else {
			parser_error(0, "unknown severity '%s'", yyvsp[0].cp);
			chan_level = log_debug(99);
		}
		freestr(yyvsp[0].cp);
	}
break;
case 141:
#line 953 "ns_parser.y"
{
		chan_level = log_debug(1);
	}
break;
case 142:
#line 957 "ns_parser.y"
{
		chan_level = yyvsp[0].num;
	}
break;
case 143:
#line 961 "ns_parser.y"
{
		chan_level = 0;
		chan_flags |= LOG_USE_CONTEXT_LEVEL|LOG_REQUIRE_DEBUG;
	}
break;
case 144:
#line 968 "ns_parser.y"
{
		chan_versions = yyvsp[0].num;
		chan_flags |= LOG_TRUNCATE;
	}
break;
case 145:
#line 973 "ns_parser.y"
{
		chan_versions = LOG_MAX_VERSIONS;
		chan_flags |= LOG_TRUNCATE;
	}
break;
case 146:
#line 980 "ns_parser.y"
{
		chan_max_size = yyvsp[0].ul_int;
	}
break;
case 147:
#line 986 "ns_parser.y"
{
		chan_versions = 0;
		chan_max_size = ULONG_MAX;
	}
break;
case 148:
#line 991 "ns_parser.y"
{
		chan_max_size = ULONG_MAX;
	}
break;
case 149:
#line 995 "ns_parser.y"
{
		chan_versions = 0;
	}
break;
case 152:
#line 1003 "ns_parser.y"
{
		chan_flags |= LOG_CLOSE_STREAM;
		chan_type = log_file;
		chan_name = yyvsp[-1].cp;
	}
break;
case 153:
#line 1011 "ns_parser.y"
{ yyval.cp = yyvsp[0].cp; }
break;
case 154:
#line 1012 "ns_parser.y"
{ yyval.cp = savestr("syslog", 1); }
break;
case 155:
#line 1015 "ns_parser.y"
{ yyval.s_int = LOG_DAEMON; }
break;
case 156:
#line 1017 "ns_parser.y"
{
		symbol_value value;

		if (lookup_symbol(constants, yyvsp[0].cp, SYM_SYSLOG, &value)) {
			yyval.s_int = value.integer;
		} else {
			parser_error(0, "unknown facility '%s'", yyvsp[0].cp);
			yyval.s_int = LOG_DAEMON;
		}
		freestr(yyvsp[0].cp);
	}
break;
case 157:
#line 1031 "ns_parser.y"
{
		chan_type = log_syslog;
		chan_facility = yyvsp[0].s_int;
	}
break;
case 158:
#line 1037 "ns_parser.y"
{ /* nothing to do */ }
break;
case 159:
#line 1038 "ns_parser.y"
{ /* nothing to do */ }
break;
case 160:
#line 1040 "ns_parser.y"
{
		chan_type = log_null;
	}
break;
case 161:
#line 1043 "ns_parser.y"
{ /* nothing to do */ }
break;
case 162:
#line 1045 "ns_parser.y"
{
		if (yyvsp[0].num)
			chan_flags |= LOG_TIMESTAMP;
		else
			chan_flags &= ~LOG_TIMESTAMP;
	}
break;
case 163:
#line 1052 "ns_parser.y"
{
		if (yyvsp[0].num)
			chan_flags |= LOG_PRINT_CATEGORY;
		else
			chan_flags &= ~LOG_PRINT_CATEGORY;
	}
break;
case 164:
#line 1059 "ns_parser.y"
{
		if (yyvsp[0].num)
			chan_flags |= LOG_PRINT_LEVEL;
		else
			chan_flags &= ~LOG_PRINT_LEVEL;
	}
break;
case 169:
#line 1073 "ns_parser.y"
{ yyval.cp = savestr("null", 1); }
break;
case 170:
#line 1077 "ns_parser.y"
{
		log_channel channel;
		symbol_value value;

		if (current_category >= 0) {
			channel = lookup_channel(yyvsp[0].cp);
			if (channel != NULL) {
				add_log_channel(current_logging,
						current_category, channel);
			} else
				parser_error(0, "unknown channel '%s'", yyvsp[0].cp);
		}
		freestr(yyvsp[0].cp);
	}
break;
case 175:
#line 1099 "ns_parser.y"
{ yyval.cp = savestr("default", 1); }
break;
case 176:
#line 1100 "ns_parser.y"
{ yyval.cp = savestr("notify", 1); }
break;
case 177:
#line 1104 "ns_parser.y"
{
		symbol_value value;

		if (lookup_symbol(constants, yyvsp[0].cp, SYM_CATEGORY, &value))
			yyval.s_int = value.integer;
		else {
			parser_error(0, "invalid logging category '%s'",
				     yyvsp[0].cp);
			yyval.s_int = -1;
		}
		freestr(yyvsp[0].cp);
	}
break;
case 178:
#line 1123 "ns_parser.y"
{
		const char *ip_printable;
		symbol_value value;
		
		ip_printable = inet_ntoa(yyvsp[0].ip_addr);
		value.pointer = NULL;
		if (lookup_symbol(symtab, ip_printable, SYM_SERVER, NULL))
			seen_server = 1;
		else
			seen_server = 0;
		if (seen_server)
			parser_error(0, "cannot redefine server '%s'", 
				     ip_printable);
		else
			define_symbol(symtab, savestr(ip_printable, 1),
				      SYM_SERVER, value,
				      SYMBOL_FREE_KEY);
		current_server = begin_server(yyvsp[0].ip_addr);
	}
break;
case 179:
#line 1143 "ns_parser.y"
{
		end_server(current_server, !seen_server);
	}
break;
case 182:
#line 1153 "ns_parser.y"
{
		set_server_option(current_server, SERVER_INFO_BOGUS, yyvsp[0].num);
	}
break;
case 183:
#line 1157 "ns_parser.y"
{
		set_server_option(current_server, SERVER_INFO_SUPPORT_IXFR, yyvsp[0].num);
	}
break;
case 184:
#line 1161 "ns_parser.y"
{
		set_server_transfers(current_server, (int)yyvsp[0].num);
	}
break;
case 185:
#line 1165 "ns_parser.y"
{
		set_server_transfer_format(current_server, yyvsp[0].axfr_fmt);
	}
break;
case 188:
#line 1177 "ns_parser.y"
{
		ip_match_list iml;
		
		iml = new_ip_match_list();
		if (yyvsp[-1].ime != NULL)
			add_to_ip_match_list(iml, yyvsp[-1].ime);
		yyval.iml = iml;
	}
break;
case 189:
#line 1186 "ns_parser.y"
{
		if (yyvsp[-1].ime != NULL)
			add_to_ip_match_list(yyvsp[-2].iml, yyvsp[-1].ime);
		yyval.iml = yyvsp[-2].iml;
	}
break;
case 191:
#line 1195 "ns_parser.y"
{
		if (yyvsp[0].ime != NULL)
			ip_match_negate(yyvsp[0].ime);
		yyval.ime = yyvsp[0].ime;
	}
break;
case 192:
#line 1201 "ns_parser.y"
{
		char *key_name;
		struct dst_key *dst_key;

		key_name = canonical_name(yyvsp[0].cp);
		if (key_name == NULL) {
			parser_error(0, "can't make key name '%s' canonical",
				     yyvsp[0].cp);
			key_name = savestr("__bad_key__", 1);
		}
		dst_key = find_key(key_name, NULL);
		yyval.ime = new_ip_match_key(dst_key);
	}
break;
case 193:
#line 1217 "ns_parser.y"
{
		yyval.ime = new_ip_match_pattern(yyvsp[0].ip_addr, 32);
	}
break;
case 194:
#line 1221 "ns_parser.y"
{
		if (yyvsp[0].num < 0 || yyvsp[0].num > 32) {
			parser_error(0, "mask bits out of range; skipping");
			yyval.ime = NULL;
		} else {
			yyval.ime = new_ip_match_pattern(yyvsp[-2].ip_addr, yyvsp[0].num);
			if (yyval.ime == NULL)
				parser_error(0, 
					   "address/mask mismatch; skipping");
		}
	}
break;
case 195:
#line 1233 "ns_parser.y"
{
		struct in_addr ia;

		if (yyvsp[-2].num > 255) {
			parser_error(0, "address out of range; skipping");
			yyval.ime = NULL;
		} else {
			if (yyvsp[0].num < 0 || yyvsp[0].num > 32) {
				parser_error(0,
					"mask bits out of range; skipping");
					yyval.ime = NULL;
			} else {
				ia.s_addr = htonl((yyvsp[-2].num & 0xff) << 24);
				yyval.ime = new_ip_match_pattern(ia, yyvsp[0].num);
				if (yyval.ime == NULL)
					parser_error(0, 
					   "address/mask mismatch; skipping");
			}
		}
	}
break;
case 197:
#line 1255 "ns_parser.y"
{
		char name[256];

		/*
		 * We want to be able to clean up this iml later so
		 * we give it a name and treat it like any other acl.
		 */
		sprintf(name, "__internal_%p", yyvsp[-1].iml);
		define_acl(savestr(name, 1), yyvsp[-1].iml);
  		yyval.ime = new_ip_match_indirect(yyvsp[-1].iml);
	}
break;
case 198:
#line 1269 "ns_parser.y"
{
		ip_match_list iml;

		iml = lookup_acl(yyvsp[0].cp);
		if (iml == NULL) {
			parser_error(0, "unknown ACL '%s'", yyvsp[0].cp);
			yyval.ime = NULL;
		} else
			yyval.ime = new_ip_match_indirect(iml);
		freestr(yyvsp[0].cp);
	}
break;
case 199:
#line 1287 "ns_parser.y"
{
		struct dst_key *dst_key;
		char *key_name;

		key_name = canonical_name(yyvsp[0].cp);
		if (key_name == NULL) {
			parser_error(0, "can't make key name '%s' canonical",
				     yyvsp[0].cp);
			yyval.keyi = NULL;
		} else {
			dst_key = lookup_key(key_name);
			if (dst_key == NULL) {
				parser_error(0, "unknown key '%s'", key_name);
				yyval.keyi = NULL;
			} else
				yyval.keyi = dst_key;
			freestr(key_name);
		}
		freestr(yyvsp[0].cp);
	}
break;
case 200:
#line 1310 "ns_parser.y"
{
		if (yyvsp[0].keyi == NULL)
			parser_error(0, "empty key not added to server list ");
		else
			add_server_key_info(current_server, yyvsp[0].keyi);
	}
break;
case 204:
#line 1324 "ns_parser.y"
{
		current_algorithm = NULL;
		current_secret = NULL;
	}
break;
case 205:
#line 1329 "ns_parser.y"
{
		struct dst_key *dst_key;
		char *key_name;

		key_name = canonical_name(yyvsp[-3].cp);
		if (key_name == NULL) {
			parser_error(0, "can't make key name '%s' canonical",
				     yyvsp[-3].cp);
		} else if (lookup_key(key_name) != NULL) {
			parser_error(0, "can't redefine key '%s'", key_name);
			freestr(key_name);
		} else {
			if (current_algorithm == NULL ||
			    current_secret == NULL)  {
				parser_error(0, "skipping bad key '%s'",
					     key_name);
				freestr(key_name);
			} else {
				dst_key = new_key_info(key_name,
						       current_algorithm,
						       current_secret);
				define_key(key_name, dst_key);
				if (reloading && secretkey_info != NULL) {
					free_key_info_list(secretkey_info);
					secretkey_info = NULL;
				}
				if (secretkey_info == NULL)
					secretkey_info = new_key_info_list();
				add_to_key_info_list(secretkey_info, dst_key);
			}
		}
		freestr(yyvsp[-3].cp);
	}
break;
case 206:
#line 1365 "ns_parser.y"
{
		current_algorithm = yyvsp[-1].cp;
		current_secret = yyvsp[0].cp;
	}
break;
case 207:
#line 1370 "ns_parser.y"
{
		current_algorithm = yyvsp[0].cp;
		current_secret = yyvsp[-1].cp;
	}
break;
case 208:
#line 1375 "ns_parser.y"
{
		current_algorithm = NULL;
		current_secret = NULL;
	}
break;
case 209:
#line 1381 "ns_parser.y"
{ yyval.cp = yyvsp[-1].cp; }
break;
case 210:
#line 1384 "ns_parser.y"
{ yyval.cp = yyvsp[-1].cp; }
break;
case 211:
#line 1392 "ns_parser.y"
{
		if (lookup_acl(yyvsp[-3].cp) != NULL) {
			parser_error(0, "can't redefine ACL '%s'", yyvsp[-3].cp);
			freestr(yyvsp[-3].cp);
		} else
			define_acl(yyvsp[-3].cp, yyvsp[-1].iml);
	}
break;
case 212:
#line 1406 "ns_parser.y"
{
		int sym_type;
		symbol_value value;
		char *zone_name;

		if (!seen_options)
			parser_error(0,
             "no options statement before first zone; using previous/default");
		sym_type = SYM_ZONE | (yyvsp[0].num & 0xffff);
		value.pointer = NULL;
		zone_name = canonical_name(yyvsp[-1].cp);
		if (zone_name == NULL) {
			parser_error(0, "can't make zone name '%s' canonical",
				     yyvsp[-1].cp);
			should_install = 0;
			zone_name = savestr("__bad_zone__", 1);
		} else {
			if (lookup_symbol(symtab, zone_name, sym_type, NULL)) {
				should_install = 0;
				parser_error(0,
					"cannot redefine zone '%s' class %d",
					     zone_name, yyvsp[0].num);
			} else {
				should_install = 1;
				define_symbol(symtab, savestr(zone_name, 1),
					      sym_type, value,
					      SYMBOL_FREE_KEY);
			}
		}
		freestr(yyvsp[-1].cp);
		current_zone = begin_zone(zone_name, yyvsp[0].num); 
	}
break;
case 213:
#line 1439 "ns_parser.y"
{
		end_zone(current_zone, should_install);
	}
break;
case 216:
#line 1449 "ns_parser.y"
{
		yyval.num = C_IN;
	}
break;
case 217:
#line 1453 "ns_parser.y"
{
		symbol_value value;

		if (lookup_symbol(constants, yyvsp[0].cp, SYM_CLASS, &value))
			yyval.num = value.integer;
		else {
			/* the zone validator will give the error */
			yyval.num = C_NONE;
		}
		freestr(yyvsp[0].cp);
	}
break;
case 218:
#line 1467 "ns_parser.y"
{
		yyval.s_int = Z_MASTER;
	}
break;
case 219:
#line 1471 "ns_parser.y"
{
		yyval.s_int = Z_SLAVE;
	}
break;
case 220:
#line 1475 "ns_parser.y"
{
		yyval.s_int = Z_HINT;
	}
break;
case 221:
#line 1479 "ns_parser.y"
{
		yyval.s_int = Z_STUB;
	}
break;
case 222:
#line 1483 "ns_parser.y"
{
		yyval.s_int = Z_FORWARD;
	}
break;
case 225:
#line 1493 "ns_parser.y"
{
		if (!set_zone_type(current_zone, yyvsp[0].s_int))
			parser_warning(0, "zone type already set; skipping");
	}
break;
case 226:
#line 1498 "ns_parser.y"
{
		if (!set_zone_filename(current_zone, yyvsp[0].cp))
			parser_warning(0,
				       "zone filename already set; skipping");
	}
break;
case 227:
#line 1504 "ns_parser.y"
{
                if (!set_zone_ixfr_file(current_zone, yyvsp[0].cp))
                        parser_warning(0,
                                       "zone ixfr data base already set; skipping");
    }
break;
case 228:
#line 1510 "ns_parser.y"
{
                if (!set_zone_ixfr_tmp(current_zone, yyvsp[0].cp))
                        parser_warning(0,
                                       "zone ixfr temp filename already set; skipping");
    }
break;
case 229:
#line 1516 "ns_parser.y"
{
		set_zone_master_port(current_zone, yyvsp[-3].us_int);
	}
break;
case 230:
#line 1520 "ns_parser.y"
{
		set_zone_transfer_source(current_zone, yyvsp[0].ip_addr);
	}
break;
case 231:
#line 1524 "ns_parser.y"
{
		if (!set_zone_checknames(current_zone, (enum severity)yyvsp[0].s_int))
			parser_warning(0,
	                              "zone checknames already set; skipping");
	}
break;
case 232:
#line 1530 "ns_parser.y"
{
		if (!set_zone_update_acl(current_zone, yyvsp[-1].iml))
			parser_warning(0,
				      "zone update acl already set; skipping");
	}
break;
case 233:
#line 1536 "ns_parser.y"
{
		if (!set_zone_query_acl(current_zone, yyvsp[-1].iml))
			parser_warning(0,
				      "zone query acl already set; skipping");
	}
break;
case 234:
#line 1542 "ns_parser.y"
{
		if (!set_zone_transfer_acl(current_zone, yyvsp[-1].iml))
			parser_warning(0,
				    "zone transfer acl already set; skipping");
	}
break;
case 236:
#line 1549 "ns_parser.y"
{
		struct zoneinfo *zp = current_zone.opaque;
		if (zp->z_fwdtab) {
                	free_forwarders(zp->z_fwdtab);
			zp->z_fwdtab = NULL;
		}

	}
break;
case 238:
#line 1559 "ns_parser.y"
{
		if (!set_zone_transfer_time_in(current_zone, yyvsp[0].num*60))
			parser_warning(0,
		       "zone max transfer time (in) already set; skipping");
	}
break;
case 239:
#line 1565 "ns_parser.y"
{
		set_zone_max_log_size_ixfr(current_zone, yyvsp[0].num);
        }
break;
case 240:
#line 1569 "ns_parser.y"
{
		set_zone_notify(current_zone, yyvsp[0].num);
	}
break;
case 241:
#line 1573 "ns_parser.y"
{
		set_zone_maintain_ixfr_base(current_zone, yyvsp[0].num);
	}
break;
case 242:
#line 1577 "ns_parser.y"
{
		/* flags proto alg key */
		set_zone_pubkey(current_zone, yyvsp[-3].num, yyvsp[-2].num, yyvsp[-1].num, yyvsp[0].cp);

	}
break;
case 244:
#line 1584 "ns_parser.y"
{
		 set_zone_dialup(current_zone, yyvsp[0].num);
	}
break;
case 246:
#line 1591 "ns_parser.y"
{
		/* nothing */
	}
break;
case 247:
#line 1595 "ns_parser.y"
{
		/* nothing */
	}
break;
case 248:
#line 1601 "ns_parser.y"
{
	  	add_zone_master(current_zone, yyvsp[0].ip_addr);
	}
break;
case 251:
#line 1611 "ns_parser.y"
{
		/* nothing */
	}
break;
case 252:
#line 1615 "ns_parser.y"
{
		/* nothing */
	}
break;
case 253:
#line 1621 "ns_parser.y"
{
	  	add_zone_notify(current_zone, yyvsp[0].ip_addr);
	}
break;
case 254:
#line 1627 "ns_parser.y"
{
		set_zone_boolean_option(current_zone, OPTION_FORWARD_ONLY, 1);
	}
break;
case 255:
#line 1631 "ns_parser.y"
{
		set_zone_boolean_option(current_zone, OPTION_FORWARD_ONLY, 0);
	}
break;
case 256:
#line 1637 "ns_parser.y"
{
		set_zone_forward(current_zone);
	}
break;
case 258:
#line 1644 "ns_parser.y"
{
		/* nothing */
	}
break;
case 259:
#line 1648 "ns_parser.y"
{
		/* nothing */
	}
break;
case 260:
#line 1654 "ns_parser.y"
{
	  	add_zone_forwarder(current_zone, yyvsp[0].ip_addr);
	}
break;
case 261:
#line 1664 "ns_parser.y"
{
	}
break;
case 262:
#line 1668 "ns_parser.y"
{
		/* nothing */
	}
break;
case 263:
#line 1672 "ns_parser.y"
{
		/* nothing */
	}
break;
case 264:
#line 1677 "ns_parser.y"
{
		/* name flags proto alg key */
		set_trusted_key(yyvsp[-4].cp, yyvsp[-3].num, yyvsp[-2].num, yyvsp[-1].num, yyvsp[0].cp);
	}
break;
case 265:
#line 1688 "ns_parser.y"
{
		if (yyvsp[0].num < 0 || yyvsp[0].num > 65535) {
		  	parser_warning(0, 
			  "invalid IP port number '%d'; setting port to 0",
			               yyvsp[0].num);
			yyvsp[0].num = 0;
		} else
			yyval.us_int = htons(yyvsp[0].num);
	}
break;
#line 2973 "y.tab.c"
    }
    yystk.ssp -= yym;
    yystate = *yystk.ssp;
    yystk.vsp -= yym;
    yym = yylhs[yyn];
    yych = yychar;
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystk.ssp = YYFINAL;
        *++yystk.vsp = yyval;
        if (yych < 0)
        {
            if ((yych = YYLEX) < 0) yych = 0;
            yychar = yych;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yych <= YYMAXTOKEN) yys = yyname[yych];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yych, yys);
            }
#endif
        }
        if (yych == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystk.ssp, yystate);
#endif
    if (yystk.ssp >= yystk.sslim && yygrowstack(&yystk))
        goto yyoverflow;
    *++yystk.ssp = yystate;
    *++yystk.vsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    YYFREESTACK(&yystk);
    return (1);
yyaccept:
    YYFREESTACK(&yystk);
    return (0);
}
