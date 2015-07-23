//Armando Silva, asilva3

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#include "inssort.h"

int MAX_LINE = 4000;

int strstrcmp(const void *this, const void *that){
   char **thisstr = (char**)this;
   char **thatstr = (char**) that;
   return strcmp (*thisstr, *thatstr);
}

int main(int argc, char **argv){
   (void) argc;
   (void) argv;
   int count = 0;
   char **buffer;
   char line[MAX_LINE];
   buffer = (char**) malloc(sizeof(char*) *1000);

   //reads in the characters
   while(fgets(line, MAX_LINE, stdin)){
      if(feof(stdin))break; //if end of file break
      buffer[count]= strdup(line); //allocates mem 
      ++count;
      if(count == 1000)break; //max size
   }

   inssort(buffer, count, sizeof(buffer[0]), strstrcmp);
//print and free each index
   for(int i = 0; i<count; ++i){
      printf("%s", buffer[i]);
     free(buffer[i]);
   }
   free(buffer);
}

