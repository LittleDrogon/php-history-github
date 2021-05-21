/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if using alloca.c.  */
/* #undef C_ALLOCA */

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
/* #undef CRAY_STACKSEG_END */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if you have alloca, as a function or macro.  */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#define HAVE_ALLOCA_H 1

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if your struct stat has st_blksize.  */
#define HAVE_ST_BLKSIZE 1

/* Define if your struct stat has st_blocks.  */
#define HAVE_ST_BLOCKS 1

/* Define if your struct stat has st_rdev.  */
#define HAVE_ST_RDEV 1

/* Define if utime(file, NULL) sets file's timestamp to the present.  */
#define HAVE_UTIME_NULL 1

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if your C compiler doesn't accept -c and -o together.  */
/* #undef NO_MINUS_C_MINUS_O */

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
 STACK_DIRECTION > 0 => grows toward higher addresses
 STACK_DIRECTION < 0 => grows toward lower addresses
 STACK_DIRECTION = 0 => direction of growth unknown
 */
/* #undef STACK_DIRECTION */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* This is the default configuration file to read */
#define CONFIGURATION_FILE_PATH "/usr/local/lib"
#define USE_CONFIG_FILE 1

/* Define if you have the gd library (-lgd). */
#define HAVE_LIBGD 1

/* Define if you want safe mode enabled by default. */
#define PHP_SAFE_MODE 0

/* Set to the path to the dir containing safe mode executables */
#define PHP_SAFE_MODE_EXEC_DIR "/usr/local/bin"

/* Define if you want POST/GET/Cookie track variables by default */
#define PHP_TRACK_VARS 0

/* Undefine if you want stricter XML/SGML compliance by default */
/* (this disables "<?expression?>" by default) */
#define DEFAULT_SHORT_OPEN_TAG 1

/* Undefine if you do not want PHP by default to escape "'" */
/* in GET/POST/Cookie data */
#define MAGIC_QUOTES 0

/* Define if you an ndbm compatible library (-ldbm).  */
#define NDBM 0

/* Define if you have the gdbm library (-lgdbm).  */
#define GDBM 1

/* Define both of these if you want the bundled REGEX library */
#define REGEX 1
#define HSREGEX 1

/* Define if you want Solid database support */
#define HAVE_SOLID 0

/* Define if you want to use the supplied dbase library */
#define DBASE 0

/* Define if you have the Oracle database client libraries */
#define HAVE_ORACLE 0

/* Define if you want to use the iODBC database driver */
#define HAVE_IODBC 0

/* Define if you have the AdabasD client libraries */
#define HAVE_ADABAS 0

/* Define if you want the LDAP directory interface */
#define HAVE_LDAP 0

/* Define to use the unified ODBC interface */
#define HAVE_UODBC 0

/* Define if you have libdl (used for dynamic linking) */
#define HAVE_LIBDL 1

/* Define if you have libdnet_stub (used for Sybase support) */
#define HAVE_LIBDNET_STUB 0

/* Define if you have and want to use libcrypt */
#define HAVE_LIBCRYPT 1

/* Define if you have and want to use libnsl */
#define HAVE_LIBNSL 0

/* Define if you have and want to use libsocket */
#define HAVE_LIBSOCKET 0

/* Define if you have the sendmail program available */
#define HAVE_SENDMAIL 1

/* Define if you are compiling PHP as an Apache module */
#define APACHE 0

#define HAVE_SYBASE 0
#define HAVE_SYBASE_CT 0

#ifndef HAVE_MYSQL
#define HAVE_MYSQL 0
#endif

#ifndef HAVE_MSQL
#define HAVE_MSQL 0
#endif

#ifndef HAVE_PGSQL
#define HAVE_PGSQL 0
#endif

#define MSQL1 0
#define HAVE_FILEPRO 0
#define HAVE_SOLID 0
#define DEBUG 1

/* Define if you want to enable the PHP TCP/IP debugger (experimental) */
#define PHP_DEBUGGER 0

/* Define if you want to enable bc style precision math support */
#define WITH_BCMATH 0

/* Define if you want to prevent the CGI from working unless REDIRECT_STATUS is defined in the environment */
#define FORCE_CGI_REDIRECT 0

/* Define if you want to enable memory limit support */
#define MEMORY_LIMIT 1

/* Define if you want include() and other functions to be able to open
 * http and ftp URLs as files.
 */
#define PHP3_URL_FOPEN 1

/* Define if you have broken header files like SunOS 4 */
#define MISSING_FCLOSE_DECL 0

/* Define if you have the crypt function.  */
#define HAVE_CRYPT 1

/* Define if you have the cuserid function.  */
#define HAVE_CUSERID 1

/* Define if you have the flock function.  */
#define HAVE_FLOCK 1

/* Define if you have the gcvt function.  */
#define HAVE_GCVT 1

/* Define if you have the getlogin function.  */
#define HAVE_GETLOGIN 1

/* Define if you have the getopt function.  */
#define HAVE_GETOPT 1

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the link function.  */
#define HAVE_LINK 1

/* Define if you have the lockf function.  */
#define HAVE_LOCKF 1

/* Define if you have the lrand48 function.  */
#define HAVE_LRAND48 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the memmove function.  */
#define HAVE_MEMMOVE 1

/* Define if you have the putenv function.  */
#define HAVE_PUTENV 1

/* Define if you have the random function.  */
#define HAVE_RANDOM 1

/* Define if you have the regcomp function.  */
#define HAVE_REGCOMP 1

/* Define if you have the rint function.  */
#define HAVE_RINT 1

/* Define if you have the setitimer function.  */
#define HAVE_SETITIMER 1

/* Define if you have the setlocale function.  */
#define HAVE_SETLOCALE 1

/* Define if you have the setvbuf function.  */
#define HAVE_SETVBUF 1

/* Define if you have the snprintf function.  */
#define HAVE_SNPRINTF 1

/* Define if you have the srand48 function.  */
#define HAVE_SRAND48 1

/* Define if you have the srandom function.  */
#define HAVE_SRANDOM 1

/* Define if you have the strcasecmp function.  */
#define HAVE_STRCASECMP 1

/* Define if you have the strdup function.  */
#define HAVE_STRDUP 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strstr function.  */
#define HAVE_STRSTR 1

/* Define if you have the symlink function.  */
#define HAVE_SYMLINK 1

/* Define if you have the tempnam function.  */
#define HAVE_TEMPNAM 1

/* Define if you have the usleep function.  */
#define HAVE_USLEEP 1

/* Define if you have the utime function.  */
#define HAVE_UTIME 1

/* Define if you have the vsnprintf function.  */
#define HAVE_VSNPRINTF 1

/* Define if you have the <crypt.h> header file.  */
#define HAVE_CRYPT_H 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <errmsg.h> header file.  */
/* #undef HAVE_ERRMSG_H */

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <grp.h> header file.  */
#define HAVE_GRP_H 1

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <netinet/in.h> header file.  */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <pwd.h> header file.  */
#define HAVE_PWD_H 1

/* Define if you have the <signal.h> header file.  */
#define HAVE_SIGNAL_H 1

/* Define if you have the <stdarg.h> header file.  */
#define HAVE_STDARG_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/socket.h> header file.  */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/varargs.h> header file.  */
/* #undef HAVE_SYS_VARARGS_H */

/* Define if you have the <sys/wait.h> header file.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the <syslog.h> header file.  */
#define HAVE_SYSLOG_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the m library (-lm).  */
#define HAVE_LIBM 1
