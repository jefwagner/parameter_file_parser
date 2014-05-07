/*! neccesary standard library includes */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "hashtab.h"

#define MINSIZE 256
#define KEYSIZE 80
#define VALSIZE 80

/*! hash tree and bucket structures */
typedef struct{
  unsigned int hkey;
  char key[KEYSIZE];
  char value[VALSIZE];
} ht_bucket;

struct hashtab_struct{
  ht_bucket *array;
  size_t max_array_size;
  size_t array_size;
};

/*! local function prototypes */
static int hashtab_grow( hashtab ht);
static int hashtab_shrink( hashtab ht);
static void ht_add( hashtab ht, unsigned int index,
		    unsigned int hkey, const char *key, const char *value);
static void ht_shift_down( hashtab ht, unsigned int index);
static void ht_shift_up( hashtab ht, unsigned int i);
/*! The hash function used */
static uint32_t oat_hash( const char *key);

hashtab hashtab_alloc( void){
  int i;
  hashtab ht = (hashtab) malloc( sizeof( struct hashtab_struct));
  ht->array = (ht_bucket *) malloc( MINSIZE*sizeof( ht_bucket));
  ht->max_array_size = MINSIZE;
  ht->array_size = 0;
  for( i=0; i<ht->max_array_size; i++){
    ht->array[i].key[0] = '\0';
  }
  return ht;
}

void hashtab_free( hashtab ht){
  free( ht->array);
  free( ht);
}

int hashtab_find( hashtab ht, const char *key, char *value){
  unsigned int i, hkey;
  size_t as = ht->max_array_size;

  /*! Hash the key */
  hkey = oat_hash( key) % as;
  i = hkey;

  while( strcmp( ht->array[i].key, key) != 0 && 
	 ht->array[i].key[0] != '\0' ){
    i=(i+1)%as;
  }
  if( ht->array[i].key[0] == '\0'){
    return HT_FAILURE;
  }

  strcpy( value, ht->array[i].value);
  return HT_SUCCESS;
}

static void ht_add( hashtab ht, unsigned int index,
		    unsigned int hkey, const char *key, const char *value){
  ht->array[index].hkey = hkey;
  strcpy( ht->array[index].key, key);
  strcpy( ht->array[index].value, value);
  ht->array_size++;
}

static void ht_shift_down( hashtab ht, unsigned int index){
  unsigned int k = 0;
  size_t as = ht->max_array_size;
  do{ 
    k++; 
  }while( ht->array[(index+k)%as].key[0] != '\0');
  while( k>0 ){
    ht->array[(index+k)%as].hkey = ht->array[(index+k-1)%as].hkey;
    strcpy( ht->array[(index+k)%as].key, ht->array[(index+k-1)%as].key);
    strcpy( ht->array[(index+k)%as].value, ht->array[(index+k-1)%as].value);
    k--;
  }
}

int hashtab_insert( hashtab ht, const char *key, const char *value){
  unsigned int i, j, hkey;
  size_t as;

  if( (double) ht->array_size / (double) ht->max_array_size > 0.6666 ){
    if( hashtab_grow( ht) == HT_MEM_ERROR ){
      return HT_MEM_ERROR;
    }
  }

  as = ht->max_array_size;
  /*! Hash the key */
  hkey = oat_hash( key) % as;
  i = hkey;
  /*! Check for direct input */
  if( ht->array[i].key[0] == '\0'){
    ht_add( ht, i, hkey, key, value);
    return HT_SUCCESS;
  }else if( strcmp( ht->array[i].key, key) == 0){
    strcpy( ht->array[i].value, value);
    return HT_SUCCESS;
    
    /*! If spot is occupied shift to next spot */
  }else if( ht->array[i].hkey == hkey){
    do{ 
      i=(i+1)%as; 
    }while( ht->array[i].hkey != hkey &&
	    ht->array[i].key[0] != '\0' );
  }
  for( j=0; j<ht->array_size+1; j++){

    /*! Direct input if we can */
    if( ht->array[(i+j)%as].key[0] == '\0'){
      ht_add( ht, (i+j)%as, hkey, key, value);
      return HT_SUCCESS;
    }
    if( strcmp( ht->array[(i+j)%as].key, key) == 0){
      strcpy( ht->array[(i+j)%as].value, value);
      return HT_SUCCESS;
    }
    /*! Otherwise shift down and input */
    if( ht->array[(i+j)%as].hkey != hkey){
      ht_shift_down( ht, (i+j)%as);
      ht_add( ht, (i+j)%as, hkey, key, value);
      return HT_SUCCESS;
    }
  }
  return HT_FAILURE;
}

static void ht_shift_up( hashtab ht, unsigned int index){
  unsigned int i, j;
  int diff;
  size_t as = ht->max_array_size;

  i=index;
  j=1;
  while( ht->array[(i+j)%as].key[0] != '\0'){
    diff = ((int) ht->array[(i+j)%as].hkey) - ((int) i);
    if( diff >= as/2 ){ diff -= (int) as; }
    if( diff <= 0 ){
      ht->array[i].hkey = ht->array[(i+j)%as].hkey;
      strcpy( ht->array[i].key, ht->array[(i+j)%as].key);
      strcpy( ht->array[i].value, ht->array[(i+j)%as].value);
      ht->array[(i+j)%as].key[0] = '\0';
      /* ht_shift_up( ht, (i+j)%as ); */
      i = (i+j)%as;
      j = 0;
    }
    j++;
  }
}

int hashtab_remove( hashtab ht, const char *key, char *value){
  unsigned int i, hkey;
  size_t as; 

  if( ht->max_array_size > MINSIZE &&
      (double) ht->array_size / (double) ht->max_array_size < 0.16666 ){
    if( hashtab_shrink( ht) == HT_MEM_ERROR ){
      return HT_MEM_ERROR;
    }
  }
  as = ht->max_array_size;

  /*! Hash the key */
  hkey = oat_hash( key) % as;
  i = hkey;

  while( strcmp( ht->array[i].key, key) != 0 && 
	 ht->array[i].key[0] != '\0'){
    i=(i+1)%as;
  }
  if( ht->array[i].key[0] == '\0'){
    return HT_FAILURE;
  }

  strcpy( value, ht->array[i].value);
  ht->array[i].key[0] = '\0';
  ht->array_size--;
  ht_shift_up( ht, i);
  return HT_SUCCESS;
}

static int hashtab_grow( hashtab ht){
  int i;
  size_t old_array_size;
  ht_bucket *old_array;
  ht_bucket *new_array = (ht_bucket *) 
    malloc( 2*(ht->max_array_size)*sizeof(ht_bucket));
  if( new_array == NULL ){
    return HT_MEM_ERROR;
  }
  old_array = ht->array;
  ht->array = new_array;
  ht->array_size = 0;
  old_array_size = ht->max_array_size;
  ht->max_array_size *= 2;
  for( i=0; i<ht->max_array_size; i++){
    ht->array[i].key[0] = '\0';
  }
  for( i=0; i<old_array_size; i++){
    if( old_array[i].key[0] != '\0' ){
      hashtab_insert( ht, old_array[i].key, old_array[i].value );
    }
  }
  free( old_array);
  return HT_SUCCESS;
} 

static int hashtab_shrink( hashtab ht){
  int i;
  size_t old_array_size;
  ht_bucket *old_array;
  ht_bucket *new_array = (ht_bucket *)
    malloc( (ht->max_array_size)/2*sizeof(ht_bucket));
  if( new_array == NULL ){
    return HT_MEM_ERROR;
  }
  old_array = ht->array;
  ht->array = new_array;
  ht->array_size = 0;
  old_array_size = ht->max_array_size;
  ht->max_array_size /= 2;
  for( i=0; i<ht->max_array_size; i++){
    ht->array[i].key[0] = '\0';
  }
  for( i=0; i<old_array_size; i++){
    if( old_array[i].key[0] != '\0' ){
      hashtab_insert( ht, old_array[i].key, old_array[i].value );
    }
  }
  free( old_array);
  return HT_SUCCESS;
} 

static uint32_t oat_hash( const char *key){
  uint32_t hkey = 0x00000000;
  int i;
  
  for( i=0; key[i] != '\0' && i<KEYSIZE; i++){
    hkey += (uint32_t) key[i];
    hkey += ( hkey << 10 );
    hkey ^= ( hkey >> 6 );
  }
  hkey += ( hkey << 3 );
  hkey ^= ( hkey >> 11 );
  hkey += ( hkey << 15 );

  return hkey;
}
