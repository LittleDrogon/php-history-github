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


/* $Id: php3_sybase.h,v 1.29 1998/02/04 11:48:48 zeev Exp $ */

#ifndef _PHP3_SYBASE_H
#define _PHP3_SYBASE_H

#if COMPILE_DL
#undef HAVE_SYBASE
#define HAVE_SYBASE 1
#endif

#if HAVE_SYBASE

extern php3_module_entry sybase_module_entry;
#define sybase_module_ptr &sybase_module_entry

extern int php3_minit_sybase(INITFUNCARG);
extern int php3_rinit_sybase(INITFUNCARG);
extern int php3_mshutdown_sybase(void);
extern int php3_rshutdown_sybase(void);
extern void php3_info_sybase(void);
extern void php3_sybase_connect(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_pconnect(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_close(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_select_db(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_query(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_free_result(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_get_last_message(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_num_rows(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_num_fields(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_fetch_row(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_fetch_array(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_fetch_object(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_data_seek(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_result(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_field_seek(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_min_error_severity(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_min_message_severity(INTERNAL_FUNCTION_PARAMETERS);

extern void php3_sybase_db_query(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_list_fields(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_fetch_lengths(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_fetch_field(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_field_seek(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_free_result(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_field_name(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_field_table(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_field_len(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_field_type(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_sybase_field_flags(INTERNAL_FUNCTION_PARAMETERS);



#include <sybfront.h>
#include <sybdb.h>
#include <syberror.h>

#define coltype(j) dbcoltype(sybase_ptr->link,j)
#define intcol(i) ((int) *(DBINT *) dbdata(sybase_ptr->link,i))
#define smallintcol(i) ((int) *(DBSMALLINT *) dbdata(sybase_ptr->link,i))
#define tinyintcol(i) ((int) *(DBTINYINT *) dbdata(sybase_ptr->link,i))
#define anyintcol(j) (coltype(j)==SYBINT4?intcol(j):(coltype(j)==SYBINT2?smallintcol(j):tinyintcol(j)))
#define charcol(i) ((DBCHAR *) dbdata(sybase_ptr->link,i))
#define floatcol(i) ((float) *(DBFLT8 *) dbdata(sybase_ptr->link,i))

typedef struct {
	long default_link;
	long num_links,num_persistent;
	long max_links,max_persistent;
	long allow_persistent;
	char *appname;
	char *server_message;
	int le_link,le_plink,le_result;
	long min_error_severity,min_message_severity;
	long cfg_min_error_severity,cfg_min_message_severity;
} sybase_module;

typedef struct {
	LOGINREC *login;
	DBPROCESS *link;
	int valid;
} sybase_link;

#define SYBASE_ROWS_BLOCK 128

typedef struct {
	char *name,*column_source;
	int max_length;
} sybase_field;

typedef struct {
	YYSTYPE **data;
	sybase_field *fields;
	sybase_link *sybase_ptr;
	int cur_row,cur_field;
	int num_rows,num_fields;
} sybase_result;


extern sybase_module php3_sybase_module;

#else

#define sybase_module_ptr NULL

#endif

#endif /* _PHP3_SYBASE_H */
