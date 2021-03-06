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


/* $Id: basic_functions.c,v 1.135 1998/02/28 13:33:28 zeev Exp $ */

#ifdef THREAD_SAFE
#include "tls.h"
#endif
#include "parser.h"
#include "modules.h"
#include "internal_functions.h"
#include "internal_functions_registry.h"
#include "list.h"
#include "basic_functions.h"
#include "operators.h"
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include "safe_mode.h"
#include "functions/basic_functions.h"
#include "functions/phpmath.h"
#include "functions/php3_string.h"
#include "functions/dns.h"
#include "functions/md5.h"
#include "functions/html.h"
#include "functions/post.h"
#include "functions/exec.h"
#include "functions/info.h"
#include "functions/url.h"
#include "functions/datetime.h"
#include "functions/fsock.h"
#include "functions/image.h"
#include "functions/php3_link.h"
#include "functions/php3_filestat.h"
#include "functions/microtime.h"
#include "functions/pageinfo.h"
#include "functions/uniqid.h"
#include "functions/base64.h"
#include "functions/mail.h"
#if WIN32|WINNT
#include "win32/unistd.h"
#endif

static unsigned char second_and_third_args_force_ref[] = { 3, BYREF_NONE, BYREF_FORCE, BYREF_FORCE };
#if PHP_DEBUGGER
extern int _php3_send_error(char *message, char *address);
#endif

function_entry basic_functions[] = {
	{"intval",		int_value,					NULL},
	{"doubleval",	double_value,				NULL},
	{"strval",		string_value,				NULL},
	{"short_tags",	php3_toggle_short_open_tag, NULL},
	{"sleep",		php3_sleep,					NULL},
	{"usleep",		php3_usleep,				NULL},
	{"ksort",		php3_key_sort,				first_arg_force_ref},
	{"asort",		php3_asort,					first_arg_force_ref},
	{"arsort",		php3_arsort,				first_arg_force_ref},
	{"sort",		php3_sort,					first_arg_force_ref},
	{"rsort",		php3_rsort,					first_arg_force_ref},
	{"sizeof",		php3_count,					first_arg_allow_ref},
	{"count",		php3_count,					first_arg_allow_ref},
	{"time",		php3_time,					NULL},
	{"mktime",		php3_mktime,				NULL},
	{"date",		php3_date,					NULL},
	{"gmdate",		php3_gmdate,				NULL},
	{"getdate",		php3_getdate,				NULL},
	{"checkdate",	php3_checkdate,				NULL},
	{"chr",			php3_chr,					NULL},
	{"ord",			php3_ord,					NULL},
	{"flush",		php3_flush,					NULL},
	{"end",			array_end,					first_arg_force_ref},
	{"prev",		array_prev,					first_arg_force_ref},
	{"next",		array_next,					first_arg_force_ref},
	{"reset",		array_reset,				first_arg_force_ref},
	{"current",		array_current,				first_arg_force_ref},
	{"key",			array_current_key,			first_arg_force_ref},
	{"gettype",		php3_gettype,				NULL},
	{"settype",		php3_settype,				first_arg_force_ref},
	{"min",			php3_min,					NULL},
	{"max",			php3_max,					NULL},

	{"addslashes",	php3_addslashes,			NULL},
	{"chop",		php3_chop,					NULL},
	{"pos",			array_current,				NULL},

	{"fsockopen",			php3_fsockopen,		NULL},
	{"getimagesize",		php3_getimagesize,	NULL},
	{"htmlspecialchars",	php3_htmlspecialchars,	NULL},
	{"htmlentities",		php3_htmlentities,	NULL},
	{"md5",					php3_md5,			NULL},

	{"parse_url",	php3_parse_url,				NULL},

	{"parse_str",	php3_parsestr,				NULL},
	{"phpinfo",		php3_info,					NULL},
	{"phpversion",	php3_version,				NULL},
	{"strlen",		php3_strlen,				NULL},
	{"strtok",		php3_strtok,				NULL},
	{"strtoupper",	php3_strtoupper,			NULL},
	{"strtolower",	php3_strtolower,			NULL},
	{"strchr",		php3_strstr,				NULL},
	{"strrev",		php3_strrev,				NULL},
	{"hebrev",		php3_hebrev,				NULL},
	{"hebrevc",		php3_hebrev_with_conversion,NULL},
	{"nl2br",		php3_newline_to_br,			NULL},
	{"basename",	php3_basename,				NULL},
	{"dirname", 	php3_dirname,				NULL},
	{"stripslashes",	php3_stripslashes,		NULL},
	{"strstr",		php3_strstr,				NULL},
	{"strrchr",		php3_strrchr,				NULL},
	{"substr",		php3_substr,				NULL},
	{"quotemeta",	php3_quotemeta,				NULL},
	{"urlencode",	php3_urlencode,				NULL},
	{"urldecode",	php3_urldecode,				NULL},
	{"rawurlencode",	php3_rawurlencode,		NULL},
	{"rawurldecode",	php3_rawurldecode,		NULL},
	{"ucfirst",		php3_ucfirst,				NULL},
	{"strtr",		php3_strtr,					NULL},
	{"sprintf",		php3_user_sprintf,			NULL},
	{"printf",		php3_user_printf,			NULL},
	{"setlocale",	php3_setlocale,				NULL},

	{"exec",			php3_exec,				second_and_third_args_force_ref},
	{"system",			php3_system,			second_arg_force_ref},
	{"escapeshellcmd",	php3_escapeshellcmd,	NULL},
	{"passthru",		php3_passthru,			second_arg_force_ref},

	{"soundex",		soundex,					NULL},

	{"rand",		php3_rand,					NULL},
	{"srand",		php3_srand,					NULL},
	{"getrandmax",	php3_getrandmax,			NULL},
	{"gethostbyaddr",	php3_gethostbyaddr,		NULL},
	{"gethostbyname",	php3_gethostbyname,		NULL},
	{"explode",		php3_explode,				NULL},
	{"implode",		php3_implode,				NULL},
	{"getenv",		php3_getenv,				NULL},
	{"error_reporting",	php3_error_reporting,	NULL},
	{"clearstatcache",	php3_clearstatcache,	NULL},

	{"unlink",		php3_unlink,				NULL},

	{"getmyuid",	php3_getmyuid,				NULL},
	{"getmypid",	php3_getmypid,				NULL},
	{"getmyinode",	php3_getmyinode,			NULL},
	{"getlastmod",	php3_getlastmod,			NULL},

	{"base64_decode",	php3_base64_decode,		NULL},
	{"base64_encode",	php3_base64_encode,		NULL},

	{"abs",			php3_abs,					NULL},
	{"ceil",		php3_ceil,					NULL},
	{"floor",		php3_floor,					NULL},
	{"round",		php3_round,					NULL},
	{"sin",			php3_sin,					NULL},
	{"cos",			php3_cos,					NULL},
	{"tan",			php3_tan,					NULL},
	{"asin",		php3_asin,					NULL},
	{"acos",		php3_acos,					NULL},
	{"atan",		php3_atan,					NULL},
	{"pi",			php3_pi,					NULL},
	{"pow",			php3_pow,					NULL},
	{"exp",			php3_exp,					NULL},
	{"log",			php3_log,					NULL},
	{"log10",		php3_log10,					NULL},
	{"sqrt",		php3_sqrt,					NULL},
	{"bindec",		php3_bindec,				NULL},
	{"hexdec",		php3_hexdec,				NULL},
	{"octdec",		php3_octdec,				NULL},
	{"decbin",		php3_decbin,				NULL},
	{"decoct",		php3_decoct,				NULL},
	{"dechex",		php3_dechex,				NULL},

	{"putenv",		php3_putenv,				NULL},
	{"microtime",	php3_microtime,				NULL},
	{"uniqid",		php3_uniqid,				NULL},
	{"linkinfo",	php3_linkinfo,				NULL},
	{"readlink",	php3_readlink,				NULL},
	{"symlink",		php3_symlink,				NULL},
	{"link",		php3_link,					NULL},
	{"get_current_user",	php3_get_current_user,	NULL},
	{"set_time_limit",	php3_set_time_limit,	NULL},
	
	{"get_cfg_var",	php3_get_cfg_var,			NULL},
	{"magic_quotes_runtime",	php3_set_magic_quotes_runtime,	NULL},
	
	{"is_long",		php3_is_long,				first_arg_force_ref},
	{"is_integer",	php3_is_long,				first_arg_force_ref},
	{"is_double",	php3_is_double,				first_arg_force_ref},
	{"is_real",		php3_is_double,				first_arg_force_ref},
	{"is_string",	php3_is_string,				first_arg_force_ref},
	{"is_array",	php3_is_array,				first_arg_force_ref},
	{"is_object",	php3_is_object,				first_arg_force_ref},

	{"leak",		php3_leak,					NULL},	
	{"error_log",		php3_error_log,					NULL},	
	{NULL, NULL, NULL}
};

php3_module_entry basic_functions_module = {
	"Basic Functions", basic_functions, NULL, NULL, php3_rinit_basic, php3_rshutdown_basic, NULL, 0, 0, 0, NULL
};

int php3_rinit_basic(INITFUNCARG)
{
	GLOBAL(strtok_string) = NULL;
	return SUCCESS;
}

int php3_rshutdown_basic(void)
{
	STR_FREE(GLOBAL(strtok_string));
	return SUCCESS;
}

/********************
 * System Functions *
 ********************/

void php3_getenv(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *str;
	char *ptr;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &str) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(str);
	if (str->type == IS_STRING &&
#if APACHE
		((ptr = table_get(GLOBAL(php3_rqst)->subprocess_env, str->value.strval)) || (ptr = getenv(str->value.strval)))
#endif
#if CGI_BINARY
		(ptr = getenv(str->value.strval))
#endif
#if USE_SAPI
		(ptr = GLOBAL(sapi_rqst)->getenv(GLOBAL(sapi_rqst)->scid,str->value.strval))
#endif
		) {
		RETURN_STRING(ptr);
	}
	RETURN_FALSE;
}


void php3_putenv(INTERNAL_FUNCTION_PARAMETERS)
{
#if HAVE_PUTENV
	YYSTYPE *str;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &str) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(str);

	if (str->value.strval && *(str->value.strval)) {
		int ret;
		/* Some versions of putenv() use the string verbatim,
		   so we can't just pass it in. But since we can never
		   safely clean this up, we don't want to use estrndup(). */
		ret = putenv(php3_strndup(str->value.strval,str->strlen));
		if (!ret) {
			RETURN_TRUE;
		}
		php3_error(E_WARNING, "putenv failed");
	}
	RETVAL_FALSE;
#endif
}


void php3_error_reporting(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *arg;
	int old_error_reporting;
	TLS_VARS;

	if (ARG_COUNT(ht)==0) {
		RETURN_LONG(GLOBAL(error_reporting));
	}
	
	if (ARG_COUNT(ht) != 1 || getParameters(ht,1,&arg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(arg);
	
	old_error_reporting = GLOBAL(error_reporting);

	switch(arg->value.lval) {
		case 0:
			GLOBAL(error_reporting)=GLOBAL(tmp_error_reporting)=0;
			break;
		case 1:
			GLOBAL(error_reporting)=GLOBAL(tmp_error_reporting)=E_ALL;
			break;
		case 2:
			GLOBAL(error_reporting)=GLOBAL(tmp_error_reporting)=E_ERROR | E_PARSE;
			break;
		case 3:
			GLOBAL(error_reporting)=GLOBAL(tmp_error_reporting)=E_ERROR;
			break;
		case 4:
			GLOBAL(error_reporting)=GLOBAL(tmp_error_reporting)=E_PARSE;
			break;
		default:
			GLOBAL(error_reporting)=GLOBAL(tmp_error_reporting)=E_ALL;
			break;
	}

	RETVAL_LONG(old_error_reporting);
}

void php3_toggle_short_open_tag(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *value;
	int ret;
	TLS_VARS;
	
	ret = php3_ini.short_open_tag;

	if (ARG_COUNT(ht)!=1 || getParameters(ht,1,&value) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(value);
	php3_ini.short_open_tag = value->value.lval;
	RETURN_LONG(ret);
}

/*******************
 * Basic Functions *
 *******************/

void int_value(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *num, *arg_base;
	int base;
	
	switch(ARG_COUNT(ht)) {
	case 1:
		if (getParameters(ht, 1, &num) == FAILURE) {
			WRONG_PARAM_COUNT;
		}
		base = 10;
		break;
	case 2:
		if (getParameters(ht, 2, &num, &arg_base) == FAILURE) {
			WRONG_PARAM_COUNT;
		}
		convert_to_long(arg_base);
		base = arg_base->value.lval;
		break;
	default:
		WRONG_PARAM_COUNT;
	}

	convert_to_long_base(num, base);
	*return_value = *num;
}


void double_value(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *num;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &num) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_double(num);
	*return_value = *num;
}


void string_value(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *num;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &num) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(num);
	*return_value = *num;
	yystype_copy_constructor(return_value);
}

static int array_key_compare(const void *a, const void *b)
{
	Bucket *first;
	Bucket *second;
	int min, r;

	first = *((Bucket **) a);
	second = *((Bucket **) b);

	if (first->arKey == NULL && second->arKey == NULL) {
		return (first->h - second->h);
	} else if (first->arKey == NULL) {
		return -1;
	} else if (second->arKey == NULL) {
		return 1;
	}
	min = MIN(first->nKeyLength, second->nKeyLength);
	if ((r = memcmp(first->arKey, second->arKey, min)) == 0) {
		return (first->nKeyLength - second->nKeyLength);
	} else {
		return r;
	}
}


void php3_key_sort(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Wrong datatype in ksort() call");
		return;
	}
	if (!ParameterPassedByReference(ht,1)) {
		php3_error(E_WARNING, "Array not passed by reference in call to ksort()");
		return;
	}
	if (hash_sort(array->value.ht, array_key_compare,0) == FAILURE) {
		return;
	}
	RETURN_TRUE;
}


void php3_count(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Wrong datatype in count() call");
		return;
	}
	if (!ParameterPassedByReference(ht,1)) {
		php3_error(E_WARNING, "Array not passed by reference in call to ksort()");
		return;
	}
	RETURN_LONG(hash_num_elements(array->value.ht));
}


/* Numbers are always smaller than strings int this function as it
 * anyway doesn't make much sense to compare two different data types.
 * This keeps it consistant and simple.
 */
static int array_data_compare(const void *a, const void *b)
{
	Bucket *f;
	Bucket *s;
	YYSTYPE *first;
	YYSTYPE *second;
	double dfirst, dsecond;

	f = *((Bucket **) a);
	s = *((Bucket **) b);

	first = (YYSTYPE *) f->pData;
	second = (YYSTYPE *) s->pData;

	if ((first->type == IS_LONG || first->type == IS_DOUBLE) &&
		(second->type == IS_LONG || second->type == IS_DOUBLE)) {
		dfirst = first->value.dval;
		dsecond = second->value.dval;
		if (first->type == IS_LONG) {
			dfirst = (double) first->value.lval;
		}
		if (second->type == IS_LONG) {
			dsecond = (double) second->value.lval;
		}
		if (dfirst < dsecond) {
			return -1;
		} else if (dfirst == dsecond) {
			return 0;
		} else {
			return 1;
		}
	}
	if ((first->type == IS_LONG || first->type == IS_DOUBLE) &&
		second->type == IS_STRING) {
		return -1;
	} else if ((first->type == IS_STRING) &&
			   (second->type == IS_LONG || second->type == IS_DOUBLE)) {
		return 1;
	}
	if (first->type == IS_STRING && second->type == IS_STRING) {
		return strcmp(first->value.strval, second->value.strval);
	}
	return 0;					/* Anything else is equal as it can't be compared */
}

static int array_reverse_data_compare(const void *a, const void *b)
{
	return array_data_compare(a,b)*-1;
}

void php3_asort(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Wrong datatype in asort() call");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to asort()");
		return;
    }
	if (hash_sort(array->value.ht, array_data_compare,0) == FAILURE) {
		return;
	}
	RETURN_TRUE;
}

void php3_arsort(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Wrong datatype in arsort() call");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to arsort()");
		return;
    }
	if (hash_sort(array->value.ht, array_reverse_data_compare,0) == FAILURE) {
		return;
	}
	RETURN_TRUE;
}

void php3_sort(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Wrong datatype in sort() call");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to sort()");
		return;
    }
	if (hash_sort(array->value.ht, array_data_compare,1) == FAILURE) {
		return;
	}
	RETURN_TRUE;
}

void php3_rsort(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Wrong datatype in rsort() call");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to rsort()");
		return;
    }
	if (hash_sort(array->value.ht, array_reverse_data_compare,1) == FAILURE) {
		return;
	}
	RETURN_TRUE;
}


void array_end(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array, *entry;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Variable passed to end() is not an array or object");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to end()");
    }
	hash_internal_pointer_end(array->value.ht);
	if (hash_get_current_data(array->value.ht, (void **) &entry) == FAILURE) {
		return;
	}
	*return_value = *entry;
	yystype_copy_constructor(return_value);
}


void array_prev(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array, *entry;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Variable passed to prev() is not an array or object");
		return;
	}
	if (!ParameterPassedByReference(ht,1)) {
		php3_error(E_WARNING, "Array not passed by reference in call to prev()");
	}
	hash_move_backwards(array->value.ht);
	if (hash_get_current_data(array->value.ht, (void **) &entry) == FAILURE) {
		return;
	}
	*return_value = *entry;
	yystype_copy_constructor(return_value);
}

void array_next(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array, *entry;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Variable passed to next() is not an array or object");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to next()");
    }
	hash_move_forward(array->value.ht);
	if (hash_get_current_data(array->value.ht, (void **) &entry) == FAILURE) {
		return;
	}
	*return_value = *entry;
	yystype_copy_constructor(return_value);
}

void array_reset(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array, *entry;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Variable passed to reset() is not an array or object");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to reset()");
    }
	hash_internal_pointer_reset(array->value.ht);
	if (hash_get_current_data(array->value.ht, (void **) &entry) == FAILURE) {
		return;
	}
	*return_value = *entry;
	yystype_copy_constructor(return_value);
}

void array_current(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array, *entry;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Variable passed to current() is not an array or object");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to current()");
    }
	if (hash_get_current_data(array->value.ht, (void **) &entry) == FAILURE) {
		return;
	}
	*return_value = *entry;
	yystype_copy_constructor(return_value);
}


void array_current_key(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *array;
	char *string_key;
	int int_key;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &array) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!(array->type & IS_HASH)) {
		php3_error(E_WARNING, "Variable passed to key() is not an array or object");
		return;
	}
    if (!ParameterPassedByReference(ht,1)) {
        php3_error(E_WARNING, "Array not passed by reference in call to key()");
    }
	switch (hash_get_current_key(array->value.ht, &string_key, &int_key)) {
		case HASH_KEY_IS_STRING:
			return_value->value.strval = string_key;
			return_value->strlen = strlen(string_key);
			return_value->type = IS_STRING;
			break;
		case HASH_KEY_IS_INT:
			return_value->type = IS_LONG;
			return_value->value.lval = int_key;
			break;
		case HASH_KEY_NON_EXISTANT:
			return;
	}
}

#ifdef __cplusplus
void php3_flush(HashTable *)
#else
void php3_flush(INTERNAL_FUNCTION_PARAMETERS)
#endif
{
#if APACHE
	TLS_VARS;
#  if MODULE_MAGIC_NUMBER > 19970110
	rflush(GLOBAL(php3_rqst));
#  else
	bflush(GLOBAL(php3_rqst)->connection->client);
#  endif
#endif
#if CGI_BINARY
	fflush(stdout);
#endif
#if USE_SAPI
	TLS_VARS;
	GLOBAL(sapi_rqst)->flush(GLOBAL(sapi_rqst)->scid);
#endif
}


void php3_sleep(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *num;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &num) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(num);
	sleep(num->value.lval);
}

void php3_usleep(INTERNAL_FUNCTION_PARAMETERS)
{
#if HAVE_USLEEP
	YYSTYPE *num;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &num) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(num);
	usleep(num->value.lval);
#endif
}

void php3_gettype(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *arg;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &arg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	switch (arg->type) {
		case IS_LONG:
			RETVAL_STRING("integer");
			break;
		case IS_DOUBLE:
			RETVAL_STRING("double");
			break;
		case IS_STRING:
			RETVAL_STRING("string");
			break;
		case IS_ARRAY:
			RETVAL_STRING("array");
			break;
		case IS_CLASS:
			RETVAL_STRING("class");
			break;
		case IS_OBJECT:
			RETVAL_STRING("object");
			break;
		default:
			RETVAL_STRING("unknown type");
	}
}


void php3_settype(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *var, *type;
	char *new_type;
	TLS_VARS;

	if (ARG_COUNT(ht) != 2 || getParameters(ht, 2, &var, &type) ==
		FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(type);
	new_type = type->value.strval;

	if (!strcasecmp(new_type, "integer")) {
		convert_to_long(var);
	} else if (!strcasecmp(new_type, "double")) {
		convert_to_double(var);
	} else if (!strcasecmp(new_type, "string")) {
		convert_to_string(var);
	} else if (!strcasecmp(new_type, "array")) {
		convert_to_array(var);
	} else if (!strcasecmp(new_type, "object")) {
		convert_to_object(var);
	} else {
		php3_error(E_WARNING, "settype: invalid type");
		RETURN_FALSE;
	}
	RETVAL_TRUE;
}


void php3_min(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE **argv;
	int argc, i;
	unsigned short min_type = IS_LONG;
	TLS_VARS;

	argc = ARG_COUNT(ht);
	/* if there is one parameter and this parameter is an array of
	 * 2 or more elements, use that array
	 */
	if (argc == 1) {
		argv = (YYSTYPE **)emalloc(sizeof(YYSTYPE *) * argc);

		if (getParametersArray(ht, argc, argv) == FAILURE ||
			argv[0]->type != IS_ARRAY) {
			WRONG_PARAM_COUNT;
		}
		if (argv[0]->value.ht->nNumOfElements < 2) {
			php3_error(E_WARNING,
					   "min: array must contain at least 2 elements");
			RETURN_FALSE;
		}
		/* replace the function parameters with the array */
		ht = argv[0]->value.ht;
		argc = ARG_COUNT(ht);
		efree(argv);
	} else if (argc < 2) {
		WRONG_PARAM_COUNT;
	}
	argv = (YYSTYPE **)emalloc(sizeof(YYSTYPE *) * argc);
	if (getParametersArray(ht, argc, argv) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	/* figure out what types to compare
	 * if the arguments contain a double, convert all of them to a double
	 * else convert all of them to long
	 */
	for (i = 0; i < argc; i++) {
		if (argv[i]->type == IS_DOUBLE) {
			min_type = IS_DOUBLE;
			break;
		}
	}
	if (min_type == IS_LONG) {
		convert_to_long(argv[0]);
		return_value->value.lval = argv[0]->value.lval;
		for (i = 1; i < argc; i++) {
			convert_to_long(argv[i]);
			if (argv[i]->value.lval < return_value->value.lval) {
				return_value->value.lval = argv[i]->value.lval;
			}
		}
	} else {
		convert_to_double(argv[0]);
		return_value->value.dval = argv[0]->value.dval;
		for (i = 1; i < argc; i++) {
			convert_to_double(argv[i]);
			if (argv[i]->value.dval < return_value->value.dval) {
				return_value->value.dval = argv[i]->value.dval;
			}
		}
	}
	efree(argv);
	return_value->type = min_type;
}


void php3_max(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE **argv;
	int argc, i;
	unsigned short max_type = IS_LONG;
	TLS_VARS;

	argc = ARG_COUNT(ht);
	/* if there is one parameter and this parameter is an array of
	 * 2 or more elements, use that array
	 */
	if (argc == 1) {
		argv = (YYSTYPE **)emalloc(sizeof(YYSTYPE *) * argc);
		if (getParametersArray(ht, argc, argv) == FAILURE ||
			argv[0]->type != IS_ARRAY) {
			WRONG_PARAM_COUNT;
		}
		if (argv[0]->value.ht->nNumOfElements < 2) {
			php3_error(E_WARNING,
					   "min: array must contain at least 2 elements");
			RETURN_FALSE;
		}
		/* replace the function parameters with the array */
		ht = argv[0]->value.ht;
		argc = ARG_COUNT(ht);
		efree(argv);
	} else if (argc < 2) {
		WRONG_PARAM_COUNT;
	}
	argv = (YYSTYPE **)emalloc(sizeof(YYSTYPE *) * argc);
	if (getParametersArray(ht, argc, argv) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	/* figure out what types to compare
	 * if the arguments contain a double, convert all of them to a double
	 * else convert all of them to long
	 */
	for (i = 0; i < argc; i++) {
		if (argv[i]->type == IS_DOUBLE) {
			max_type = IS_DOUBLE;
			break;
		}
	}
	if (max_type == IS_LONG) {
		convert_to_long(argv[0]);
		return_value->value.lval = argv[0]->value.lval;
		for (i = 1; i < argc; i++) {
			convert_to_long(argv[i]);
			if (argv[i]->value.lval > return_value->value.lval) {
				return_value->value.lval = argv[i]->value.lval;
			}
		}
	} else {
		convert_to_double(argv[0]);
		return_value->value.dval = argv[0]->value.dval;
		for (i = 1; i < argc; i++) {
			convert_to_double(argv[i]);
			if (argv[i]->value.dval > return_value->value.dval) {
				return_value->value.dval = argv[i]->value.dval;
			}
		}
	}
	efree(argv);
	return_value->type = max_type;
}


void php3_get_current_user(INTERNAL_FUNCTION_PARAMETERS)
{
	TLS_VARS;

	RETURN_STRING(_php3_get_current_user());
}


void php3_get_cfg_var(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *varname;
	char *value;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &varname)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_string(varname);
	
	if (cfg_get_string(varname->value.strval,&value)==FAILURE) {
		RETURN_FALSE;
	}
	RETURN_STRING(value);
}

void php3_set_magic_quotes_runtime(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *new_setting;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &new_setting)==FAILURE) {
		RETURN_FALSE;
	}
	convert_to_long(new_setting);
	
	php3_ini.magic_quotes_runtime=new_setting->value.lval;
	RETURN_TRUE;
}


void php3_is_type(INTERNAL_FUNCTION_PARAMETERS,int type)
{
	YYSTYPE *arg;
	
	if (ARG_COUNT(ht)!=1 || getParameters(ht, 1, &arg)==FAILURE) {
		RETURN_FALSE;
	}
	if (arg->type == type) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}


void php3_is_long(INTERNAL_FUNCTION_PARAMETERS) { php3_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_LONG); }
void php3_is_double(INTERNAL_FUNCTION_PARAMETERS) { php3_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_DOUBLE); }
void php3_is_string(INTERNAL_FUNCTION_PARAMETERS) { php3_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_STRING); }
void php3_is_array(INTERNAL_FUNCTION_PARAMETERS) { php3_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_ARRAY); }
void php3_is_object(INTERNAL_FUNCTION_PARAMETERS) { php3_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_OBJECT); }


void php3_leak(INTERNAL_FUNCTION_PARAMETERS)
{
	int leakbytes=3;
	YYSTYPE *leak;

	if (ARG_COUNT(ht)>=1) {
		if (getParameters(ht, 1, &leak)==SUCCESS) {
			convert_to_long(leak);
			leakbytes = leak->value.lval;
		}
	}
	
	emalloc(leakbytes);
}

/* 
	1st arg = error message
	2nd arg = error option
	3rd arg = optional parameters (email address or tcp address)
	4th arg = used for additional headers if email

  error options
    0 = send to php3_error_log (uses syslog or file depending on ini setting)
	1 = send via email to 3rd parameter 4th option = additional headers
	2 = send via tcp/ip to 3rd parameter (name or ip:port)
	4 = save to file in 3rd parameter
*/

void php3_error_log(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *string, *erropt, *option, *emailhead;
	int opt_err=0;
	char *message, *opt=NULL, *headers=NULL;
	TLS_VARS;

	if (ARG_COUNT(ht) != 1 || (ARG_COUNT(ht) > 2 && ARG_COUNT(ht) < 4)) {
		WRONG_PARAM_COUNT;
	}
	if (getParameters(ht,1,&string) == FAILURE){
		php3_error(E_WARNING,"Invalid argument 1 in error_log");
		RETURN_FALSE;
	} else {
		convert_to_string(string);
		message=string->value.strval;
	}
	if (ARG_COUNT(ht) > 2){
		if (getParameters(ht,2,&erropt) == FAILURE){
			php3_error(E_WARNING,"Invalid argument 2 in error_log");
			RETURN_FALSE;
		} else {
			convert_to_long(erropt);
			opt_err=erropt->value.lval;
		}
		if (getParameters(ht,3,&option) == FAILURE){
			php3_error(E_WARNING,"Invalid argument 3 in error_log");
			RETURN_FALSE;
		} else {
			convert_to_string(option);
			opt=option->value.strval;
		}
		if (ARG_COUNT(ht) > 3){
			if (getParameters(ht,4,&emailhead) == FAILURE){
				php3_error(E_WARNING,"Invalid argument 4 in error_log");
				RETURN_FALSE;
			} else {
				convert_to_string(emailhead);
				headers=emailhead->value.strval;
			}
		}
	} 
	if(_php3_error_log(opt_err,message,opt,headers)==FAILURE)
		RETURN_FALSE;
	RETURN_TRUE;
}

PHPAPI int _php3_error_log(int opt_err,char *message,char *opt,char *headers){
	FILE *logfile;

	switch(opt_err){
	case 1: /*send an email*/
		{
#if HAVE_SENDMAIL
		if (!_php3_mail(opt,"PHP3 error_log message",message,headers)){
			return FAILURE;
		}
#else
		php3_error(E_WARNING,"Mail option not available!");
		return FAILURE;
#endif
		}
		break;
	case 2: /*send to an address */
#if PHP_DEBUGGER
		if (!_php3_send_error(message,opt)){
			return FAILURE;
		}
#else
		php3_error(E_WARNING,"TCP/IP option not available!");
		return FAILURE;
#endif
		break;
	case 3: /*save to a file*/
		/*FIXME does this need safe_mode stuff?*/
		logfile=fopen(opt,"a");
		fwrite(message,sizeof(message),1,logfile);
		fclose(logfile);
		break;
	default:
		php3_log_err(message);
		break;
	}
	return SUCCESS;
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
