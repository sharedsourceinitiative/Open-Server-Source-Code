/*
**  pth_vers.c -- Version Information for GNU Pth (syntax: C/C++)
**  [automatically generated and maintained by GNU shtool]
*/

#ifdef _PTH_VERS_C_AS_HEADER_

#ifndef _PTH_VERS_C_
#define _PTH_VERS_C_

#define PTH_INTERNAL_VERSION 0x200200

typedef struct {
    const int   v_hex;
    const char *v_short;
    const char *v_long;
    const char *v_tex;
    const char *v_gnu;
    const char *v_web;
    const char *v_sccs;
    const char *v_rcs;
} pth_internal_version_t;

extern pth_internal_version_t pth_internal_version;

#endif /* _PTH_VERS_C_ */

#else /* _PTH_VERS_C_AS_HEADER_ */

#define _PTH_VERS_C_AS_HEADER_
#include "pth_vers.c"
#undef  _PTH_VERS_C_AS_HEADER_

pth_internal_version_t pth_internal_version = {
    0x200200,
    "2.0.0",
    "2.0.0 (17-Feb-2003)",
    "This is GNU Pth, Version 2.0.0 (17-Feb-2003)",
    "GNU Pth 2.0.0 (17-Feb-2003)",
    "GNU Pth/2.0.0",
    "@(#)GNU Pth 2.0.0 (17-Feb-2003)",
    "$Id: GNU Pth 2.0.0 (17-Feb-2003) $"
};

#endif /* _PTH_VERS_C_AS_HEADER_ */

