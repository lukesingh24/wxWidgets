#ifndef _CONFIG_H
#define _CONFIG_H

# if    !defined(WINDOWS) && !defined(WIN32_SYSTEM) && !defined(OS2)
#  define       _UNIX_

#  include      <stdlib.h>
#  include      <sys/types.h>
#  include      <string.h>
#  include      <stdio.h>

#  define       MEM_ALLOC(size) (malloc((size_t)(size)))
#  define       MEM_FREE(ptr)   {if(ptr) free(ptr);}

#  define       STRCPY(t, s)    (strcpy((char*)(t), (char*)(s)))
#  define       STRNCPY(t,s,n)  (strncpy((char*)(t), (char*)(s), (size_t)(n)))
#  define       STRCAT(t, s)    (strcat((char*)(t), (char*)(s)))
#  define       STRNCAT(t,s,n)  (strncat((char*)(t), (char*)(s), (size_t)(n)))
#  define       STREQ(a, b)     (strcmp((char*)(a), (char*)(b)) == 0)
#  define       STRLEN(str)     ((str)? strlen((char*)(str)):0)

#  define       EXPORT
#  define       CALLBACK
#  define       FAR

   typedef      signed short    SSHOR;
   typedef      short           WORD;
   typedef      long            DWORD;

   typedef      WORD            WPARAM;
   typedef      DWORD           LPARAM;
// KB: I don't see where HWND and BOOL could get defined before here,
// but putting in the #ifndef's solved the compilation problem on Solaris.
#ifndef HWND
typedef      void*           HWND;
#endif
#ifndef BOOL
typedef      int             BOOL;
#endif

# endif /* _UNIX_ */

# if    defined(WINDOWS) || defined(WIN32_SYSTEM)

#  include      <windows.h>
#  include      <windowsx.h>

#  ifdef        _MSVC_
#   define      MEM_ALLOC(size) (fmalloc((size_t)(size)))
#   define      MEM_FREE(ptr)   ((ptr)? ffree((PTR)(ptr)):0))
#   define      STRCPY(t, s)    (fstrcpy((char FAR*)(t), (char FAR*)(s)))
#   define      STRNCPY(t,s,n)  (fstrncpy((char FAR*)(t), (char FAR*)(s), (size_t)(n)))
#   define      STRLEN(str)     ((str)? fstrlen((char FAR*)(str)):0)
#   define      STREQ(a, b)     (fstrcmp((char FAR*)(a), (char FAR*)(b) == 0)
#  endif

#  ifdef        _BORLAND_
#   define      MEM_ALLOC(size) (farmalloc((unsigned long)(size))
#   define      MEM_FREE(ptr)   ((ptr)? farfree((void far*)(ptr)):0)
#   define      STRCPY(t, s)    (_fstrcpy((char FAR*)(t), (char FAR*)(s)))
#   define      STRNCPY(t,s,n)  (_fstrncpy((char FAR*)(t), (char FAR*)(s), (size_t)(n)))
#   define      STRLEN(str)     ((str)? _fstrlen((char FAR*)(str)):0)
#   define      STREQ(a, b)     (_fstrcmp((char FAR*)(a), (char FAR*)(b) == 0)
#  endif

# endif /* WINDOWS */

# if    defined(OS2)

#  include      <stdlib.h>
#  include      <stdio.h>
#  include      <string.h>
#  include      <memory.h>
#  define INCL_DOSMODULEMGR                 /* Module Manager values */
#  define INCL_DOSERRORS                    /* Error values          */
#  include      <os2.h>

#  ifndef FAR
#    define     FAR
#  endif

#  define       MEM_ALLOC(size) (malloc((size_t)(size)))
#  define       MEM_FREE(ptr)   (free((ptr)))
#  define       STRCPY(t, s)    (strcpy((char*)(t), (char*)(s)))
#  define       STRNCPY(t,s,n)  (strncpy((char*)(t), (char*)(s), (size_t)(n)))
#  define       STRCAT(t, s)    (strcat((char*)(t), (char*)(s)))
#  define       STRNCAT(t,s,n)  (strncat((char*)(t), (char*)(s), (size_t)(n)))
#  define       STRLEN(str)     ((str)? strlen((char*)(str)):0)
#  define       STREQ(a, b)     (0 == strcmp((char *)(a), (char *)(b)))

   typedef      signed short    SSHOR;
   typedef      short           WORD;
   typedef      long            DWORD;

   typedef      WORD            WPARAM;
   typedef      DWORD           LPARAM;

# endif /* OS2 */

# define        SYSERR          (-1)

# ifndef        NULL
#   define      NULL            ((void FAR*)0UL)
# endif

#endif
