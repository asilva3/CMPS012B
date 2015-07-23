// $Id: hashset.c,v 1.7 2013-05-23 15:41:07-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "debugf.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   for(size_t i = 0; i < this->size ; ++i)
      this->chains[i] = NULL;
   return this;
}

void free_hashset (hashset *this) {

   //Go through each index and free the hash table
   for(int i=0; i < this -> size; ++i){
   while(this->chains[i]!=NULL){
      hashnode *old = this->chains[i];
      this->chains[i] = this->chains[i]->link; //free the array
      free(old->word); //free the word
      free(old); //free the array
   }
   }
   free(this->chains);
   free(this);
}

// Method for x-option
void x_dump(hashset *this, bool optxx){
   printf("%7zu words \n", this->load);
   printf("%7zu size of the hash array\n", this->size);
   
   int *chainsize = calloc(this->size, sizeof(int));
   int count =0;
//Runs through the size and counting the size of the items in each index
   for(unsigned int i=0; i<this->size; ++i){
      hashnode *curr = this->chains[i];
      count = 0;
      while(curr!= NULL){
         ++count;
         curr = curr ->link;
      }
      ++chainsize[count];
   }
//Prints each item in the index
   for(size_t pos = 1; pos< this->size; ++pos){
      if(chainsize[pos] !=0){
         printf("%7d Size of Chain %2zu\n", chainsize[pos], pos);
      }
   }

   if(optxx){
      for(size_t pos = 0; pos<this->size; ++pos){
         hashnode *curr = this->chains[pos];
         if(curr!= NULL){
            printf("array[%10zu] = %12zu\"%s\"\n", pos,
                     strhash(curr->word), curr->word);
         }
         while(curr!=NULL){
            curr = curr->link;
            if(curr != NULL){
               printf("     = %12zu\"%s\"\n", strhash(curr->word),
                        curr->word);
            }
         }
      }
   }free(chainsize);
}

void put_hashset (hashset *this, char *item) {
   size_t hash = strhash(item) % this->size;
   hashnode *curr = this->chains[hash];
   while (curr != 0) {
      if (strcmp (curr->word, item) == 0) return;
      curr = curr->link;
   }
//creates a temp to the array
   hashnode *temp = malloc(sizeof(hashnode));
   temp->word = item;
   temp->link = this->chains[hash];
   this->chains[hash] = temp;
//Increase the number of items put in the hash
   ++this->load;

   if(this->load *2 > this->size){
      //Create a new array double the size
      size_t oldsize = this->size;
      this->size = this->size *2 +1;
      size_t sizchains = this->size * sizeof (hashnode *);
      hashnode **new = malloc (sizchains);
      memset(new, 0, sizchains);
       
      //Run through checking where to allocate the words to the new array
      for(int i = 0; i < oldsize; ++i){
//while there are still words in the index keep pointing to the new array
         while(this->chains[i] != 0){
            hashnode *old = this->chains[i]; //old node points to the new 
            //points the old array to the new array link
            this->chains[i] = this->chains[i]->link; 
            int hashed = strhash(old->word)% this->size;
            old->link = new[hashed];
            new[hashed] = old;
         }
   }
   free(this->chains);
   this->chains = new;
   }
}

bool has_hashset (hashset *this, char *item) {
   if(item == NULL) return false;
   size_t chain = strhash(item) % this->size;
   for(hashnode *curr= this->chains[chain]; curr != NULL; curr= curr->link){
      if(strcmp(curr-> word, item) == 0) return true;
   }return false;
}

