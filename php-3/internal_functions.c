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


/* $Id: internal_functions.c,v 1.303 1998/02/18 21:15:00 andi Exp $ */

#ifdef THREAD_SAFE
#include "tls.h"
#endif

#include "parser.h"
#ifndef MSVC5
#endif
#include "internal_functions.h"
#include "internal_functions_registry.h"
#include "list.h"
#include "modules.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#if HAVE_LIBDL
# if MSVC5
#  include <windows.h>
#  define dlclose FreeLibrary
#  define dlopen(a,b) LoadLibrary(a)
#  define dlsym GetProcAddress
# else
#  include <dlfcn.h>
# endif
#endif

#include "functions/php3_ldap.h"
#include "functions/php3_mysql.h"
#include "functions/php3_bcmath.h"
#include "functions/php3_msql.h"
#include "functions/basic_functions.h"
#include "functions/phpmath.h"
#include "functions/php3_string.h"
#include "functions/oracle.h"
#include "functions/base64.h"
#include "functions/php3_dir.h"
#include "functions/dns.h"
#include "functions/php3_pgsql.h"
#include "functions/php3_sybase.h"
#include "functions/php3_sybase-ct.h"
#include "functions/reg.h"
#include "functions/mail.h"
#include "functions/md5.h"
#include "functions/php3_gd.h"
#include "functions/html.h"
#include "functions/dl.h"
#include "functions/head.h"
#include "functions/post.h"
#include "functions/exec.h"
#include "functions/php3_solid.h"
#include "functions/sybsql.h"
#include "functions/adabasd.h"
#include "functions/file.h"
#include "functions/dbase.h"
#include "functions/filepro.h"
#include "functions/db.h"
#include "functions/php3_syslog.h"
#include "functions/php3_filestat.h"
#include "functions/php3_browscap.h"
#include "functions/pack.h"
#include "php3_debugger.h"
#include "functions/php3_unified_odbc.h"
#include "dl/snmp/php3_snmp.h"

extern php3_ini_structure php3_ini;
extern php3_ini_structure php3_ini_master;

#ifndef THREAD_SAFE
HashTable list_destructors,module_registry;
#endif

unsigned char first_arg_force_ref[] = { 1, BYREF_FORCE };
unsigned char first_arg_allow_ref[] = { 1, BYREF_ALLOW };
unsigned char second_arg_force_ref[] = { 2, BYREF_NONE, BYREF_FORCE };
unsigned char second_arg_allow_ref[] = { 2, BYREF_NONE, BYREF_ALLOW };

php3_builtin_module php3_builtin_modules[] = 
{
	{"Basic functions",				basic_functions_module_ptr},
	{"Dynamic extension loading",	dl_module_ptr},
	{"Directory",					php3_dir_module_ptr},
	{"File statting",				php3_filestat_module_ptr},
	{"File handling",				php3_file_module_ptr},
	{"HTTP Header",					php3_header_module_ptr},
	{"Sendmail",					mail_module_ptr},
	{"Debugger",					debugger_module_ptr},
	{"Syslog",						syslog_module_ptr},
	{"MySQL",						mysql_module_ptr},
	{"mSQL",						msql_module_ptr},
	{"PostgresSQL",					pgsql_module_ptr},
	{"LDAP",						ldap_module_ptr},
	{"FilePro",						filepro_module_ptr},
	{"Sybase SQL",					sybase_module_ptr},
	{"Sybase SQL - CT",				sybct_module_ptr},
	{"Sybase SQL - old",			sybase_old_module_ptr},
	{"Unified ODBC",				uodbc_module_ptr},
	{"DBase",						dbase_module_ptr},
	{"Regular Expressions",			regexp_module_ptr},
	{"Solid",						solid_module_ptr},
	{"Adabas",						adabas_module_ptr},
	{"Image",						gd_module_ptr},
	{"Oracle",						oracle_module_ptr},
	{"Apache",						apache_module_ptr},
	{"Crypt",						crypt_module_ptr},
	{"DBM",							dbm_module_ptr},
	{"bcmath",						bcmath_module_ptr},
	{"browscap",					browscap_module_ptr},
	{"SNMP",						snmp_module_ptr},
	{"Pack/Unpack",					pack_module_ptr},
	{NULL,							NULL}
};

	
/* this function doesn't check for too many parameters */
PHPAPI int getParameters(HashTable *ht, int param_count,...)
{
	va_list ptr;
	YYSTYPE **param, *tmp = NULL;
	int i;

	va_start(ptr, param_count);

	for (i = 0; i < param_count; i++) {
		param = va_arg(ptr, YYSTYPE **);
		if (hash_index_find(ht, i, (void **) &tmp) == FAILURE) {
			va_end(ptr);
			return FAILURE;
		}
		*param = tmp;
	}
	va_end(ptr);
	return SUCCESS;
}


PHPAPI int getParametersArray(HashTable *ht, int param_count, YYSTYPE **argument_array)
{
	int i;
	YYSTYPE *data;

	for (i = 0; i < param_count; i++) {
		if (hash_index_find(ht, i, (void **) &data) == FAILURE) {
			return FAILURE;
		}
		argument_array[i] = data;
	}
	return SUCCESS;
}

PHPAPI int getThis(YYSTYPE **this) {
	YYSTYPE *data;
	TLS_VARS;

	if (hash_find(GLOBAL(active_symbol_table), "this", sizeof("this"), (void **)&data) == FAILURE) {
		return FAILURE;
	}
	*this = data;
	return SUCCESS;
}

PHPAPI int ParameterPassedByReference(HashTable *ht, uint n)
{
	return hash_index_is_pointer(ht, n-1);
}


PHPAPI void wrong_param_count()
{
	TLS_VARS;

	php3_error(E_WARNING,"Wrong parameter count for %s()",GLOBAL(function_state.function_name));
}

	
inline PHPAPI int array_init(YYSTYPE *arg)
{
	arg->value.ht = (HashTable *) emalloc(sizeof(HashTable));
	if (!arg->value.ht || hash_init(arg->value.ht, 0, NULL, YYSTYPE_DESTRUCTOR, 0)) {
		php3_error(E_CORE_ERROR, "Cannot allocate memory for array");
		return FAILURE;
	}
	arg->type = IS_ARRAY;
	return SUCCESS;
}

inline PHPAPI int object_init(YYSTYPE *arg)
{
	arg->value.ht = (HashTable *) emalloc(sizeof(HashTable));
	if (!arg->value.ht || hash_init(arg->value.ht, 0, NULL, YYSTYPE_DESTRUCTOR, 0)) {
		php3_error(E_CORE_ERROR, "Cannot allocate memory for array");
		return FAILURE;
	}
	arg->type = IS_OBJECT;
	return SUCCESS;
}


inline PHPAPI int add_assoc_long(YYSTYPE *arg, char *key, long n)
{
	YYSTYPE tmp;

	tmp.type = IS_LONG;
	tmp.value.lval = n;
	return hash_update(arg->value.ht, key, strlen(key)+1, (void *) &tmp, sizeof(YYSTYPE), NULL);
}


inline PHPAPI int add_assoc_double(YYSTYPE *arg, char *key, double d)
{
	YYSTYPE tmp;

	tmp.type = IS_DOUBLE;
	tmp.value.dval = d;
	return hash_update(arg->value.ht, key, strlen(key)+1, (void *) &tmp, sizeof(YYSTYPE), NULL);
}


inline PHPAPI int add_assoc_string(YYSTYPE *arg, char *key, char *str, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = strlen(str);
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_update(arg->value.ht, key, strlen(key)+1, (void *) &tmp, sizeof(YYSTYPE), NULL);
}


inline PHPAPI int add_assoc_stringl(YYSTYPE *arg, char *key, char *str, uint length, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = length;
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_update(arg->value.ht, key, strlen(key)+1, (void *) &tmp, sizeof(YYSTYPE), NULL);
}


inline PHPAPI int add_assoc_function(YYSTYPE *arg, char *key,void (*function_ptr)(INTERNAL_FUNCTION_PARAMETERS))
{
	YYSTYPE tmp;
	
	tmp.type = IS_INTERNAL_FUNCTION;
	tmp.value.internal_function = function_ptr;
	return hash_update(arg->value.ht, key, strlen(key)+1, (void *) &tmp, sizeof(YYSTYPE), NULL);
}


inline PHPAPI int add_index_long(YYSTYPE *arg, uint index, long n)
{
	YYSTYPE tmp;

	tmp.type = IS_LONG;
	tmp.value.lval = n;
	return hash_index_update(arg->value.ht, index, (void *) &tmp, sizeof(YYSTYPE),NULL);
}


inline PHPAPI int add_index_double(YYSTYPE *arg, uint index, double d)
{
	YYSTYPE tmp;

	tmp.type = IS_DOUBLE;
	tmp.value.dval = d;
	return hash_index_update(arg->value.ht, index, (void *) &tmp, sizeof(YYSTYPE),NULL);
}


inline PHPAPI int add_index_string(YYSTYPE *arg, uint index, char *str, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = strlen(str);
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_index_update(arg->value.ht, index, (void *) &tmp, sizeof(YYSTYPE),NULL);
}


inline PHPAPI int add_index_stringl(YYSTYPE *arg, uint index, char *str, uint length, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = length;
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_index_update(arg->value.ht, index, (void *) &tmp, sizeof(YYSTYPE),NULL);
}

inline PHPAPI int add_next_index_long(YYSTYPE *arg, long n)
{
	YYSTYPE tmp;

	tmp.type = IS_LONG;
	tmp.value.lval = n;
	return hash_next_index_insert(arg->value.ht, &tmp, sizeof(YYSTYPE),NULL);
}


inline PHPAPI int add_next_index_double(YYSTYPE *arg, double d)
{
	YYSTYPE tmp;

	tmp.type = IS_DOUBLE;
	tmp.value.dval = d;
	return hash_next_index_insert(arg->value.ht, &tmp, sizeof(YYSTYPE),NULL);
}


inline PHPAPI int add_next_index_string(YYSTYPE *arg, char *str, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = strlen(str);
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_next_index_insert(arg->value.ht, &tmp, sizeof(YYSTYPE),NULL);
}


inline PHPAPI int add_next_index_stringl(YYSTYPE *arg, char *str, uint length, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = length;
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_next_index_insert(arg->value.ht, &tmp, sizeof(YYSTYPE),NULL);
}


inline PHPAPI int add_get_assoc_string(YYSTYPE *arg, char *key, char *str, void **dest, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = strlen(str);
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_update(arg->value.ht, key, strlen(key)+1, (void *) &tmp, sizeof(YYSTYPE), dest);
}


inline PHPAPI int add_get_assoc_stringl(YYSTYPE *arg, char *key, char *str, uint length, void **dest, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = length;
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_update(arg->value.ht, key, strlen(key)+1, (void *) &tmp, sizeof(YYSTYPE), dest);
}

inline PHPAPI int add_get_index_string(YYSTYPE *arg, uint index, char *str, void **dest, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = strlen(str);
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_index_update(arg->value.ht, index, (void *) &tmp, sizeof(YYSTYPE),dest);
}


inline PHPAPI int add_get_index_stringl(YYSTYPE *arg, uint index, char *str, uint length, void **dest, int duplicate)
{
	YYSTYPE tmp;

	tmp.type = IS_STRING;
	tmp.strlen = length;
	if (duplicate) {
		tmp.value.strval = estrndup(str,tmp.strlen);
	} else {
		tmp.value.strval = str;
	}
	return hash_index_update(arg->value.ht, index, (void *) &tmp, sizeof(YYSTYPE),dest);
}

int module_startup_modules(void)
{
	php3_builtin_module *ptr = php3_builtin_modules;

	while (ptr->name) {
		if (ptr->module) {
			if (ptr->module->module_startup_func) {
				if (ptr->module->module_startup_func(MODULE_PERSISTENT)==FAILURE) {
					php3_error(E_CORE_ERROR,"Unable to start %s module",ptr->name);
					return FAILURE;
				}
			}
			ptr->module->type = MODULE_PERSISTENT;
			register_module(ptr->module);
		}
		ptr++;
	}
	return SUCCESS;
}


int _register_list_destructors(void (*list_destructor)(void *), void (*plist_destructor)(void *))
{
	list_destructors_entry ld;
	TLS_VARS;
	
	ld.list_destructor=(void (*)(void *)) list_destructor;
	ld.plist_destructor=(void (*)(void *)) plist_destructor;
	
	if (hash_next_index_insert(&GLOBAL(list_destructors),(void *) &ld,sizeof(list_destructors_entry),NULL)==FAILURE) {
		return FAILURE;
	}
	return GLOBAL(list_destructors).nNextFreeElement-1;
}


/* registers all functions in *library_functions in the function hash */
PHPAPI int register_functions(function_entry *functions)
{
	function_entry *ptr = functions;
	YYSTYPE phps;
	int count=0,unload=0;
	TLS_VARS;

	while (ptr->fname) {
		phps.value.internal_function = ptr->handler;
		phps.type = IS_INTERNAL_FUNCTION;
		phps.func_arg_types = ptr->func_arg_types;
		if (!phps.value.internal_function) {
			php3_error(E_CORE_WARNING,"Null function defined as active function");
			unregister_functions(functions,count);
			return FAILURE;
		}
		if (hash_add(&GLOBAL(function_table), ptr->fname, strlen(ptr->fname)+1, &phps, sizeof(YYSTYPE), NULL) == FAILURE) {
			unload=1;
			break;
		}
		ptr++;
		count++;
	}
	if (unload) { /* before unloading, display all remaining bad function in the module */
		while (ptr->fname) {
			if (hash_exists(&GLOBAL(function_table), ptr->fname, strlen(ptr->fname)+1)) {
				php3_error(E_CORE_WARNING,"Module load failed - duplicate function name - %s",ptr->fname);
			}
			ptr++;
		}
		unregister_functions(functions,count);
		return FAILURE;
	}
	return SUCCESS;
}

/* count=-1 means erase all functions, otherwise, 
 * erase the first count functions
 */
PHPAPI void unregister_functions(function_entry *functions,int count)
{
	function_entry *ptr = functions;
	int i=0;
	TLS_VARS;

	while (ptr->fname) {
		if (count!=-1 && i>=count) {
			break;
		}
#if 0
		php3_printf("Unregistering %s()\n",ptr->fname);
#endif
		hash_del(&GLOBAL(function_table),ptr->fname,strlen(ptr->fname)+1);
		ptr++;
		i++;
	}
}


int register_module(php3_module_entry *module)
{
	TLS_VARS;

#if 0
	php3_printf("%s:  Registering module\n",module->name);
#endif
	if (register_functions(module->functions)==FAILURE) {
		php3_error(E_CORE_WARNING,"%s:  Unable to register functions, unable to load",module->name);
		return FAILURE;
	}
	module->module_started=1;
	return hash_add(&GLOBAL(module_registry),module->name,strlen(module->name)+1,(void *)module,sizeof(php3_module_entry),NULL);
}


void module_destructor(php3_module_entry *module)
{
	if (module->request_started && module->request_shutdown_func) {
#if 0
		php3_printf("%s:  Request shutdown\n",module->name);
#endif
		module->request_shutdown_func();
	}
	module->request_started=0;
	if (module->module_started && module->module_shutdown_func) {
#if 0
		php3_printf("%s:  Module shutdown\n",module->name);
#endif
		module->module_shutdown_func();
	}
	module->module_started=0;
	unregister_functions(module->functions,-1);
#if HAVE_LIBDL
	if (module->handle) {
		dlclose(module->handle);
	}
#endif
}


/* call request startup for all modules */
int module_registry_request_startup(php3_module_entry *module)
{
	if (!module->request_started && module->request_startup_func) {
#if 0
		php3_printf("%s:  Request startup\n",module->name);
#endif
		module->request_startup_func(module->type);
	}
	module->request_started=1;
	return 0;
}


/* for persistent modules - call request shutdown and flag NOT to erase
 * for temporary modules - do nothing, and flag to erase
 */
int module_registry_cleanup(php3_module_entry *module)
{
	switch(module->type) {
		case MODULE_PERSISTENT:
			if (module->request_started && module->request_shutdown_func) {
#if 0
				php3_printf("%s:  Request shutdown\n",module->name);
#endif
				module->request_shutdown_func();
			}
			module->request_started=0;
			return 0;
			break;
		case MODULE_TEMPORARY:
			return 1;
			break;
	}
	return 0;
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
