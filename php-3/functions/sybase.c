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
   | php3_sybase_get_column_content() based on code by:                   |
   |                     Muhammad A Muquit <MA_Muquit@fccc.edu>           |
   |                     Rasmus Lerdorf <rasmus@lerdorf.on.ca>            |
   +----------------------------------------------------------------------+
 */
 
/* $Id: sybase.c,v 1.85 1998/02/04 11:52:53 zeev Exp $ */


#ifndef MSVC5
#include "config.h"
#endif
#include "parser.h"
#include "internal_functions.h"
#include "php3_sybase.h"
#include "php3_string.h"

#if HAVE_SYBASE

#include <sybfront.h>
#include <sybdb.h>
#include <syberror.h>
#include "list.h"

#if BROKEN_SYBASE_PCONNECTS
#include "http_log.h"
#endif

function_entry sybase_functions[] = {
	{"sybase_connect",			php3_sybase_connect,			NULL},
	{"sybase_pconnect",			php3_sybase_pconnect,			NULL},
	{"sybase_close",			php3_sybase_close,				NULL},
	{"sybase_select_db",		php3_sybase_select_db,			NULL},
	{"sybase_query",			php3_sybase_query,				NULL},
	{"sybase_free_result",		php3_sybase_free_result,		NULL},
	{"sybase_get_last_message",	php3_sybase_get_last_message,	NULL},
	{"sybase_num_rows",			php3_sybase_num_rows,			NULL},
	{"sybase_num_fields",		php3_sybase_num_fields,			NULL},
	{"sybase_fetch_row",		php3_sybase_fetch_row,			NULL},
	{"sybase_fetch_array",		php3_sybase_fetch_array,		NULL},
	{"sybase_fetch_object",		php3_sybase_fetch_object,		NULL},
	{"sybase_data_seek",		php3_sybase_data_seek,			NULL},
	{"sybase_fetch_field",		php3_sybase_fetch_field,		NULL},
	{"sybase_field_seek",		php3_sybase_field_seek,			NULL},
	{"sybase_result",			php3_sybase_result,				NULL},
	{"sybase_min_error_severity",	php3_sybase_min_error_severity,		NULL},
	{"sybase_min_message_severity",	php3_sybase_min_message_severity,	NULL},
	{"mssql_connect",			php3_sybase_connect,			NULL},
	{"mssql_pconnect",			php3_sybase_pconnect,			NULL},
	{"mssql_close",				php3_sybase_close,				NULL},
	{"mssql_select_db",			php3_sybase_select_db,			NULL},
	{"mssql_query",				php3_sybase_query,				NULL},
	{"mssql_free_result",		php3_sybase_free_result,		NULL},
	{"mssql_get_last_message",	php3_sybase_get_last_message,	NULL},
	{"mssql_num_rows",			php3_sybase_num_rows,			NULL},
	{"mssql_num_fields",		php3_sybase_num_fields,			NULL},
	{"mssql_fetch_row",			php3_sybase_fetch_row,			NULL},
	{"mssql_fetch_array",		php3_sybase_fetch_array,		NULL},
	{"mssql_fetch_object",		php3_sybase_fetch_object,		NULL},
	{"mssql_data_seek",			php3_sybase_data_seek,			NULL},
	{"mssql_fetch_field",		php3_sybase_fetch_field,		NULL},
	{"mssql_field_seek",		php3_sybase_field_seek,			NULL},
	{"mssql_result",			php3_sybase_result,				NULL},
	{"mssql_min_error_severity",	php3_sybase_min_error_severity,		NULL},
	{"mssql_min_message_severity",	php3_sybase_min_message_severity,	NULL},
	{NULL, NULL, NULL}
};

php3_module_entry sybase_module_entry = {
	"Sybase SQL", sybase_functions, php3_minit_sybase, php3_mshutdown_sybase, php3_rinit_sybase, php3_rshutdown_sybase, php3_info_sybase, 0, 0, 0, NULL
};

#if COMPILE_DL
php3_module_entry *get_module() { return &sybase_module_entry; }
#endif

THREAD_LS sybase_module php3_sybase_module;
THREAD_LS static HashTable *resource_list, *resource_plist;


#define CHECK_LINK(link) { if (link==-1) { php3_error(E_WARNING,"Sybase:  A link to the server could not be established"); RETURN_FALSE; } }


/* error handler */
static int php3_sybase_error_handler(DBPROCESS *dbproc,int severity,int dberr,
										int oserr,char *dberrstr,char *oserrstr)
{
	if (severity >= php3_sybase_module.min_error_severity) {
		php3_error(E_WARNING,"Sybase error:  %s (severity %d)",dberrstr,severity);
	}
	return INT_CANCEL;  
}

/* message handler */
static int php3_sybase_message_handler(DBPROCESS *dbproc,DBINT msgno,int msgstate,
										int severity,char *msgtext,char *srvname,
										char *procname,DBUSMALLINT line)
{
	if (severity >= php3_sybase_module.min_message_severity) {
		php3_error(E_WARNING,"Sybase message:  %s (severity %d)",msgtext,severity);
	}
	STR_FREE(php3_sybase_module.server_message);
	php3_sybase_module.server_message = estrdup(msgtext);
	return 0;
}


static int _clean_invalid_results(list_entry *le)
{
	if (le->type == php3_sybase_module.le_result) {
		sybase_link *sybase_ptr = ((sybase_result *) le->ptr)->sybase_ptr;
		
		if (!sybase_ptr->valid) {
			return 1;
		}
	}
	return 0;
}


static void _free_sybase_result(sybase_result *result)
{
	int i,j;
	
	if (result->data) {
		for (i=0; i<result->num_rows; i++) {
			for (j=0; j<result->num_fields; j++) {
				yystype_destructor(&result->data[i][j]);
			}
			efree(result->data[i]);
		}
		efree(result->data);
	}
	
	if (result->fields) {
		for (i=0; i<result->num_fields; i++) {
			STR_FREE(result->fields[i].name);
			STR_FREE(result->fields[i].column_source);
		}
		efree(result->fields);
	}
	efree(result);
}


static void _close_sybase_link(sybase_link *sybase_ptr)
{
	sybase_ptr->valid = 0;
	hash_apply(resource_list,(int (*)(void *))_clean_invalid_results);
	dbclose(sybase_ptr->link);
	dbloginfree(sybase_ptr->login);
	efree(sybase_ptr);
	php3_sybase_module.num_links--;
}


static void _close_sybase_plink(sybase_link *sybase_ptr)
{
	dbclose(sybase_ptr->link);
	dbloginfree(sybase_ptr->login);
	free(sybase_ptr);
	php3_sybase_module.num_persistent--;
	php3_sybase_module.num_links--;
}


int php3_minit_sybase(INITFUNCARG)
{
	char *interface_file;

	if (dbinit()==FAIL) {
		return FAILURE;
	}
	dberrhandle((EHANDLEFUNC) php3_sybase_error_handler);
	dbmsghandle((MHANDLEFUNC) php3_sybase_message_handler);

	if (cfg_get_string("sybase.interface_file",&interface_file)==SUCCESS) {
		dbsetifile(interface_file);
	}
	if (cfg_get_long("sybase.allow_persistent",&php3_sybase_module.allow_persistent)==FAILURE) {
		php3_sybase_module.allow_persistent=1;
	}
	if (cfg_get_long("sybase.max_persistent",&php3_sybase_module.max_persistent)==FAILURE) {
		php3_sybase_module.max_persistent=-1;
	}
	if (cfg_get_long("sybase.max_links",&php3_sybase_module.max_links)==FAILURE) {
		php3_sybase_module.max_links=-1;
	}
	if (cfg_get_long("sybase.min_error_severity",&php3_sybase_module.cfg_min_error_severity)==FAILURE) {
		php3_sybase_module.cfg_min_error_severity=10;
	}
	if (cfg_get_long("sybase.min_message_severity",&php3_sybase_module.cfg_min_message_severity)==FAILURE) {
		php3_sybase_module.cfg_min_message_severity=10;
	}
	
	php3_sybase_module.num_persistent=0;
	php3_sybase_module.le_link = register_list_destructors(_close_sybase_link,NULL);
	php3_sybase_module.le_plink = register_list_destructors(NULL,_close_sybase_plink);
	php3_sybase_module.le_result = register_list_destructors(_free_sybase_result,NULL);
	
	return SUCCESS;
}


int php3_rinit_sybase(INITFUNCARG)
{
	php3_sybase_module.default_link=-1;
	php3_sybase_module.num_links = php3_sybase_module.num_persistent;
	php3_sybase_module.appname = estrndup("PHP 3.0",7);
	php3_sybase_module.server_message = empty_string;
	php3_sybase_module.min_error_severity = php3_sybase_module.cfg_min_error_severity;
	php3_sybase_module.min_message_severity = php3_sybase_module.cfg_min_message_severity;
	return SUCCESS;
}

int php3_mshutdown_sybase(void)
{
	dbexit();
	return SUCCESS;
}

int php3_rshutdown_sybase(void)
{
	efree(php3_sybase_module.appname);
	STR_FREE(php3_sybase_module.server_message);
	return SUCCESS;
}

static void php3_sybase_do_connect(INTERNAL_FUNCTION_PARAMETERS,int persistent)
{
	char *user,*passwd,*host;
	char *hashed_details;
	int hashed_details_length;
	sybase_link sybase,*sybase_ptr;

	resource_list = list;
	resource_plist = plist;
		
	switch(ARG_COUNT(ht)) {
		case 0: /* defaults */
			host=user=passwd=NULL;
			hashed_details_length=6+3;
			hashed_details = (char *) emalloc(hashed_details_length+1);
			strcpy(hashed_details,"sybase___");
			break;
		case 1: {
				YYSTYPE *yyhost;
				
				if (getParameters(ht, 1, &yyhost)==FAILURE) {
					RETURN_FALSE;
				}
				convert_to_string(yyhost);
				host = yyhost->value.strval;
				user=passwd=NULL;
				hashed_details_length = yyhost->strlen+6+3;
				hashed_details = (char *) emalloc(hashed_details_length+1);
				sprintf(hashed_details,"sybase_%s__",yyhost->value.strval);
			}
			break;
		case 2: {
				YYSTYPE *yyhost,*yyuser;
				
				if (getParameters(ht, 2, &yyhost, &yyuser)==FAILURE) {
					RETURN_FALSE;
				}
				convert_to_string(yyhost);
				convert_to_string(yyuser);
				host = yyhost->value.strval;
				user = yyuser->value.strval;
				passwd=NULL;
				hashed_details_length = yyhost->strlen+yyuser->strlen+6+3;
				hashed_details = (char *) emalloc(hashed_details_length+1);
				sprintf(hashed_details,"sybase_%s_%s_",yyhost->value.strval,yyuser->value.strval);
			}
			break;
		case 3: {
				YYSTYPE *yyhost,*yyuser,*yypasswd;
			
				if (getParameters(ht, 3, &yyhost, &yyuser, &yypasswd) == FAILURE) {
					RETURN_FALSE;
				}
				convert_to_string(yyhost);
				convert_to_string(yyuser);
				convert_to_string(yypasswd);
				host = yyhost->value.strval;
				user = yyuser->value.strval;
				passwd = yypasswd->value.strval;
				hashed_details_length = yyhost->strlen+yyuser->strlen+yypasswd->strlen+6+3;
				hashed_details = (char *) emalloc(hashed_details_length+1);
				sprintf(hashed_details,"sybase_%s_%s_%s",yyhost->value.strval,yyuser->value.strval,yypasswd->value.strval); /* SAFE */
			}
			break;
		default:
			WRONG_PARAM_COUNT;
			break;
	}


	/* set a DBLOGIN record */	
	if ((sybase.login=dblogin())==NULL) {
		php3_error(E_WARNING,"Sybase:  Unable to allocate login record");
		RETURN_FALSE;
	}
	
	if (user) {
		DBSETLUSER(sybase.login,user);
	}
	if (passwd) {
		DBSETLPWD(sybase.login,passwd);
	}
	DBSETLAPP(sybase.login,php3_sybase_module.appname);
	sybase.valid = 1;

	if (!php3_sybase_module.allow_persistent) {
		persistent=0;
	}
	if (persistent) {
		list_entry *le;

		/* try to find if we already have this link in our persistent list */
		if (hash_find(plist, hashed_details, hashed_details_length+1, (void **) &le)==FAILURE) {  /* we don't */
			list_entry new_le;

			if (php3_sybase_module.max_links!=-1 && php3_sybase_module.num_links>=php3_sybase_module.max_links) {
				php3_error(E_WARNING,"Sybase:  Too many open links (%d)",php3_sybase_module.num_links);
				efree(hashed_details);
				dbloginfree(sybase.login);
				RETURN_FALSE;
			}
			if (php3_sybase_module.max_persistent!=-1 && php3_sybase_module.num_persistent>=php3_sybase_module.max_persistent) {
				php3_error(E_WARNING,"Sybase:  Too many open persistent links (%d)",php3_sybase_module.num_persistent);
				efree(hashed_details);
				dbloginfree(sybase.login);
				RETURN_FALSE;
			}
			/* create the link */
			if ((sybase.link=dbopen(sybase.login,host))==FAIL) {
				/*php3_error(E_WARNING,"Sybase:  Unable to connect to server:  %s",sybase_error(sybase));*/
				efree(hashed_details);
				dbloginfree(sybase.login);
				RETURN_FALSE;
			}

			if (dbsetopt(sybase.link,DBBUFFER,"2",-1)==FAIL) {
				efree(hashed_details);
				dbloginfree(sybase.login);
				dbclose(sybase.link);
				RETURN_FALSE;
			}

			/* hash it up */
			sybase_ptr = (sybase_link *) malloc(sizeof(sybase_link));
			memcpy(sybase_ptr,&sybase,sizeof(sybase_link));
			new_le.type = php3_sybase_module.le_plink;
			new_le.ptr = sybase_ptr;
			if (hash_update(plist, hashed_details, hashed_details_length+1, (void *) &new_le, sizeof(list_entry),NULL)==FAILURE) {
				free(sybase_ptr);
				efree(hashed_details);
				dbloginfree(sybase.login);
				RETURN_FALSE;
			}
			php3_sybase_module.num_persistent++;
			php3_sybase_module.num_links++;
		} else {  /* we do */
			if (le->type != php3_sybase_module.le_plink) {
#if BROKEN_SYBASE_PCONNECTS
				log_error("PHP/Sybase:  Hashed persistent link is not a Sybase link!",php3_rqst->server);
#endif
				php3_error(E_WARNING,"Sybase:  Hashed persistent link is not a Sybase link!");
				RETURN_FALSE;
			}
			
			sybase_ptr = (sybase_link *) le->ptr;
			/* test that the link hasn't died */
			if (DBDEAD(sybase_ptr->link)==TRUE) {
#if BROKEN_SYBASE_PCONNECTS
				log_error("PHP/Sybase:  Persistent link died, trying to reconnect...",php3_rqst->server);
#endif
				if ((sybase_ptr->link=dbopen(sybase_ptr->login,host))==FAIL) {
#if BROKEN_SYBASE_PCONNECTS
					log_error("PHP/Sybase:  Unable to reconnect!",php3_rqst->server);
#endif
					/*php3_error(E_WARNING,"Sybase:  Link to server lost, unable to reconnect");*/
					hash_del(plist, hashed_details, hashed_details_length+1);
					efree(hashed_details);
					RETURN_FALSE;
				}
#if BROKEN_SYBASE_PCONNECTS
				log_error("PHP/Sybase:  Reconnect successful!",php3_rqst->server);
#endif
				if (dbsetopt(sybase_ptr->link,DBBUFFER,"2",-1)==FAIL) {
#if BROKEN_SYBASE_PCONNECTS
					log_error("PHP/Sybase:  Unable to set required options",php3_rqst->server);
#endif
					hash_del(plist, hashed_details, hashed_details_length+1);
					efree(hashed_details);
					RETURN_FALSE;
				}
			}
		}
		return_value->value.lval = php3_list_insert(sybase_ptr,php3_sybase_module.le_plink);
		return_value->type = IS_LONG;
	} else { /* non persistent */
		list_entry *index_ptr,new_index_ptr;
		
		/* first we check the hash for the hashed_details key.  if it exists,
		 * it should point us to the right offset where the actual sybase link sits.
		 * if it doesn't, open a new sybase link, add it to the resource list,
		 * and add a pointer to it with hashed_details as the key.
		 */
		if (hash_find(list,hashed_details,hashed_details_length+1,(void **) &index_ptr)==SUCCESS) {
			int type,link;
			void *ptr;

			if (index_ptr->type != le_index_ptr) {
				RETURN_FALSE;
			}
			link = (int) index_ptr->ptr;
			ptr = php3_list_find(link,&type);   /* check if the link is still there */
			if (ptr && (type==php3_sybase_module.le_link || type==php3_sybase_module.le_plink)) {
				return_value->value.lval = php3_sybase_module.default_link = link;
				return_value->type = IS_LONG;
				efree(hashed_details);
				return;
			} else {
				hash_del(list,hashed_details,hashed_details_length+1);
			}
		}
		if (php3_sybase_module.max_links!=-1 && php3_sybase_module.num_links>=php3_sybase_module.max_links) {
			php3_error(E_WARNING,"Sybase:  Too many open links (%d)",php3_sybase_module.num_links);
			efree(hashed_details);
			RETURN_FALSE;
		}
		
		if ((sybase.link=dbopen(sybase.login,host))==NULL) {
			/*php3_error(E_WARNING,"Sybase:  Unable to connect to server:  %s",sybase_error(sybase));*/
			efree(hashed_details);
			RETURN_FALSE;
		}

		if (dbsetopt(sybase.link,DBBUFFER,"2",-1)==FAIL) {
			efree(hashed_details);
			dbloginfree(sybase.login);
			dbclose(sybase.link);
			RETURN_FALSE;
		}
		
		/* add it to the list */
		sybase_ptr = (sybase_link *) emalloc(sizeof(sybase_link));
		memcpy(sybase_ptr,&sybase,sizeof(sybase_link));
		return_value->value.lval = php3_list_insert(sybase_ptr,php3_sybase_module.le_link);
		return_value->type = IS_LONG;
		
		/* add it to the hash */
		new_index_ptr.ptr = (void *) return_value->value.lval;
		new_index_ptr.type = le_index_ptr;
		if (hash_update(list,hashed_details,hashed_details_length+1,(void *) &new_index_ptr, sizeof(list_entry),NULL)==FAILURE) {
			efree(hashed_details);
			RETURN_FALSE;
		}
		php3_sybase_module.num_links++;
	}
	efree(hashed_details);
	php3_sybase_module.default_link=return_value->value.lval;
}


static int php3_sybase_get_default_link(INTERNAL_FUNCTION_PARAMETERS)
{
	if (php3_sybase_module.default_link==-1) { /* no link opened yet, implicitly open one */
		HashTable dummy;

		hash_init(&dummy,0,NULL,NULL,0);
		php3_sybase_do_connect(&dummy,return_value,list,plist,0);
		hash_destroy(&dummy);
	}
	return php3_sybase_module.default_link;
}


void php3_sybase_connect(INTERNAL_FUNCTION_PARAMETERS)
{
	php3_sybase_do_connect(INTERNAL_FUNCTION_PARAM_PASSTHRU,0);
}

void php3_sybase_pconnect(INTERNAL_FUNCTION_PARAMETERS)
{
	php3_sybase_do_connect(INTERNAL_FUNCTION_PARAM_PASSTHRU,1);
}


void php3_sybase_close(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *sybase_link_index;
	int id,type;
	
	switch (ARG_COUNT(ht)) {
		case 0:
			id = php3_sybase_module.default_link;
			break;
		case 1:
			if (getParameters(ht, 1, &sybase_link_index)==FAILURE) {
				RETURN_FALSE;
			}
			convert_to_long(sybase_link_index);
			id = sybase_link_index->value.lval;
			break;
		default:
			WRONG_PARAM_COUNT;
			break;
	}
	
	php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_link && type!=php3_sybase_module.le_plink) {
		php3_error(E_WARNING,"%d is not a Sybase link index",id);
		RETURN_FALSE;
	}
	
	php3_list_delete(id);
	RETURN_TRUE;
}
	

void php3_sybase_select_db(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *db,*sybase_link_index;
	int id,type;
	sybase_link  *sybase_ptr;
	
	switch(ARG_COUNT(ht)) {
		case 1:
			if (getParameters(ht, 1, &db)==FAILURE) {
				RETURN_FALSE;
			}
			id = php3_sybase_get_default_link(INTERNAL_FUNCTION_PARAM_PASSTHRU);
			break;
		case 2:
			if (getParameters(ht, 2, &db, &sybase_link_index)==FAILURE) {
				RETURN_FALSE;
			}
			convert_to_long(sybase_link_index);
			id = sybase_link_index->value.lval;
			break;
		default:
			WRONG_PARAM_COUNT;
			break;
	}
	
	CHECK_LINK(id);
	
	sybase_ptr = (sybase_link *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_link && type!=php3_sybase_module.le_plink) {
		php3_error(E_WARNING,"%d is not a Sybase link index",id);
		RETURN_FALSE;
	}
	
	convert_to_string(db);
	
	if (dbuse(sybase_ptr->link,db->value.strval)==FAIL) {
		/*php3_error(E_WARNING,"Sybase:  Unable to select database:  %s",sybase_error(sybase));*/
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}


static void php3_sybase_get_column_content(sybase_link *sybase_ptr,int offset,YYSTYPE *result)
{
	if (dbdatlen(sybase_ptr->link,offset) == 0) {
		var_reset(result);
		return;
	}

	switch (coltype(offset))
	{
		case SYBINT2:
		case SYBINT4: {	
			result->value.lval = (long) anyintcol(offset);
			result->type = IS_LONG;
			break;
		}
		case SYBCHAR:
		case SYBTEXT: {
			int length;
			char *data = charcol(offset);

			length=dbdatlen(sybase_ptr->link,offset);
			while (length>0 && charcol(offset)[length-1] == ' ') { /* nuke trailing whitespace */
				length--;
			}
			result->value.strval = estrndup(data,length);
			result->strlen = length;
			result->type = IS_STRING;
			break;
		}
		/*case SYBFLT8:*/
		case SYBREAL: {
			result->value.dval = (double) floatcol(offset);
			result->type = IS_DOUBLE;
			break;
		}
		default: {
			if (dbwillconvert(coltype(offset),SYBCHAR)) {
				char buf[1024];
				
				dbconvert(NULL,coltype(offset),dbdata(sybase_ptr->link,offset), dbdatlen(sybase_ptr->link,offset),SYBCHAR,buf,-1);
				result->strlen = strlen(buf);
				result->value.strval = estrndup(buf,result->strlen);
				result->type = IS_STRING;
			} else {
				php3_error(E_WARNING,"Sybase:  column %d has unknown data type (%d)", offset, coltype(offset));
				var_reset(result);
			}
		}
	}
}


void php3_sybase_query(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *query,*sybase_link_index;
	int id,type,retvalue;
	sybase_link *sybase_ptr;
	sybase_result *result;
	int num_fields;
	int blocks_initialized=1;
	int i,j;
	
	switch(ARG_COUNT(ht)) {
		case 1:
			if (getParameters(ht, 1, &query)==FAILURE) {
				RETURN_FALSE;
			}
			id = php3_sybase_module.default_link;
			break;
		case 2:
			if (getParameters(ht, 2, &query, &sybase_link_index)==FAILURE) {
				RETURN_FALSE;
			}
			convert_to_long(sybase_link_index);
			id = sybase_link_index->value.lval;
			break;
		default:
			WRONG_PARAM_COUNT;
			break;
	}
	
	sybase_ptr = (sybase_link *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_link && type!=php3_sybase_module.le_plink) {
		php3_error(E_WARNING,"%d is not a Sybase link index",id);
		RETURN_FALSE;
	}
	
	convert_to_string(query);
	if (dbcmd(sybase_ptr->link,query->value.strval)==FAIL) {
		/*php3_error(E_WARNING,"Sybase:  Unable to set query");*/
		RETURN_FALSE;
	}
	if (dbsqlexec(sybase_ptr->link)==FAIL || dbresults(sybase_ptr->link)==FAIL) {
		/*php3_error(E_WARNING,"Sybase:  Query failed");*/
		RETURN_FALSE;
	}
	
	/* The following is more or less the equivalent of mysql_store_result().
	 * fetch all rows from the server into the row buffer, thus:
	 * 1)  Being able to fire up another query without explicitly reading all rows
	 * 2)  Having numrows accessible
	 */
	
	retvalue=dbnextrow(sybase_ptr->link);
	
	if (retvalue==FAIL) {
		RETURN_FALSE;
	}

	num_fields = dbnumcols(sybase_ptr->link);
	if (num_fields<=0) {
		RETURN_TRUE;
	}
	
	result = (sybase_result *) emalloc(sizeof(sybase_result));
	result->data = (YYSTYPE **) emalloc(sizeof(YYSTYPE *)*SYBASE_ROWS_BLOCK);
	result->sybase_ptr = sybase_ptr;
	result->cur_field=result->cur_row=result->num_rows=0;
	result->num_fields = num_fields;

	i=0;
	while (retvalue!=FAIL && retvalue!=NO_MORE_ROWS) {
		result->num_rows++;
		if (result->num_rows > blocks_initialized*SYBASE_ROWS_BLOCK) {
			result->data = (YYSTYPE **) erealloc(result->data,sizeof(YYSTYPE *)*SYBASE_ROWS_BLOCK*(++blocks_initialized));
		}
		result->data[i] = (YYSTYPE *) emalloc(sizeof(YYSTYPE)*num_fields);
		for (j=1; j<=num_fields; j++) {
			php3_sybase_get_column_content(sybase_ptr, j, &result->data[i][j-1]);
		}
		retvalue=dbnextrow(sybase_ptr->link);
		dbclrbuf(sybase_ptr->link,DBLASTROW(sybase_ptr->link)-1); 
		i++;
	}
	result->num_rows = DBCOUNT(sybase_ptr->link);
	
	result->fields = (sybase_field *) emalloc(sizeof(sybase_field)*num_fields);
	j=0;
	for (i=0; i<num_fields; i++) {
		char *fname = dbcolname(sybase_ptr->link,i+1);
		char computed_buf[16];
		
		if (*fname) {
			result->fields[i].name = estrdup(fname);
		} else {
			if (j>0) {
				snprintf(computed_buf,16,"computed%d",j);
			} else {
				strcpy(computed_buf,"computed");
			}
			result->fields[i].name = estrdup(computed_buf);
			j++;
		}
		result->fields[i].max_length = dbcollen(sybase_ptr->link,i+1);
		result->fields[i].column_source = estrdup(dbcolsource(sybase_ptr->link,i+1));
		if (!result->fields[i].column_source) {
			result->fields[i].column_source = empty_string;
		}
	}
	return_value->value.lval = php3_list_insert(result,php3_sybase_module.le_result);
	return_value->type = IS_LONG;
}

                        
void php3_sybase_free_result(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *sybase_result_index;
	sybase_result *result;
	int type;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &sybase_result_index)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(sybase_result_index);
	result = (sybase_result *) php3_list_find(sybase_result_index->value.lval,&type);
	
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",sybase_result_index->value.lval);
		RETURN_FALSE;
	}
	php3_list_delete(sybase_result_index->value.lval);
}


void php3_sybase_get_last_message(INTERNAL_FUNCTION_PARAMETERS)
{
	RETURN_STRING(php3_sybase_module.server_message);
}


void php3_sybase_num_rows(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *result_index;
	int type,id;
	sybase_result *result;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &result_index)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(result_index);
	id = result_index->value.lval;
	
	result = (sybase_result *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",id);
		RETURN_FALSE;
	}	
	
	return_value->value.lval = result->num_rows;
	return_value->type = IS_LONG;
}


void php3_sybase_num_fields(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *result_index;
	int type,id;
	sybase_result *result;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &result_index)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(result_index);
	id = result_index->value.lval;
	
	result = (sybase_result *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",id);
		RETURN_FALSE;
	}	
	
	return_value->value.lval = result->num_fields;
	return_value->type = IS_LONG;
}


void php3_sybase_fetch_row(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *sybase_result_index;
	int type,i,id;
	sybase_result *result;
	YYSTYPE field_content;

	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &sybase_result_index)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(sybase_result_index);
	id = sybase_result_index->value.lval;
	
	result = (sybase_result *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",id);
		RETURN_FALSE;
	}
	
	if (result->cur_row >= result->num_rows) {
		RETURN_FALSE;
	}
	
	array_init(return_value);
	for (i=0; i<result->num_fields; i++) {
		field_content = result->data[result->cur_row][i];
		yystype_copy_constructor(&field_content);
		hash_index_update(return_value->value.ht, i, (void *) &field_content, sizeof(YYSTYPE),NULL);
	}
	result->cur_row++;
}


static void php3_sybase_fetch_hash(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *sybase_result_index;
	sybase_result *result;
	int type;
	int i;
	YYSTYPE *yystype_ptr,tmp;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &sybase_result_index)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(sybase_result_index);
	result = (sybase_result *) php3_list_find(sybase_result_index->value.lval,&type);
	
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",sybase_result_index->value.lval);
		RETURN_FALSE;
	}
	
	if (result->cur_row >= result->num_rows) {
		RETURN_FALSE;
	}
	
	if (array_init(return_value)==FAILURE) {
		RETURN_FALSE;
	}
	
	for (i=0; i<result->num_fields; i++) {
		tmp = result->data[result->cur_row][i];
		yystype_copy_constructor(&tmp);
		if (php3_ini.magic_quotes_runtime && tmp.type == IS_STRING) {
			tmp.value.strval = _php3_addslashes(tmp.value.strval,1);
		}
		hash_index_update(return_value->value.ht, i, (void *) &tmp, sizeof(YYSTYPE), (void **) &yystype_ptr);
		hash_pointer_update(return_value->value.ht, result->fields[i].name, strlen(result->fields[i].name)+1, yystype_ptr);
	}
	result->cur_row++;
}


void php3_sybase_fetch_object(INTERNAL_FUNCTION_PARAMETERS)
{
	php3_sybase_fetch_hash(INTERNAL_FUNCTION_PARAM_PASSTHRU);
	if (return_value->type==IS_ARRAY) {
		return_value->type=IS_OBJECT;
	}
}


void php3_sybase_fetch_array(INTERNAL_FUNCTION_PARAMETERS)
{
	php3_sybase_fetch_hash(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

void php3_sybase_data_seek(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *sybase_result_index,*offset;
	int type,id;
	sybase_result *result;

	if (ARG_COUNT(ht)!=2 || getParameters(ht, 2, &sybase_result_index, &offset)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(sybase_result_index);
	id = sybase_result_index->value.lval;
	
	result = (sybase_result *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",id);
		RETURN_FALSE;
	}

	convert_to_long(offset);
	if (offset->value.lval<0 || offset->value.lval>=result->num_rows) {
		php3_error(E_WARNING,"Sybase:  Bad row offset");
		RETURN_FALSE;
	}
	
	result->cur_row = offset->value.lval;
	RETURN_TRUE;
}


void php3_sybase_fetch_field(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *sybase_result_index,*offset;
	int type,id,field_offset;
	sybase_result *result;

	switch (ARG_COUNT(ht)) {
		case 1:
			if (getParameters(ht, 1, &sybase_result_index)==FAILURE) {
				RETURN_FALSE;
			}
			field_offset=-1;
			break;
		case 2:
			if (getParameters(ht, 2, &sybase_result_index, &offset)==FAILURE) {
				RETURN_FALSE;
			}
			convert_to_long(offset);
			field_offset = offset->value.lval;
			break;
		default:
			WRONG_PARAM_COUNT;
			break;
	}
	
	convert_to_long(sybase_result_index);
	id = sybase_result_index->value.lval;
	
	result = (sybase_result *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",id);
		RETURN_FALSE;
	}
	
	if (field_offset==-1) {
		field_offset = result->cur_field;
		result->cur_field++;
	}
	
	if (field_offset<0 || field_offset >= result->num_fields) {
		if (ARG_COUNT(ht)==2) { /* field specified explicitly */
			php3_error(E_WARNING,"Sybase:  Bad column offset");
		}
		RETURN_FALSE;
	}

	if (object_init(return_value)==FAILURE) {
		RETURN_FALSE;
	}
	add_property_string(return_value, "name",result->fields[field_offset].name, 1);
	add_property_long(return_value, "max_length",result->fields[field_offset].max_length);
	add_property_string(return_value, "column_source",result->fields[field_offset].column_source, 1);
}

void php3_sybase_field_seek(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *sybase_result_index,*offset;
	int type,id,field_offset;
	sybase_result *result;

	if (ARG_COUNT(ht)!=2 || getParameters(ht, 2, &sybase_result_index, &offset)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(sybase_result_index);
	id = sybase_result_index->value.lval;
	
	result = (sybase_result *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",id);
		RETURN_FALSE;
	}
	
	convert_to_long(offset);
	field_offset = offset->value.lval;
	
	if (field_offset<0 || field_offset >= result->num_fields) {
		php3_error(E_WARNING,"Sybase:  Bad column offset");
		RETURN_FALSE;
	}

	result->cur_field = field_offset;
	RETURN_TRUE;
}


void php3_sybase_result(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *row, *field, *sybase_result_index;
	int id,type,field_offset=0;
	sybase_result *result;
	

	if (ARG_COUNT(ht)!=3 || getParameters(ht, 3, &sybase_result_index, &row, &field)==FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_long(sybase_result_index);
	id = sybase_result_index->value.lval;
	
	result = (sybase_result *) php3_list_find(id,&type);
	if (type!=php3_sybase_module.le_result) {
		php3_error(E_WARNING,"%d is not a Sybase result index",id);
		RETURN_FALSE;
	}
	
	convert_to_long(row);
	if (row->value.lval<0 || row->value.lval>=result->num_rows) {
		php3_error(E_WARNING,"Sybase:  Bad row offset (%d)",row->value.lval);
		RETURN_FALSE;
	}

	switch(field->type) {
		case IS_STRING: {
			int i;

			for (i=0; i<result->num_fields; i++) {
				if (!strcasecmp(result->fields[i].name,field->value.strval)) {
					field_offset = i;
					break;
				}
			}
			if (i>=result->num_fields) { /* no match found */
				php3_error(E_WARNING,"Sybase:  %s field not found in result",field->value.strval);
				RETURN_FALSE;
			}
			break;
		}
		default:
			convert_to_long(field);
			field_offset = field->value.lval;
			if (field_offset<0 || field_offset>=result->num_fields) {
				php3_error(E_WARNING,"Sybase:  Bad column offset specified");
				RETURN_FALSE;
			}
			break;
	}

	*return_value = result->data[row->value.lval][field_offset];
	yystype_copy_constructor(return_value);
}


void php3_info_sybase(void)
{
	char maxp[16],maxl[16];
	
	if (php3_sybase_module.max_persistent==-1) {
		strcpy(maxp,"Unlimited");
	} else {
		snprintf(maxp,15,"%ld",php3_sybase_module.max_persistent);
		maxp[15]=0;
	}
	if (php3_sybase_module.max_links==-1) {
		strcpy(maxl,"Unlimited");
	} else {
		snprintf(maxl,15,"%ld",php3_sybase_module.max_links);
		maxl[15]=0;
	}
	php3_printf("<table cellpadding=5>"
				"<tr><td>Allow persistent links:</td><td>%s</td></tr>\n"
				"<tr><td>Persistent links:</td><td>%d/%s</td></tr>\n"
				"<tr><td>Total links:</td><td>%d/%s</td></tr>\n"
				"<tr><td>Application name:</td><td>%s</td></tr>\n"
				"<tr><td valign=\"top\" width=\"20%%\">Client API information:</td><td><pre>%s</pre></td></tr>\n"
				"</table>\n",
				(php3_sybase_module.allow_persistent?"Yes":"No"),
				php3_sybase_module.num_persistent,maxp,
				php3_sybase_module.num_links,maxl,
				php3_sybase_module.appname,
				dbversion());
}


void php3_sybase_min_error_severity(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *severity;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &severity)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(severity);
	php3_sybase_module.min_error_severity = severity->value.lval;
}


void php3_sybase_min_message_severity(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *severity;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &severity)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(severity);
	php3_sybase_module.min_message_severity = severity->value.lval;
}

#endif
