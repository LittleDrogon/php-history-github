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
   | Author: Bj?rn Borud - Guardian Networks AS <borud@guardian.no>    |
   +----------------------------------------------------------------------+
 */
/* $Id: soundex.c,v 1.20 1997/12/31 15:56:49 rasmus Exp $ */

#include "parser.h"
#include "internal_functions.h"
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "php3_string.h"

/* Simple soundex algorithm as described by Knuth in TAOCP, vol 3 */
void soundex(INTERNAL_FUNCTION_PARAMETERS)
{
	char l, u;
	char *somestring;
	int i, j, n;
	YYSTYPE *arg;

	/* pad with '0' and terminate with 0 ;-) */
	char soundex[5] =
	{'0', '0', '0', '0', 0};

	static char soundex_table[26] =
	{0,							/* A */
	 '1',						/* B */
	 '2',						/* C */
	 '3',						/* D */
	 0,							/* E */
	 '1',						/* F */
	 '2',						/* G */
	 0,							/* H */
	 0,							/* I */
	 '2',						/* J */
	 '2',						/* K */
	 '4',						/* L */
	 '5',						/* M */
	 '5',						/* N */
	 0,							/* O */
	 '1',						/* P */
	 '2',						/* Q */
	 '6',						/* R */
	 '2',						/* S */
	 '3',						/* T */
	 0,							/* U */
	 '1',						/* V */
	 0,							/* W */
	 '2',						/* X */
	 0,							/* Y */
	 '2'};						/* Z */
	if (ARG_COUNT(ht) != 1 || getParameters(ht, 1, &arg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(arg);

	somestring = arg->value.strval;

	n = arg->strlen;

	/* convert chars to upper case and strip non-letter chars */
	j = 0;
	for (i = 0; i < n; i++) {
		u = toupper(somestring[i]);
		if ((u > 64) && (u < 91)) {
			somestring[j] = u;
			j++;
		}
	}

	/* null-terminate string */
	somestring[j] = 0;

	n = strlen(somestring);

	/* prefix soundex string with first valid char */
	soundex[0] = somestring[0];

	/* remember first char */
	l = soundex_table[((somestring[0]) - 65)];

	j = 1;

	/* build soundex string */
	for (i = 1; i < n && j < 4; i++) {
		u = soundex_table[((somestring[i]) - 65)];

		if (u != l) {
			if (u != 0) {
				soundex[(int) j++] = u;
			}
			l = u;
		}
	}
	return_value->value.strval = soundex;
	return_value->strlen = 4;
	yystype_copy_constructor(return_value);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
