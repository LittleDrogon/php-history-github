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

/* $Id: link.c,v 1.22 1997/12/31 15:56:32 rasmus Exp $ */
#ifdef THREAD_SAFE
#include "tls.h"
#endif
#include "parser.h"
#include "internal_functions.h"

#include <stdlib.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <string.h>
#if HAVE_PWD_H
#if MSVC5
#include "win32/pwd.h"
#else
#include <pwd.h>
#endif
#endif
#if HAVE_GRP_H
#if MSVC5
#include "win32/grp.h"
#else
#include <grp.h>
#endif
#endif
#include <errno.h>
#include <ctype.h>

#include "safe_mode.h"
#include "php3_link.h"

void php3_readlink(INTERNAL_FUNCTION_PARAMETERS)
{
#if HAVE_SYMLINK
	YYSTYPE *filename;
	char buff[256];
	int ret;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &filename) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(filename);

	ret = readlink(filename->value.strval, buff, 255);
	if (ret == -1) {
		php3_error(E_WARNING, "readlink failed (%s)", strerror(errno));
		RETURN_FALSE;
	}
	/* Append NULL to the end of the string */
	buff[ret] = '\0';
	RETURN_STRING(buff);
#endif
}

void php3_linkinfo(INTERNAL_FUNCTION_PARAMETERS)
{
#if HAVE_SYMLINK
	YYSTYPE *filename;
	struct stat sb;
	int ret;

	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &filename) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(filename);

	ret = lstat(filename->value.strval, &sb);
	if (ret == -1) {
		php3_error(E_WARNING, "LinkInfo failed (%s)", strerror(errno));
		RETURN_LONG(-1L);
	}
	RETURN_LONG((long) sb.st_dev);
#endif
}

void php3_symlink(INTERNAL_FUNCTION_PARAMETERS)
{
#if HAVE_SYMLINK
	YYSTYPE *topath, *frompath;
	int ret;

	if (ARG_COUNT(ht) != 2 || getParameters(ht, 2, &topath, &frompath) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(topath);
	convert_to_string(frompath);

#if PHP_SAFE_MODE
	if (!_php3_checkuid(topath->value.strval, 2)) {
		php3_error(E_WARNING, "SAFE MODE Restriction in effect.  Invalid owner of file to be linked.");
		RETURN_FALSE;
	}
#endif

	ret = symlink(topath->value.strval, frompath->value.strval);
	if (ret == -1) {
		php3_error(E_WARNING, "SymLink failed (%s)", strerror(errno));
		RETURN_FALSE;
	}
	RETURN_TRUE;
#endif
}

void php3_link(INTERNAL_FUNCTION_PARAMETERS)
{
#if HAVE_LINK
	YYSTYPE *topath, *frompath;
	int ret;

	if (ARG_COUNT(ht) != 2 || getParameters(ht, 2, &topath, &frompath) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(topath);
	convert_to_string(frompath);

#if PHP_SAFE_MODE
	if (!_php3_checkuid(topath->value.strval, 2)) {
		php3_error(E_WARNING, "SAFE MODE Restriction in effect.  Invalid owner of file to be linked.");
		RETURN_FALSE;
	}
#endif

	ret = link(topath->value.strval, frompath->value.strval);
	if (ret == -1) {
		php3_error(E_WARNING, "Link failed (%s)", strerror(errno));
		RETURN_FALSE;
	}
	RETURN_TRUE;
#endif
}

void php3_unlink(INTERNAL_FUNCTION_PARAMETERS)
{
	YYSTYPE *filename;
	int ret;
	TLS_VARS;
	
	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &filename) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(filename);

#if PHP_SAFE_MODE
	if (!_php3_checkuid(filename->value.strval, 2)) {
		php3_error(E_WARNING, "SAFE MODE Restriction in effect.  Invalid owner of file to be unlinked.");
		RETURN_FALSE;
	}
#endif

	ret = unlink(filename->value.strval);
	if (ret == -1) {
		php3_error(E_WARNING, "Unlink failed (%s)", strerror(errno));
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

function_entry link_functions[] = {
	{"readlink",		php3_readlink,		NULL},
	{"linkinfo",		php3_linkinfo,		NULL},
	{"symlink",			php3_symlink,		NULL},
	{"link",			php3_link,			NULL},
	{"unlink",			php3_unlink,		NULL},
	{NULL, NULL, NULL}
};


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
