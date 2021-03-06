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
   | Authors: Jim Winstead (jimw@php.net)                                 |
   +----------------------------------------------------------------------+
 */
#if defined(COMPILE_DL)
#include "dl/phpdl.h"
#endif

#if defined(THREAD_SAFE)
#include "tls.h"
#endif
#include <stdlib.h>

#include "parser.h"
#include "internal_functions.h"
#include "list.h"

#if DBASE
#include "../dbase/dbf.h"
#if defined(THREAD_SAFE)
DWORD DbaseTls;
static int numthreads=0;
void *dbase_mutex;

typedef struct dbase_global_struct{
	int le_dbhead;
}dbase_global_struct;

#define DBase_GLOBAL(a) dbase_globals->a

#define DBase_TLS_VARS \
	dbase_global_struct *dbase_globals; \
	dbase_globals=TlsGetValue(DbaseTls); 

#else
static int le_dbhead;
#define DBase_GLOBAL(a) a
#define DBase_TLS_VARS
#endif

#include <fcntl.h>
#include <errno.h>


static void _close_dbase(dbhead_t *dbhead)
{
	close(dbhead->db_fd);
	free_dbf_head(dbhead);
}


int php3_minit_dbase(INITFUNCARG)
{
#if defined(THREAD_SAFE)
	dbase_global_struct *dbase_globals;
#if !defined(COMPILE_DL)
	CREATE_MUTEX(dbase_mutex,"DBase_TLS");
	SET_MUTEX(dbase_mutex);
	numthreads++;
	if(numthreads==1){
	if((DbaseTls=TlsAlloc())==0xFFFFFFFF){
		FREE_MUTEX(dbase_mutex);
		return 0;
	}}
	FREE_MUTEX(dbase_mutex);
#endif
	dbase_globals = (dbase_global_struct *) LocalAlloc(LPTR, sizeof(dbase_global_struct)); 
	TlsSetValue(DbaseTls, (void *) dbase_globals);
#endif
	DBase_GLOBAL(le_dbhead) = register_list_destructors(_close_dbase,NULL);
	return SUCCESS;
}

static int php3_mend_dbase(void){
#if defined(THREAD_SAFE)
	dbase_global_struct *dbase_globals;
	dbase_globals = TlsGetValue(DbaseTls); 
	if (dbase_globals != 0) 
		LocalFree((HLOCAL) dbase_globals); 
#if !defined(COMPILE_DL)
	SET_MUTEX(dbase_mutex);
	numthreads--;
	if(!numthreads){
	if(!TlsFree(DbaseTls)){
		FREE_MUTEX(dbase_mutex);
		return 0;
	}}
	FREE_MUTEX(dbase_mutex);
#endif
#endif
	return SUCCESS;
}

void php3_dbase_open(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbf_name, *options;
	dbhead_t *dbh;
	int handle;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 2 || getParameters(ht,2,&dbf_name,&options)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(dbf_name);
	convert_to_long(options);

	dbh = dbf_open(dbf_name->value.strval, options->value.lval);
	if (dbh == NULL) {
		php3_error(E_WARNING, "unable to open database %s", dbf_name->value.strval);
		RETURN_FALSE;
	}

	handle = php3_list_insert(dbh, DBase_GLOBAL(le_dbhead));
	RETURN_LONG(handle);
}

void php3_dbase_close(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbh_id;
	dbhead_t *dbh;
	int dbh_type;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht,1,&dbh_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(dbh_id);
	dbh = php3_list_find(dbh_id->value.lval, &dbh_type);
	if (!dbh || dbh_type != DBase_GLOBAL(le_dbhead)) {
		php3_error(E_WARNING, "Unable to find database for identifier %d", dbh_id->value.lval);
		RETURN_FALSE;
	}

	php3_list_delete(dbh_id->value.lval);
}

void php3_dbase_numrecords(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbh_id;
	dbhead_t *dbh;
	int dbh_type;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht,1,&dbh_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(dbh_id);
	dbh = php3_list_find(dbh_id->value.lval, &dbh_type);
	if (!dbh || dbh_type != DBase_GLOBAL(le_dbhead)) {
		php3_error(E_WARNING, "Unable to find database for identifier %d", dbh_id->value.lval);
		RETURN_FALSE;
	}

	RETURN_LONG(dbh->db_records);
}

void php3_dbase_numfields(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbh_id;
	dbhead_t *dbh;
	int dbh_type;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht,1,&dbh_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(dbh_id);
	dbh = php3_list_find(dbh_id->value.lval, &dbh_type);
	if (!dbh || dbh_type != DBase_GLOBAL(le_dbhead)) {
		php3_error(E_WARNING, "Unable to find database for identifier %d", dbh_id->value.lval);
		RETURN_FALSE;
	}

	RETURN_LONG(dbh->db_nfields);
}

void php3_dbase_pack(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbh_id;
	dbhead_t *dbh;
	int dbh_type;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 1 || getParameters(ht,1,&dbh_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(dbh_id);
	dbh = php3_list_find(dbh_id->value.lval, &dbh_type);
	if (!dbh || dbh_type != DBase_GLOBAL(le_dbhead)) {
		php3_error(E_WARNING, "Unable to find database for identifier %d", dbh_id->value.lval);
		RETURN_FALSE;
	}

        pack_dbf(dbh);
        put_dbf_info(dbh);
}

void php3_dbase_add_record(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbh_id, *fields, *field;
	dbhead_t *dbh;
	int dbh_type;

	int num_fields;
	dbfield_t *dbf, *cur_f;
	char *cp, *t_cp;
	int i;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 2 || getParameters(ht,2,&dbh_id,&fields)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(dbh_id);
	if (fields->type != IS_ARRAY) {
		php3_error(E_WARNING, "Expected array as second parameter");
		RETURN_FALSE;
	}

	dbh = php3_list_find(dbh_id->value.lval, &dbh_type);
	if (!dbh || dbh_type != DBase_GLOBAL(le_dbhead)) {
		php3_error(E_WARNING, "Unable to find database for identifier %d", dbh_id->value.lval);
		RETURN_FALSE;
	}

	num_fields = hash_num_elements(fields->value.ht);

	if (num_fields != dbh->db_nfields) {
		php3_error(E_WARNING, "Wrong number of fields specified");
		RETURN_FALSE;
	}

	cp = t_cp = (char *)emalloc(dbh->db_rlen + 1);
	if (!cp) {
		php3_error(E_WARNING, "unable to allocate memory");
		RETURN_FALSE;
	}
	*t_cp++ = VALID_RECORD;

	dbf = dbh->db_fields;
	for (i = 0, cur_f = dbf; cur_f < &dbf[num_fields]; i++, cur_f++) {
		if (hash_index_find(fields->value.ht, i, (void **)&field) == FAILURE) {
			php3_error(E_WARNING, "unexpected error");
			efree(cp);
			RETURN_FALSE;
		}
		convert_to_string(field);
		sprintf(t_cp, cur_f->db_format, field->value.strval); 
		t_cp += cur_f->db_flen;
	}

	dbh->db_records++;
	if (put_dbf_record(dbh, dbh->db_records, cp) < 0) {
		php3_error(E_WARNING, "unable to put record at %ld", dbh->db_records);
		efree(cp);
		RETURN_FALSE;
	}

        put_dbf_info(dbh);
	efree(cp);
}

void php3_dbase_delete_record(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbh_id, *record;
	dbhead_t *dbh;
	int dbh_type;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 2 || getParameters(ht,2,&dbh_id,&record)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(dbh_id);
	convert_to_long(record);

	dbh = php3_list_find(dbh_id->value.lval, &dbh_type);
	if (!dbh || dbh_type != DBase_GLOBAL(le_dbhead)) {
		php3_error(E_WARNING, "Unable to find database for identifier %d", dbh_id->value.lval);
		RETURN_FALSE;
	}

	if (del_dbf_record(dbh, record->value.lval) < 0) {
		if (record->value.lval > dbh->db_records) {
			php3_error(E_WARNING, "record %d out of bounds", record->value.lval);
		} else {
			php3_error(E_WARNING, "unable to delete record %d", record->value.lval);
		}
	}

        put_dbf_info(dbh);
}

void php3_dbase_get_record(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *dbh_id, *record;
	dbhead_t *dbh;
	int dbh_type;
	dbfield_t *dbf, *cur_f;
	char *data, *fnp, *str_value;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 2 || getParameters(ht,2,&dbh_id,&record)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(dbh_id);
	convert_to_long(record);

	dbh = php3_list_find(dbh_id->value.lval, &dbh_type);
	if (!dbh || dbh_type != DBase_GLOBAL(le_dbhead)) {
		php3_error(E_WARNING, "Unable to find database for identifier %d", dbh_id->value.lval);
		RETURN_FALSE;
	}

	if ((data = get_dbf_record(dbh, record->value.lval)) == NULL) {
		php3_error(E_WARNING, "Tried to read bad record %d", record->value.lval);
		RETURN_FALSE;
	}

	dbf = dbh->db_fields;

	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}

        fnp = (char *)emalloc(dbh->db_rlen);
        for (cur_f = dbf; cur_f < &dbf[dbh->db_nfields]; cur_f++) {
		/* get the value */
		str_value = (char *)emalloc(cur_f->db_flen + 1);
                sprintf(str_value, cur_f->db_format, get_field_val(data, cur_f, fnp));

		/* now convert it to the right php internal type */
	        switch (cur_f->db_type) {
		case 'C':
			add_next_index_string(return_value,str_value,1);
			break;
		case 'N':	/* FALLS THROUGH */
		case 'L':	/* FALLS THROUGH */
		case 'D':
			if (cur_f->db_fdc == 0) {
				add_next_index_long(return_value, strtol(str_value, NULL, 10));
			} else {
				add_next_index_double(return_value, atof(str_value));
			}
			break;
		case 'M':
			/* this is a memo field. don't know how to deal with
			   this yet */
			break;
		default:
			/* should deal with this in some way */
			break;
		}
		efree(str_value);
        }
        efree(fnp);

	/* mark whether this record was deleted */
	if (data[0] == '*') {
		add_assoc_long(return_value,"deleted",1);
	}
	else {
		add_assoc_long(return_value,"deleted",0);
	}

	free(data);
}

void php3_dbase_create(INTERNAL_FUNCTION_PARAMETERS) {
	YYSTYPE *filename, *fields, *field, *value;
	int fd;
	dbhead_t *dbh;

	int num_fields;
	dbfield_t *dbf, *cur_f;
	int i, rlen, handle;
	DBase_TLS_VARS;

	if (ARG_COUNT(ht) != 2 || getParameters(ht,2,&filename,&fields)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(filename);

	if (fields->type != IS_ARRAY) {
		php3_error(E_WARNING, "Expected array as second parameter");
		RETURN_FALSE;
	}

	if ((fd = open(filename->value.strval, O_RDWR|O_CREAT, 0644)) < 0) {
		php3_error(E_WARNING, "Unable to create database (%d): %s", errno, strerror(errno));
		RETURN_FALSE;
	}

	num_fields = hash_num_elements(fields->value.ht);

	/* have to use regular malloc() because this gets free()d by
	   code in the dbase library */
	dbh = (dbhead_t *)malloc(sizeof(dbhead_t));
	dbf = (dbfield_t *)malloc(sizeof(dbfield_t) * num_fields);
	if (!dbh || !dbf) {
		php3_error(E_WARNING, "Unable to allocate memory for header info");
		RETURN_FALSE;
	}
	
	/* initialize the header structure */
	dbh->db_fields = dbf;
	dbh->db_fd = fd;
	dbh->db_dbt = DBH_TYPE_NORMAL;
	strcpy(dbh->db_date, "19930818");
	dbh->db_records = 0;
	dbh->db_nfields = num_fields;
	dbh->db_hlen = sizeof(struct dbf_dhead) + 2 + num_fields * sizeof(struct dbf_dfield);

	rlen = 1;
	
	for (i = 0, cur_f = dbf; i < num_fields; i++, cur_f++) {
		/* look up the first field */
		if (hash_index_find(fields->value.ht, i, (void **)&field) == FAILURE) {
			php3_error(E_WARNING, "unable to find field %d", i);
			free_dbf_head(dbh);
			RETURN_FALSE;
		}

		if (field->type != IS_ARRAY) {
			php3_error(E_WARNING, "second parameter must be array of arrays");
			free_dbf_head(dbh);
			RETURN_FALSE;
		}

		/* field name */
		if (hash_index_find(field->value.ht, 0, (void **)&value) == FAILURE) {
			php3_error(E_WARNING, "expected field name as first element of list in field %d", i);
			free_dbf_head(dbh);
			RETURN_FALSE;
		}
		convert_to_string(value);
		copy_crimp(cur_f->db_fname, value->value.strval, value->strlen);

		/* field type */
		if (hash_index_find(field->value.ht,1,(void **)&value) == FAILURE) {
			php3_error(E_WARNING, "expected field type as sececond element of list in field %d", i);
			RETURN_FALSE;
		}
		convert_to_string(value);
		cur_f->db_type = toupper(*value->value.strval);

		cur_f->db_fdc = 0;

		/* verify the field length */
		switch (cur_f->db_type) {
		case 'L':
			cur_f->db_flen = 1;
			break;
		case 'M':
			cur_f->db_flen = 9;
			dbh->db_dbt = DBH_TYPE_MEMO;
			/* should create the memo file here, probably */
			break;
		case 'D':
			cur_f->db_flen = 8;
			break;
		case 'N':
		case 'C':
			/* field length */
			if (hash_index_find(field->value.ht,2,(void **)&value) == FAILURE) {
				php3_error(E_WARNING, "expected field length as third element of list in field %d", i);
				free_dbf_head(dbh);
				RETURN_FALSE;
			}
			convert_to_long(value);
			cur_f->db_flen = value->value.lval;

			if (cur_f->db_type == 'N') {
				if (hash_index_find(field->value.ht,3,(void **)&value) == FAILURE) {
					php3_error(E_WARNING, "expected field precision as fourth element of list in field %d", i);
					free_dbf_head(dbh);
					RETURN_FALSE;
				}
				convert_to_long(value);
				cur_f->db_fdc = value->value.lval;
			}
			break;
		default:
			php3_error(E_WARNING, "unknown field type '%c'", cur_f->db_type);
		}
		cur_f->db_foffset = rlen;
		rlen += cur_f->db_flen;
	
		cur_f->db_format = get_dbf_f_fmt(cur_f);
	}

	dbh->db_rlen = rlen;
        put_dbf_info(dbh);

	handle = php3_list_insert(dbh, DBase_GLOBAL(le_dbhead));
	RETURN_LONG(handle);
}

function_entry dbase_functions[] = {
	{"dbase_open",			php3_dbase_open,			NULL},
	{"dbase_create",		php3_dbase_create,			NULL},
	{"dbase_close",			php3_dbase_close,			NULL},
	{"dbase_numrecords",	php3_dbase_numrecords,		NULL},
	{"dbase_numfields",		php3_dbase_numfields,		NULL},
	{"dbase_add_record",	php3_dbase_add_record,		NULL},
	{"dbase_get_record",	php3_dbase_get_record,		NULL},
	{"dbase_delete_record",	php3_dbase_delete_record,	NULL},
	{"dbase_pack",			php3_dbase_pack,			NULL},
	{NULL, NULL, NULL}
};

php3_module_entry dbase_module_entry = {
	"DBase", dbase_functions, php3_minit_dbase, php3_mend_dbase, NULL, NULL, NULL, 0, 0, 0, NULL
};


#if defined(COMPILE_DL)
DLEXPORT php3_module_entry *get_module(void) { return &dbase_module_entry; }

#if (WIN32|WINNT) && defined(THREAD_SAFE)

/*NOTE: You should have an odbc.def file where you
export DllMain*/
BOOL WINAPI DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
    switch( ul_reason_for_call ) {
    case DLL_PROCESS_ATTACH:
		if((DbaseTls=TlsAlloc())==0xFFFFFFFF){
			return 0;
		}
		break;    
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if(!TlsFree(DbaseTls)){
			return 0;
		}
		break;
    }
    return 1;
}
#endif
#endif

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
