//Armando Silva , asilva3

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <math.h>

#include "inssort.h"

int exit_status = EXIT_SUCCESS;
int size = 1000;

//compare to function
int compar (const void *this, const void *that){
   double *thisdoub = (double*) this;
   double *thatdoub = (double*) that;
   //decides if its less than one greater than 0 or more than 1
   return *thisdoub < *thatdoub ? -1 : *thatdoub ? +1 :0;
}

int main (int argc, char **argv){
   double array[size];
   int count =0;

//insertion
   for(size_t i= 0; i< size; ++i){
      double number;
      int rc = scanf("%lg", &number);
      if(rc == EOF) break;
      else if(rc == 1){
      //store the number in the index
         array[i] = number;
         ++count;
      }else{
         exit_status = EXIT_FAILURE;
         break;
      }
   }
   inssort(array, count, sizeof(double), compar);
   for(int i = 0; i<count ; ++i){
     printf("%20.15g\n", array[i]);
   }
   return EXIT_SUCCESS;
}

