#include <stdlib.h>
#include <stdio.h>

#include "params.h"

int main( void){
  int i, status;
  FILE *file;
  char filename[] = "params_testfile_00.txt";
  int jj, jarray[10];
  double dd, darray[5];
  char str[80], str0[80], str1[80], str2[80], **strarray;
  pdata pd;
  strarray = (char **) malloc( 3*sizeof(char*));
  strarray[0] = str0;
  strarray[1] = str1;
  strarray[2] = str2;

  for( i=0; i<6; i++){
    pdata_initialize( &pd);
    printf( "Testing param_testfile_%02d.txt\n", i);
    sprintf( filename, "params_testfile_%02d.txt", i);
    file = fopen( filename, "r");
    status = pdata_read_file( &pd, file);
    fclose( file);
    if( status == PDATA_FORMAT ){
      printf( "Success: File format problem!\n");
    }
  }

  pdata_initialize( &pd);
  printf( "Testing param_testfile_%02d.txt\n", 6);
  sprintf( filename, "params_testfile_%02d.txt", 6);
  file = fopen( filename, "r");
  status = pdata_read_file( &pd, file);
  fclose( file);
  if( status == PDATA_SUCCESS ){
    printf( "Success: We successfully read a properly formatted file!\n");
  }

  status = pdata_get_var_i( &pd, "first", &jj);
  if( status == PDATA_SUCCESS){
    printf( "Success: Integer data \"first\" = %d\n", jj);
  }
  status = pdata_get_var_i( &pd, "firstish", &jj);
  if( status == PDATA_FAILURE){
    printf( "Success: Integer data \"firstish\" is not present \n");
  }
  status = pdata_get_var_d( &pd, "second", &dd);
  if( status == PDATA_SUCCESS){
    printf( "Success: Flout data \"second\" = %1.4f\n", dd);
  }
  status = pdata_get_var_d( &pd, "secondish", &dd);
  if( status == PDATA_FAILURE){
    printf( "Success: Float data \"secondish\" is not present \n");
  }
  status = pdata_get_var_s( &pd, "third", str);
  if( status == PDATA_SUCCESS){
    printf( "Success: String data \"third\" = %s\n", str);
  }
  status = pdata_get_var_s( &pd, "thirdish", str);
  if( status == PDATA_FAILURE){
    printf( "Success: String data \"thirdish\" is not present \n");
  }

  status = pdata_get_array_i( &pd, "nope", jarray);
  if( status == 0){
    printf( "Success: Read no element for integer array \"nope\" \n");
  }
  status = pdata_get_array_i( &pd, "intarray", jarray);
  if( status == 10 ){
    printf( "Success: Integer array \"intarray\" \n");
    printf( "         read %d elements\n", status);
    printf( "         ");
    for( i=0; i<10; i++){
      printf( "%d ", jarray[i]);
    }
    printf( "\n");
  }
  status = pdata_get_array_d( &pd, "floatarray", darray);
  if( status == 5 ){
    printf( "Success: Read double array \"floatarray\" \n");
    printf( "         read %d elements\n", status);
    printf( "         ");
    for( i=0; i<5; i++){
      printf( "%1.4f ", darray[i]);
    }
    printf( "\n");
  }
  status = pdata_get_array_s( &pd, "stringarray", strarray);
  if( status == 3 ){
    printf( "Success: Read string array \"stringarray\" \n");
    printf( "         read %d elements\n", status);
    printf( "         ");
    for( i=0; i<3; i++){
      printf( "%s ", strarray[i]);
    }
    printf( "\n");
  }

  status = pdata_get_varlist( &pd, "mydata", 3, 
			      STRING_T, str, 
			      DOUBLE_T, &dd, 
			      INT_T, &jj);
  if( status == PDATA_SUCCESS){
    printf( "Success: Read data structure \"mydata\" \n");
    printf( "name: %s, age: %1.2f, # of children: %d\n", str, dd, jj);
  }

  free( strarray);
  return 0;
}
