/*
   +----------------------------------------------------------------------+
   | PHP HTML Embedded Scripting Language Version 3.0                     |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997,1998 PHP Development Team (See Credits file)      |
   +----------------------------------------------------------------------+
   | This program is free software; you can redistribute it and/or modify |
   | it under the terms of the GNU General Public License as published by |
   | the Free Software Foundation; either version 2 of the License, or    |
   | (at your option) any later version.                                  |
   |                                                                      |
   | This program is distributed in the hope that it will be useful,      |
   | but WITHOUT ANY WARRANTY; without even the implied warranty of       |
   | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        |
   | GNU General Public License for more details.                         |
   |                                                                      |
   | You should have received a copy of the GNU General Public License    |
   | along with this program; if not, write to the Free Software          |
   | Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.            |
   +----------------------------------------------------------------------+
   | Authors:                                                             |
   |                                                                      |
   +----------------------------------------------------------------------+
 */
#ifndef _TLS_H_
#define _TLS_H_
#if WIN32|WINNT
#include "win32/pwd.h"
#include "win32/sendmail.h"
#include <winsock.h>
#endif

#include "alloc.h"
#include "functions/head.h"
#include "functions/number.h"
#include <sys/stat.h>
#if USE_SAPI
#include "serverapi/sapi.h"
#endif

typedef struct php3_global_struct{
	/*all globals must be here*/
	/*alloc.c*/

	int saved_umask;

	mem_header *head;
	/*debbuger.c*/
	char *debugger_host;
	long debugger_port;
	long debugger_default;
	int debugger_on;
	int debug_socket;
	char *myhostname;
	int mypid;
	char *currenttime;
	char debug_timebuf[50]; /*STATIC VAR*/
	/*getopt.c*/
	char *optarg;
	int optind;
	int opterr;
	int optopt;
	/*file.c*/
	int pclose_ret;
	int wsa_fp;
	/*internal_functions.c*/
	HashTable list_destructors;
	HashTable module_registry;
	/*language-parser.tab.c*/
	HashTable symbol_table;
	HashTable function_table;
	HashTable include_names;
	TokenCacheManager token_cache_manager;
	Stack css;
	Stack for_stack;
	Stack input_source_stack;
	Stack function_state_stack;     
	Stack switch_stack;
	Stack variable_unassign_stack; 
	HashTable *active_symbol_table;  
	int Execute;  
	int ExecuteFlag;
	int current_lineno;
	int include_count;
	FunctionState function_state;
	char *class_name;
	HashTable *class_symbol_table;
	YYSTYPE return_value,globals;
	unsigned int param_index;
	YYSTYPE *array_ptr;
	/*list.c*/
	HashTable list;
	HashTable plist;
	/*main.c*/
	unsigned int max_execution_time;
	int error_reporting;
	int tmp_error_reporting;
	int initialized;
	int module_initialized;
	int shutdown_requested;
	int phplineno;
	int in_eval;
	int php3_display_source;
	int php3_preprocess;
#if APACHE
	request_rec *php3_rqst;
#endif
#if USE_SAPI
	struct sapi_request_info *sapi_rqst;
#endif
#if WIN32|WINNT
	unsigned int wintimer_counter;
	unsigned int wintimer;
	unsigned int timerstart;
#endif
	FILE *phpin;
	/*request_info.c*/
	php3_request_info request_info;
	/*token_cache.c*/
	YYSTYPE phplval;
	TokenCache *tc; /*active token cache */
	
	/*Functions*/
	/*bc math*/
	long bc_precision;
	bc_num _zero_;
	bc_num _one_;
	bc_num _two_;
	/*browscap.c*/
	HashTable browser_hash;
	char *lookup_browser_name;
	YYSTYPE *found_browser_entry;
	/*dir.c*/
	int dirp_id;
	int le_dirp;
	/*file.c*/
	int fgetss_state;
	int le_fp;
	int le_pp;
	/*filestat.c*/
	char *CurrentStatFile;
#if MSVC5
	unsigned int CurrentStatLength;
#else
	int CurrentStatLength;
#endif
	struct stat sb;
	/*formated_print.c*/
	char cvt_buf[80]; /*STATIC VAR*/
	/*head.c*/
	int php3_HeaderPrinted;
	int php3_PrintHeader;
	CookieList *top;
	char *cont_type;
	int header_called;
	/*info.c*/
#if APACHE
	module *top_module;
#endif
	/*pageinfo.c*/
	long page_uid;
	long page_inode;
	long page_mtime;
	/*post.c*/
	int php3_track_vars;
	/*strings.h*/
	char *strtok_string;
	char *strtok_pos1; /*STATIC VAR*/
	char *strtok_pos2; /*STATIC VAR*/
#ifndef HAVE_STRERROR
	char str_ebuf[40]; /*STATIC VAR*/
#endif	
#if WIN32|WINNT
	/*pwd.c*/
	struct passwd pw;	/* should we return a malloc()'d structure   */
	/*sendmail.c*/
	char Buffer[MAIL_BUFFER_SIZE]; 
	SOCKET sc;
	WSADATA Data;
	struct hostent *adr;
	SOCKADDR_IN sock_in;
	int WinsockStarted;
	char *AppName;
	char MailHost[HOST_NAME_LEN];
	char LocalHost[HOST_NAME_LEN];
	/*winsyslog.c*/
	char *loghdr;		/* log file header string */
	HANDLE loghdl;	/* handle of event source */
	/*time.c*/
	unsigned int proftimer,virttimer,realtimer;
	LPMSG phpmsg;
	/*winutil*/
	char Win_Error_msg[256];
#endif
	/*check for each module if it is compiled staticly
	we should include their globals here.*/
} php3_globals_struct;

#ifdef THREAD_SAFE
extern DWORD TlsIndex;

extern int tls_create(void);
extern int tls_destroy(void);

/* these are from the flex scanner */
#ifndef YY_TLS_VARS
#define phptext php_gbl->text
#define phpleng php_gbl->leng
extern DWORD phpLexTlsIndex;
#define YY_TLS_VARS flex_globals *php_gbl = TlsGetValue(phpLexTlsIndex)
#endif

/* needed for control structure */
extern int include_file(YYSTYPE *file,int display_source);
extern int conditional_include_file(YYSTYPE *file, YYSTYPE *return_offset INLINE_TLS);
extern void initialize_input_file_buffer(FILE *f);
extern void eval_string(YYSTYPE *str, YYSTYPE *return_offset, int display_source INLINE_TLS);

/* Other needed defines */
#if !defined(COMPILE_DL)
extern int phplex(YYSTYPE *phplval, struct php3_global_struct *php3_globals, flex_globals *php_gbl);
extern int read_next_token(TokenCacheManager *tcm, Token **token, YYSTYPE *phplval,  struct php3_global_struct *php3_globals, flex_globals *php_gbl);
#endif
#else
extern php3_globals_struct *php3_globals;

#endif

extern int tls_startup(void);
extern int tls_shutdown(void);

#endif
