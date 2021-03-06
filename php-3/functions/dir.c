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
   +----------------------------------------------------------------------+
 */

/* $Id: dir.c,v 1.40 1998/01/23 01:29:43 zeev Exp $ */

#ifdef THREAD_SAFE
#include "tls.h"
#endif
#include "parser.h"
#include "internal_functions.h"
#include "list.h"

#include "php3_dir.h"

#if HAVE_DIRENT_H
#include <dirent.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <errno.h>

#if MSVC5
#if !(APACHE)
#define NEEDRDH 1
#endif
#include "win32/readdir.h"
#endif

#ifndef THREAD_SAFE
static int dirp_id = 0;
static int le_dirp;
#endif

function_entry php3_dir_functions[] = {
	{"opendir",		php3_opendir,	NULL},
	{"closedir",	php3_closedir,	NULL},
	{"chdir",		php3_chdir,		NULL},
	{"rewinddir",	php3_rewinddir,	NULL},
	{"readdir",		php3_readdir,	NULL},
	{"dir",			php3_getdir,	NULL},
	{NULL, NULL, NULL}
};

php3_module_entry php3_dir_module_entry = {
	"PHP_dir", php3_dir_functions, php3_minit_dir, NULL, NULL, NULL, NULL, 0, 0, 0, NULL
};


int php3_minit_dir(INITFUNCARG)
{
	TLS_VARS;
	
	GLOBAL(le_dirp) = register_list_destructors(closedir,NULL);
	return SUCCESS;
}

void php3_opendir(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *arg;
	DIR *dirp;
	int ret;
	TLS_VARS;
	
	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &arg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(arg);

	dirp = opendir(arg->value.strval);
	if (!dirp) {
		php3_error(E_WARNING, "OpenDir: %s (errno %d)", strerror(errno),errno);
		RETURN_FALSE;
	}
	ret = php3_list_insert(dirp, GLOBAL(le_dirp));
	GLOBAL(dirp_id) = ret;
	RETURN_LONG(ret);
}

void php3_closedir(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *id, *tmp;
	int id_to_find;
	DIR *dirp;
	int dirp_type;
	TLS_VARS;
	
	if (ARG_COUNT(ht) == 0) {
		if (getThis(&id) == SUCCESS) {
			if (hash_find(id->value.ht, "handle", sizeof("handle"), (void **)&tmp) == FAILURE) {
				php3_error(E_WARNING, "unable to find my handle property");
				RETURN_FALSE;
			}
			id_to_find = tmp->value.lval;
		} else {
			id_to_find = GLOBAL(dirp_id);
		}
	} else if ((ARG_COUNT(ht) != 1) || getParameters(ht, 1, &id) == FAILURE) {
		WRONG_PARAM_COUNT;
	} else {
		convert_to_long(id);
		id_to_find = id->value.lval;
	}
		
	dirp = (DIR *)php3_list_find(id_to_find, &dirp_type);
	if (!dirp || dirp_type != GLOBAL(le_dirp)) {
		php3_error(E_WARNING, "unable to find identifier (%d)", id_to_find);
		RETURN_FALSE;
	}
	php3_list_delete(id_to_find);
}

void php3_chdir(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *arg;
	int ret;
	TLS_VARS;
	
	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &arg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(arg);
	ret = chdir(arg->value.strval);

	if (ret < 0) {
		php3_error(E_WARNING, "ChDir: %s (errno %d)", strerror(errno), errno);
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

void php3_rewinddir(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *id, *tmp;
	int id_to_find;
	DIR *dirp;
	int dirp_type;
	TLS_VARS;
	
	if (ARG_COUNT(ht) == 0) {
		if (getThis(&id) == SUCCESS) {
			if (hash_find(id->value.ht, "handle", sizeof("handle"), (void **)&tmp) == FAILURE) {
				php3_error(E_WARNING, "unable to find my handle property");
				RETURN_FALSE;
			}
			id_to_find = tmp->value.lval;
		} else {
			id_to_find = GLOBAL(dirp_id);
		}
	} else if ((ARG_COUNT(ht) != 1) || getParameters(ht, 1, &id) == FAILURE) {
		WRONG_PARAM_COUNT;
	} else {
		convert_to_long(id);
		id_to_find = id->value.lval;
	}
		
	dirp = (DIR *)php3_list_find(id_to_find, &dirp_type);
	if (!dirp || dirp_type != GLOBAL(le_dirp)) {
		php3_error(E_WARNING, "unable to find identifier (%d)", id_to_find);
		RETURN_FALSE;
	}
	rewinddir(dirp);
}

void php3_readdir(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *id, *tmp;
	int id_to_find;
	DIR *dirp;
	int dirp_type;
	struct dirent *direntp;
	TLS_VARS;
	
	if (ARG_COUNT(ht) == 0) {
		if (getThis(&id) == SUCCESS) {
			if (hash_find(id->value.ht, "handle", sizeof("handle"), (void **)&tmp) == FAILURE) {
				php3_error(E_WARNING, "unable to find my handle property");
				RETURN_FALSE;
			}
			id_to_find = tmp->value.lval;
		} else {
			id_to_find = GLOBAL(dirp_id);
		}
	} else if ((ARG_COUNT(ht) != 1) || getParameters(ht, 1, &id) == FAILURE) {
		WRONG_PARAM_COUNT;
	} else {
		convert_to_long(id);
		id_to_find = id->value.lval;
	}
		
	dirp = (DIR *)php3_list_find(id_to_find, &dirp_type);
	if (!dirp || dirp_type != GLOBAL(le_dirp)) {
		php3_error(E_WARNING, "unable to find identifier (%d)", id_to_find);
		RETURN_FALSE;
	}
	direntp = readdir(dirp);
	if (direntp) {
		RETURN_STRINGL(direntp->d_name, strlen(direntp->d_name));
	}
	RETURN_FALSE;
}

void php3_getdir(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *arg;
	DIR *dirp;
	int ret;
	TLS_VARS;
	
	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &arg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(arg);

	dirp = opendir(arg->value.strval);
	if (!dirp) {
		php3_error(E_WARNING, "OpenDir: %s (errno %d)", strerror(errno), errno);
		RETURN_FALSE;
	}
	ret = php3_list_insert(dirp, GLOBAL(le_dirp));
	GLOBAL(dirp_id) = ret;

	/* construct an object with some methods */
	object_init(return_value);
	add_property_long(return_value, "handle", ret);
	add_property_stringl(return_value, "path", arg->value.strval, arg->strlen, 1);
	add_method(return_value, "read", php3_readdir);
	add_method(return_value, "rewind", php3_rewinddir);
	add_method(return_value, "close", php3_closedir);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
