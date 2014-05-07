#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "params.h"

#define VALSTR "var"
#define VALLIST "list"

/* Use a hash table to store the data from the parameter file */
/* The parameter file has list of names and values which we will store
 * in a hash table where the variable names are the keys. We will
 * rename the hash table data type `hashtab` to `pdata`.
 */

/* allocate and free wrappers for the hash table */

pdata pdata_alloc( size_t size){
  return hashtab_alloc( size);
}

void pdata_free( pdata pd){
  hashtab_free( pd);
}

/* skip over comment blocks */
/* This function skips over comment blocks in the file. A comment is
 * defined as any line where the first non-whitespace character is
 * `#`.  After the loop the stream pointer points to the first
 * non-whitespace, non-`#` character. 
 */
static int skip_comment( FILE *file){
  char c;
  while( 1 ){
    do{
      c = getc( file);
    }while( isspace( c) && c != EOF );
    switch( c ){
    case '#':
      while( c != '\n' && c != EOF ){
	c = getc( file);
      }
      if( c == EOF ){
	return 1;
      }
      break;
    case EOF:
      return 1;
    default:
      ungetc( c, file);
      return 0;
    }
  }
}

/* Fill up the pdata data structure from a parameter file */
/* This function loops through the parameter file filling up the pdata
 * data structure. The loop runs by skipping comment blocks. If a
 * comment block (or whitespace) ends the file, it breaks the
 * loop. Inside the loop it reads strings. After any comment blocks
 * the first string should be either the strings `var` or `varlist`
 *
 */
int pdata_read_file( FILE *file, pdata pd){
  int i, n, num_l;
  char str[10], vname[80], vname_iter[80], vval[80];

  /* This like loops over the comment blocks */
  while( skip_comment( file) == 0 ){
    /* we read the next string (limited to 9 characters) */
    n = fscanf( file, "%9s", str);
    /* If we have a read error, we fail with a format error */
    if( strlen( str) == 9 || n != 1 ){
      return PDATA_FORMAT;
    }
    /* If we read a the value keyword read the name and value */
    if( strcmp( str, VALSTR) == 0){
      skip_comment( file);
      n = fscanf( file, "%79s", vname);
      if( strlen( vname) == 79 || n != 1 ){
	return PDATA_FORMAT;
      }
      skip_comment( file);
      n = fscanf( file, "%79s", vval);
      if( strlen( vval) == 79 || n != 1 ){
	return PDATA_FORMAT;
      }
      /* Insert into the hash table to lookup by name */
      if( hashtab_insert( pd, vname, vval) == HT_MEM_ERROR ){
	return PDATA_MEM_ERROR;
      }
      /* If we read the list keyword read the name length */
    }else if( strcmp( str, VALLIST) == 0){
      skip_comment( file);
      n = fscanf( file, "%73s", vname);
      if( strlen( vname) == 73 || n != 1 ){
	return PDATA_FORMAT;
      }
      i = 0;
      while( 1==1 ){
	n = fscanf( file, "%79s", vval);
	if( strlen( vval) == 79 || n != 1 ){
	  return PDATA_FORMAT;
	}
	if( val[0] == '#' ||
	    strcmp( val, VALSTR) == 0 ||
	    strcpm( val, VALLIST) == 0 ){
	  break;
	}
	sprintf( vname_iter, "%s___%03d", vname, i);
	if( hashtab_insert( pd, vname, vval) == HT_MEM_ERROR ){
	  return PDATA_MEM_ERROR;
	}
	i++;
      }
    }else{
      return PDATA_FORMAT;
    }
  }
  return PDATA_SUCCESS;
}

/* get a single double or integer or string from pdata */
/* These functions are sipmle table lookups for the key `name`,
 * then copy the string into the appropriate pointer.
 */
int pdata_get_var_d( pdata pd, const char *name, double *val){
  char str[80];
  if( hashtab_find( pd, name, str) == HT_FAILURE){
    return PDATA_FAILURE;
  };
  *val = atof( str);
  return PDATA_SUCCESS;
}

int pdata_get_var_i( pdata pd, const char *name, int *val){
  char str[80];
  if( hashtab_find( pd, name, str) == HT_FAILURE){
    return PDATA_FAILURE;
  };
  *val = atoi( str);
  return PDATA_SUCCESS;
}

int pdata_get_var_s( pdata pd, const char *name, char *val){
  char str[80];
  if( hashtab_find( pd, name, str) == HT_FAILURE){
    return PDATA_FAILURE;
  };
  strcpy( val, str);
  return PDATA_SUCCESS;
}

/* Get the number of elements stored under the key `name` */
/* This function finds the number of elements under key `name`
 *
 */
int pdata_listlength( pdata pd, const char *name){
  int i=0;
  char name_iter[80];
  char str[80];
  do{
    sprintf( name_iter, "%s___%03d", name, i);
    i++;
  }while( hashtab_find( pd, name_iter, str) == HT_SUCCESS );
  return i;
}

/* Get an array of double/integers/strings from pdata */
/* These functions do a series of table lookups and fill up and array
 * pointed to by the appropriate pointer.
 */
int pdata_get_array_d( pdata pd, const char *name, double *val){
  int i=0;
  char name_iter[80];
  char str[80];
  sprintf( name_iter, "%s___%03d", name, i);
  while( hashtab_find( pd, name_iter, str) == HT_SUCCESS ){
    val[i] = atof( str);
    i++;
    sprintf( name_iter, "%s___%03d", name, i);
  }
  return i;
}

int pdata_get_array_i( pdata pd, const char *name, int n, int *val){
  int i=0;
  char name_iter[80];
  char str[80];
  sprintf( name_iter, "%s___%03d", name, i);
  while( hashtab_find( pd, name_iter, str) == HT_SUCCESS ){
    val[i] = atoi( str);
    i++;
    sprintf( name_iter, "%s___%03d", name, i);
  }
  return i;
}

int pdata_get_array_s( pdata pd, const char *name, int n, char **val){
  int i=0;
  char name_iter[80];
  char str[80];
  sprintf( name_iter, "%s___%03d", name, i);
  while( hashtab_find( pd, name_iter, str) == HT_SUCCESS ){
    strcpy( val[i], str);
    i++;
    sprintf( name_iter, "%s___%03d", name, i);
  }
  return i;
}

/* Read in multiple data of various types */
/* Use the `stdarg.h` macros to make a function that can take a
 * variable number of arguments. The arguments should always come in
 * pairs `data_type` (INT_T, DOUBLE_T, STRING_T), then a pointer to
 * that data type. This can be used to write simple wrappers to fill
 * up custom defined structs.
 */
int pdata_get_varlist( pdata pd, const char *name, 
		       int start, int end, ...){
  int i, *j;
  double *d;
  char *s;
  char name_iter[80], str[80];
  va_list vl;
 
  /* make sure there are an even number of arguments */
  if( start > end || (end-start)%2 != 0 ){
    return PDATA_FAILURE;
  }
  /* make sure the last argument is there */
  sprintf( name_iter, "%s___%03d", name, end);
  if( hashtab_find( pdata, name_iter, str) == HT_FAILURE){
    return PDATA_FAILURE;
  }

  /* Go through the arguments */
  va_start( vl, end);
  for( i=start; i<end; i++){
    sprintf( name_iter, "%s___%03d", name, start+i);
    if( hashtab_find( pd, name_iter, str) == HT_FAILURE){
      return i;
    };
    switch( va_arg( vl, pdata_type)){
    case DOUBLE_T:
      d = va_arg( vl, double*);
      *d = atof( str);
      break;
    case INT_T:
      j = va_arg( vl, int*);
      *j = atoi( str);
      break;
    case STRING_T:
      s = va_arg( vl, char*);
      strcpy( s, str);
      break;
    default:
      break;
    }
  }
  va_end( vl);

  return PDATA_SUCCESS;
}
