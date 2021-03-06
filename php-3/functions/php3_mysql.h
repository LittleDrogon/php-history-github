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
   | Authors: Zeev Suraski <bourbon@netvision.net.il>                     |
   +----------------------------------------------------------------------+
 */


/* $Id: php3_mysql.h,v 1.16 1998/02/20 22:49:07 shane Exp $ */

#ifndef _PHP3_MYSQL_H
#define _PHP3_MYSQL_H

#if COMPILE_DL
#undef HAVE_MYSQL
#define HAVE_MYSQL 1
#endif

#if HAVE_MYSQL
#ifndef DLEXPORT
#define DLEXPORT
#endif

extern php3_module_entry mysql_module_entry;
#define mysql_module_ptr &mysql_module_entry

extern int php3_minit_mysql(INITFUNCARG);
extern int php3_rinit_mysql(INITFUNCARG);
extern int php3_mshutdown_mysql(void);
extern void php3_info_mysql(void);
extern void php3_mysql_connect(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_pconnect(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_close(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_select_db(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_create_db(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_drop_db(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_query(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_db_query(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_list_dbs(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_list_tables(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_list_fields(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_affected_rows(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_insert_id(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_result(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_num_rows(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_num_fields(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_fetch_row(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_fetch_array(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_fetch_object(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_data_seek(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_fetch_lengths(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_fetch_field(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_field_seek(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_free_result(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_field_name(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_field_table(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_field_len(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_field_type(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_mysql_field_flags(INTERNAL_FUNCTION_PARAMETERS);

typedef struct {
	long default_link;
	long num_links,num_persistent;
	long max_links,max_persistent;
	long allow_persistent;
	int le_result,le_link,le_plink;
	int default_port;
} mysql_module;

#ifndef THREAD_SAFE
extern mysql_module php3_mysql_module;
#endif

#else

#define mysql_module_ptr NULL

#endif

#endif /* _PHP3_MYSQL_H */
