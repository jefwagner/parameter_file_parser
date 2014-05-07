#ifndef JW_HASHTAB
#define JW_HASHTAB

/*! opaque pointer to hash table */
typedef struct hashtab_struct *hashtab;

/*! public interface */
hashtab hashtab_alloc( void);
void hashtab_free( hashtab ht);
int hashtab_find( hashtab ht, const char *key, char *value);
int hashtab_insert( hashtab ht, const char *key, const char *value);
int hashtab_remove( hashtab ht, const char *key, char *value);

/*! hash table status codes */
#define HT_SUCCESS 0
#define HT_FAILURE -1
#define HT_MEM_ERROR 1

#endif
