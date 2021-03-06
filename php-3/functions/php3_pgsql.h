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
   | Authors: Jouni Ahto                                                  |
   +----------------------------------------------------------------------+
 */
 
/* $Id: php3_pgsql.h,v 1.10 1998/02/28 20:50:40 zeev Exp $ */

#ifndef _PHP3_PGSQL_H
#define _PHP3_PGSQL_H

#if COMPILE_DL
#undef HAVE_PGSQL
#define HAVE_PGSQL 1
#endif

#if HAVE_PGSQL

extern php3_module_entry pgsql_module_entry;
#define pgsql_module_ptr &pgsql_module_entry

extern int php3_minit_pgsql(INITFUNCARG);
extern int php3_rinit_pgsql(INITFUNCARG);
extern void php3_pgsql_connect(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_pconnect(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_close(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_dbname(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_error_message(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_options(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_port(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_tty(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_host(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_exec(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_num_rows(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_num_fields(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_cmdtuples(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_field_name(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_field_size(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_field_type(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_field_number(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_result(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_data_length(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_data_isnull(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_free_result(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_last_oid(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_lo_create(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_lo_unlink(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_lo_open(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_lo_close(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_lo_read(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_lo_write(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_pgsql_lo_readall(INTERNAL_FUNCTION_PARAMETERS);

#include <libpq-fe.h>
#include <libpq/libpq-fs.h>

typedef struct pgLofp {
        PGconn *conn;
	int lofd;
} pgLofp;

typedef struct {
	long default_link;
	long num_links,num_persistent;
	long max_links,max_persistent;
	long allow_persistent;
	int le_link,le_plink,le_result,le_lofp,le_string;
} pgsql_module;

extern pgsql_module php3_pgsql_module;

#else

#define pgsql_module_ptr NULL

#endif

#endif /* _PHP3_PGSQL_H */
