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
   | Authors: Andi Gutmans <andi@php.net>                                 |
   |          Zeev Suraski <bourbon@netvision.net.il>                     |
   +----------------------------------------------------------------------+
 */


/* $Id: main.h,v 1.44 1998/02/01 23:53:06 zeev Exp $ */


#ifndef _MAIN_H
#define _MAIN_H

#define INIT_SYMBOL_TABLE 0x1
#define INIT_TOKEN_CACHE 0x2
#define INIT_CSS 0x4
#define INIT_FOR_STACK 0x8
#define INIT_SWITCH_STACK 0x10
#define INIT_INCLUDE_STACK 0x20
#define INIT_FUNCTION_STATE_STACK 0x40
#define INIT_ENVIRONMENT 0x80
#define INIT_INCLUDE_NAMES_HASH 0x100
#define INIT_FUNCTION_TABLE 0x200
#define INIT_REQUEST_INFO 0x400
#define INIT_FUNCTIONS 0x800
#define INIT_SCANNER 0x1000
#define INIT_MEMORY_MANAGER 0x2000
#define INIT_LIST 0x4000
#define INIT_PLIST 0x8000
#define INIT_CONFIG 0x10000
#define INIT_VARIABLE_UNASSIGN_STACK 0x20000
#define INIT_LIST_DESTRUCTORS 0x40000
#define INIT_MODULE_REGISTRY 0x80000
#define INIT_WINSOCK 0x100000

#define NORMAL_SHUTDOWN 1
#define NO_SHUTDOWN 0
#define ABNORMAL_SHUTDOWN -1

extern int php3_request_startup(INLINE_TLS_VOID);
extern void php3_request_shutdown(void *dummy INLINE_TLS);
extern int php3_module_startup(INLINE_TLS_VOID);
extern void php3_module_shutdown(INLINE_TLS_VOID);

extern int php3_get_lineno(int lineno);
extern char *php3_get_filename(int lineno);

#ifndef THREAD_SAFE
extern int shutdown_requested;
#if (!PHP_ISAPI)
extern char **environ;
#endif
extern int initialized;
#endif

#endif
