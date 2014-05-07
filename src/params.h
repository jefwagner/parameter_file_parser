#ifndef JW_PARAMS
#define JW_PARAMS

#include <stdio.h>
#include <stdarg.h>
#include "hashtab.h"

#define PDATA_SUCCESS 1
#define PDATA_FAILURE 0
#define PDATA_FORMAT -1
#define PDATA_MEM_ERROR -2

typedef hashtab pdata;
typedef enum{ DOUBLE_T, INT_T, STRING_T} pdata_type;

pdata pdata_alloc( size_t size);
void pdata_free( pdata pd);
int pdata_read_file( FILE *file, pdata pd);

int pdata_get_var_d( pdata pd, const char *name, double *val);
int pdata_get_var_i( pdata pd, const char *name, int *val);
int pdata_get_var_s( pdata pd, const char *name, char *val);

int pdata_listlength( pdata pd, const char *name);
int pdata_get_array_d( pdata pd, const char *name, double *val);
int pdata_get_array_i( pdata pd, const char *name, int *val);
int pdata_get_array_s( pdata pd, const char *name, char **val);

int pdata_get_list( pdata pd, const char *name, int start, int end, ...);

#endif
