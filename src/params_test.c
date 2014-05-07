#include <stdlib.h>
#include <stdio.h>
#include "params.h"

int main( void){
  int i, status;
  FILE *file;
  pdata pd; 
  char filename[] = "params_testfile_00.txt";
  int jj, jarray[10];
  double dd, darray[5];
  char str[80], str0[80], str1[80], str2[80], **strarray;
  strarray = (char **) malloc( 3*sizeof(char*));
  strarray[0] = str0;
  strarray[1] = str1;
  strarray[2] = str2;

  for( i=0; i<6; i++){
    pd = pdata_alloc( 256);
    printf( "Testing param_testfile_%02d.txt\n", i);
    sprintf( filename, "params_testfile_%02d.txt", i);
    file = fopen( filename, "r");
    status = pdata_read_file( file, pd);
    fclose( file);
    if( status == PDATA_FORMAT ){
      printf( "Success: File format problem!\n");
    }
    pdata_free( pd);
  }

  pd = pdata_alloc( 200);
  printf( "Testing param_testfile_%02d.txt\n", 6);
  sprintf( filename, "params_testfile_%02d.txt", 6);
  file = fopen( filename, "r");
  status = pdata_read_file( file, pd);
  fclose( file);
  if( status == PDATA_SUCCESS ){
    printf( "Success: We successfully read a properly formatted file!\n");
  }

  status = pdata_get_var_i( pd, "first", &jj);
  if( status == PDATA_SUCCESS){
    printf( "Success: Integer data \"first\" = %d\n", jj);
  }
  status = pdata_get_var_i( pd, "firstish", &jj);
  if( status == PDATA_FAILURE){
    printf( "Success: Integer data \"firstish\" is not present \n");
  }
  status = pdata_get_var_d( pd, "second", &dd);
  if( status == PDATA_SUCCESS){
    printf( "Success: Flout data \"second\" = %1.4f\n", dd);
  }
  status = pdata_get_var_d( pd, "secondish", &dd);
  if( status == PDATA_FAILURE){
    printf( "Success: Float data \"secondish\" is not present \n");
  }
  status = pdata_get_var_s( pd, "third", str);
  if( status == PDATA_SUCCESS){
    printf( "Success: String data \"third\" = %s\n", str);
  }
  status = pdata_get_var_s( pd, "thirdish", str);
  if( status == PDATA_FAILURE){
    printf( "Success: String data \"thirdish\" is not present \n");
  }

  status = pdata_get_array_i( pd, "intarray", 10, jarray);
  if( status == PDATA_SUCCESS){
    printf( "Success: Read integer array \"intarray\" \n");
    for( i=0; i<10; i++){
      printf( "%d ", jarray[i]);
    }
    printf( "\n");
  }
  status = pdata_get_array_i( pd, "nope", 10, jarray);
  if( status > 0){
    printf( "Success: Integer array \"nope\" \n");
    printf( "         failed on element %d \n", status);
    printf( "         Read and filled: \n");
    for( i=0; i<10; i++){
      printf( "%d ", jarray[i]);
    }
    printf( "\n");
  }
  status = pdata_get_array_i( pd, "intarray", 15, jarray);
  if( status > 0){
    printf( "Success: Integer array \"intarray\" \n");
    printf( "         failed on element %d \n", status);
    printf( "         Read and filled: \n");
    for( i=0; i<10; i++){
      printf( "%d ", jarray[i]);
    }
    printf( "\n");
  }
  status = pdata_get_array_d( pd, "floatarray", 5, darray);
  if( status == PDATA_SUCCESS){
    printf( "Success: Read double array \"floatarray\" \n");
    for( i=0; i<5; i++){
      printf( "%1.4f ", darray[i]);
    }
    printf( "\n");
  }
  status = pdata_get_array_s( pd, "stringarray", 3, strarray);
  if( status == PDATA_SUCCESS){
    printf( "Success: Read string array \"stringarray\" \n");
    for( i=0; i<3; i++){
      printf( "%s ", strarray[i]);
    }
    printf( "\n");
  }

  status = pdata_get_varlist( pd, "mydata", 3, 
			      STRING_T, str, 
			      DOUBLE_T, &dd, 
			      INT_T, &jj);
  if( status == PDATA_SUCCESS){
    printf( "Success: Read data structure \"mydata\" \n");
    printf( "name: %s, age: %1.2f, # of children: %d\n", str, dd, jj);
  }

  

  free( strarray);
  pdata_free( pd);
  return 0;
}
