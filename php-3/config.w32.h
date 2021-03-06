/* config.w32.h.  Configure file for win32 platforms  */
/* tested only with MS Visual C++ V5 */

/* should be added to runtime config*/
#define PHP3_URL_FOPEN 1

/* ----------------------------------------------------------------
   The following are defaults for run-time configuration
   ---------------------------------------------------------------*/

#define PHP_SAFE_MODE 0
#define MAGIC_QUOTES 0
/* This is the default configuration file to read */
#define CONFIGURATION_FILE_PATH "php3.ini"
#define USE_CONFIG_FILE 1

/* Undefine if you want stricter XML/SGML compliance by default */
/* this disables "<?expression?>" and "<?=expression?>" */
#define DEFAULT_SHORT_OPEN_TAG 1

/* NOTE Logging may be removed from PHP. Consult the PHP DEV list*/
/* Define if you want to allow logging to a db file or sql database */
#define LOGGING 0

/* Define if you want the logging to go to ndbm/gdbm/flatfile */
#define LOG_DBM 0
#define LOG_DBM_DIR "."
#define LOG_SQL 0

#define PHP_TRACK_VARS 1

/* ----------------------------------------------------------------
   The following defines are for those modules which require
   external libraries to compile.  These will be removed from 
   here in a future beta, as these modules will be moved out to dll's 
   ---------------------------------------------------------------*/
#if defined(COMPILE_DL)
/* Define if you an ndbm compatible library (-ldbm).  */
#define NDBM 1
#define GDBM 0
/* Define if using Berkley DB on Win32, you must define
   NDBM also!  */
#define BSD2 1
/* Define if you have the gdbm library (-lgdbm).  */
/* #undef GDBM */
#define HAVE_LDAP 1
#define HAVE_SNMP 1
#define HAVE_CRYPT 1
#define HAVE_ORACLE 1
#define HAVE_ADABAS 1
#define HAVE_SOLID 1
#define HAVE_MYSQL 1
#define HAVE_MSQL 1
#define HAVE_PGSQL 1
#define HAVE_SYBASE 1
#define HAVE_FILEPRO 1
#define DBASE 1

/* Define if you have the gd library (-lgd).  */
#define HAVE_LIBGD 1

#else
#define HAVE_SNMP 0
#define HAVE_MYSQL 0

#define HAVE_LDAP 0
#define DBASE 0
#define NDBM 0
#define GDBM 0
#define BSD2 0
#define HAVE_CRYPT 0
#define HAVE_ORACLE 0
#define HAVE_ADABAS 0
#define HAVE_SOLID 0
#define HAVE_MSQL 0
#define HAVE_PGSQL 0
#define HAVE_SYBASE 0
#define HAVE_LIBGD 0
#define HAVE_FILEPRO 0
#endif
/* ----------------------------------------------------------------
   The following may or may not be (or need to be) ported to the
   windows environment.
   ---------------------------------------------------------------*/

/* Define if you have the link function.  */
#define HAVE_LINK 0

/* Define if you have the lockf function.  */
/* #undef HAVE_LOCKF */

/* Define if you have the lrand48 function.  */
/* #undef HAVE_LRAND48 */

/* Define if you have the srand48 function.  */
/* #undef HAVE_SRAND48 */

/* Define if you have the symlink function.  */
#define HAVE_SYMLINK 0

/* Define if you have the usleep function.  */
#define HAVE_USLEEP 1

#define NEED_ISBLANK 1
/* ----------------------------------------------------------------
   The following may be changed and played with right now, but
   will move to the "don't touch" list below eventualy.
   ---------------------------------------------------------------*/

/*#define APACHE 0 defined in preprocessor section*/


/* ----------------------------------------------------------------
   The following should never need to be played with
   Functions defined to 0 or remarked out are either already
   handled by the standard VC libraries, or are no longer needed, or
   simply will/can not be ported.

   DONT TOUCH!!!!!  Unless you realy know what your messing with!
   ---------------------------------------------------------------*/

#define HAVE_SETITIMER 0
#define HAVE_IODBC 0 /*getting rid of old odbc*/
#define HAVE_UODBC 1
#define HAVE_LIBDL 1
#define HAVE_SENDMAIL 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_PUTENV 1
#define PHP_DEBUGGER 1


/* Define if you have Netscapes LDAP sdk v1.1 */
#define HAVE_NSLDAP 0

/* Define if you have the flock function.  */
#define HAVE_FLOCK 1

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
/* #undef HAVE_ALLOCA_H */

/* Define if you have <sys/time.h> */
#define HAVE_SYS_TIME_H 0

/* Define if you have <signal.h> */
#define HAVE_SIGNAL_H 1

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if your struct stat has st_blksize.  */
#define HAVE_ST_BLKSIZE 0

/* Define if your struct stat has st_blocks.  */
#define HAVE_ST_BLOCKS 0

/* Define if your struct stat has st_rdev.  */
#define HAVE_ST_RDEV 1

/* Define if utime(file, NULL) sets file's timestamp to the present.  */
#define HAVE_UTIME_NULL 1

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

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

/* Define both of these if you want the bundled REGEX library */
#define REGEX 1
#define HSREGEX 1

/* Define if you have the gcvt function.  */
#define HAVE_GCVT 1

/* Define if you have the getlogin function.  */
#define HAVE_GETLOGIN 1

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the memmove function.  */
#define HAVE_MEMMOVE 1

/* Define if you have the putenv function.  */
#define HAVE_PUTENV 1

/* Define if you have the regcomp function.  */
#define HAVE_REGCOMP 1

/* Define if you have the setlocale function.  */
#define HAVE_SETLOCALE 1

/* Define if you have the setvbuf function.  */
#define HAVE_SETVBUF 1

/* Define if you have the snprintf function.  */
#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1
/* Define if you have the strcasecmp function.  */
#define HAVE_STRCASECMP 1

/* Define if you have the strdup function.  */
#define HAVE_STRDUP 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strstr function.  */
#define HAVE_STRSTR 1

/* Define if you have the tempnam function.  */
#define HAVE_TEMPNAM 1

/* Define if you have the utime function.  */
#define HAVE_UTIME 1

/* Define if you have the <crypt.h> header file.  */
/* #undef HAVE_CRYPT_H */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 0

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <grp.h> header file.  */
#define HAVE_GRP_H 0

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <pwd.h> header file.  */
#define HAVE_PWD_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/socket.h> header file.  */
#define HAVE_SYS_SOCKET_H 0

/* Define if you have the <sys/wait.h> header file.  */
#define HAVE_SYS_WAIT_H 0

/* Define if you have the <syslog.h> header file.  */
#define HAVE_SYSLOG_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 0

/* Define if you have the crypt library (-lcrypt).  */
/* #undef HAVE_LIBCRYPT 0 */

/* Define if you have the dl library (-ldl).  */
#define HAVE_LIBDL 1

/* Define if you have the m library (-lm).  */
#define HAVE_LIBM 1

/* Define if you have the nsl library (-lnsl).  */
/* #undef HAVE_LIBNSL */

/* Define if you have the socket library (-lsocket).  */
/* #undef HAVE_LIBSOCKET */

/* Define if you have the cuserid function.  */
#define HAVE_CUSERID 0

/* Define if you have the rint function.  */
#undef HAVE_RINT
