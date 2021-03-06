/* Defines and types for Windows 95/NT */
#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <malloc.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
typedef int uid_t;
typedef int gid_t;
typedef int mode_t;
typedef char * caddr_t;
#define lstat(x, y) stat(x, y)
#define		_IFIFO	0010000	/* fifo */
#define		_IFBLK	0060000	/* block special */
#define		_IFLNK	0120000	/* symbolic link */
#define S_IFIFO		_IFIFO
#define S_IFBLK		_IFBLK
#define	S_IFLNK		_IFLNK
#define pclose		_pclose
#define popen		_popen
#define chdir		_chdir
#define mkdir(a,b)	_mkdir(a)
#define rmdir		_rmdir
#define getpid		_getpid
#if !(APACHE)
#define sleep(s)	_sleep(s*1000)
#endif
#define getcwd		_getcwd
#define snprintf	_snprintf
#define off_t		_off_t
#define vsnprintf	_vsnprintf
typedef unsigned int uint;
#if !NSAPI
#define strcasecmp(s1, s2) stricmp(s1, s2)
#define strncasecmp(s1, s2, n) strnicmp(s1, s2, n)
typedef int pid_t;
#endif

/* missing in vc5 math.h */
#define M_PI             3.14159265358979323846
#define M_TWOPI         (M_PI * 2.0)
#define M_PI_2           1.57079632679489661923
#define M_PI_4           0.78539816339744830962

/* This is only for VC Standard edition, undefine for
 * others.
 */
#if !DEBUG
#define inline  __inline
#endif

/* General Windows stuff */
#define WINDOWS 1

/* Prevent use of VC5 OpenFile function */
#define NOOPENFILE

/* sendmail is built-in */
#ifdef PHP_PROG_SENDMAIL
#undef PHP_PROG_SENDMAIL
#define PHP_PROG_SENDMAIL "Built in mailer"
#endif

#define CONVERT_TO_WIN_FS(Filename) 			\
{									   			\
	char *stemp;				   			    \
	if (Filename)								\
		for (stemp = Filename; *stemp; stemp++) \
			if ( *stemp	== '/')				    \
				*stemp = '\\';				    \
}									 
