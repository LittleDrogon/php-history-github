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


/* $Id: list.c,v 1.74 1998/01/19 23:27:14 shane Exp $ */

#ifdef THREAD_SAFE
#include "tls.h"
#endif
#include "parser.h"
#include "list.h"
#include "modules.h"

#include "functions/db.h"

#if HAVE_DIRENT_H
#include <dirent.h>
#endif

#if MSVC5
#if !(APACHE)
#define NEEDRDH 1
#endif
#include "win32/readdir.h"
#endif
HashTable list,plist;
PHPAPI int le_index_ptr;

PHPAPI int php3_list_do_insert(HashTable *list,void *ptr, int type)
{
	int index;
	list_entry le;

	index = hash_next_free_element(list);

	if (index==0) index++;

	le.ptr=ptr;
	le.type=type;
	hash_index_update(list, index, (void *) &le, sizeof(list_entry), NULL);
	return index;
}

PHPAPI int php3_list_do_delete(HashTable *list,int id)
{
	return hash_index_del(list, id);
}

PHPAPI void *php3_list_do_find(HashTable *list,int id, int *type)
{
	list_entry *le;

	if (hash_index_find(list, id, (void **) &le)==SUCCESS) {
		*type = le->type;
		return le->ptr;
	} else {
		*type = -1;
		return NULL;
	}
}

PHPAPI void list_entry_destructor(void *ptr)
{
	list_entry *le = (list_entry *) ptr;
	list_destructors_entry *ld;
	TLS_VARS;
	
	if (hash_index_find(&GLOBAL(list_destructors),le->type,(void **) &ld)==SUCCESS) {
		if (ld->list_destructor) {
			(ld->list_destructor)(le->ptr);
		}
	} else {
		php3_error(E_WARNING,"Unknown list entry type in request shutdown (%d)",le->type);
	}
}


PHPAPI void plist_entry_destructor(void *ptr)
{
	list_entry *le = (list_entry *) ptr;
	list_destructors_entry *ld;
	TLS_VARS;

	if (hash_index_find(&GLOBAL(list_destructors),le->type,(void **) &ld)==SUCCESS) {
		if (ld->plist_destructor) {
			(ld->plist_destructor)(le->ptr);
		}
	} else {
		php3_error(E_WARNING,"Unknown persistent list entry type in module shutdown (%d)",le->type);
	}
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
