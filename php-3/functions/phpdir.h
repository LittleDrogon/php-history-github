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
   | Authors: Rasmus Lerdorf <rasmus@lerdorf.on.ca>                       |
   +----------------------------------------------------------------------+
 */


/* $Id: phpdir.h,v 1.2 1997/12/31 15:56:45 rasmus Exp $ */

#ifndef _PHPDIR_H
#define _PHPDIR_H

/* directory functions */
extern void php3_opendir(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_closedir(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_chdir(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_rewinddir(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_readdir(INTERNAL_FUNCTION_PARAMETERS);
extern void php3_getdir(INTERNAL_FUNCTION_PARAMETERS);

#endif /* _PHPDIR_H */
