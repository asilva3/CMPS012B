// $Id: bigint.c,v 1.12 2013-05-16 15:14:31-07 - - $
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

static void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}

bigint *new_string_bigint (char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   return this;
}

static bigint *do_add (bigint *this, bigint *that) {
   bigint *tmp;
   int result = 0;
   int carry = 0;
   
   //Set Capacity of Size
   if(this->size > MIN_CAPACITY || that->size > MIN_CAPACITY){
      tmp= new_bigint((this->size > that-> size 
                      ? this-> size: that->size)+1);
   }else {
      tmp= new_bigint(MIN_CAPACITY);
   }
   tmp->size = (this->size > that->size ? this->size :
                                    that->size) + 1;
   tmp->negative= this->negative;
   //Adding method 
   for(size_t i=0; i<tmp->size-1;++i){
      if(i >= that->size) result= this->digits[i] + carry;
      else if(i>= this->size) result = that->digits[i] + carry;
      else{
      result = this->digits[i] + that->digits[i] + carry;
      }
      tmp->digits[i]= result%10; 
      carry= result/10;
      
   }
   return tmp;
}

static bigint *do_sub (bigint *this, bigint *that) {
   bigint *tmp;
   int result = 0;   
   int borrow= 0;

   //Set Capacity of Size 
   if(this->size > MIN_CAPACITY || that->size > MIN_CAPACITY){
      tmp = new_bigint((this->size > that->size)
                        ? this->size: that->size);
   }
   else { tmp= new_bigint(MIN_CAPACITY); }
   
   tmp->size = (this->size > that->size ? this->size :
                                       that->size);
   tmp->negative= this->negative; 
   //Subtracting method
   for(size_t i=0; i<tmp->size;++i){
      if(i >= that->size) result = this->digits[i]- borrow +10;
      else if(i >= this->size) result = that->digits[i]- borrow +10;
      else{
      result = this->digits[i] - that->digits[i] - borrow +10;
      }
      tmp->digits[i]= result %10; 
      borrow = 1 - result/10;
       
  }return tmp;
}

void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

void print_bigint (bigint *this, FILE *file) {
   int count = 0;
   (void)file;
   trim_zeros (this);
   if(this->negative == true)
     printf("-");
   for(int i= this->size-1; i >= 0; --i) {
      printf ("%d", this->digits[i]);
      count ++;
      if(count == 69){
         printf("\\\n");
         count = 0;
      }
   }
   printf("\n");
}

bigint *add_bigint (bigint *this, bigint *that) {
   if((this->negative && !that->negative) ||
      (!this->negative && that->negative)){
      if(this->size > that->size)   
         return do_sub(this, that);
      else if(this->size < that->size)
         return do_sub(that, this);
      else if(this->size == that->size){
         for(size_t i= this->size-1; i >= 0; --i){
            if(this->digits[i] < that->digits[i])
               return do_sub(that, this);
            if(this->digits[i] > that->digits[i])
               return do_sub(this, that);
            }
         }
   }
   else return do_add(this, that);
}

bigint *sub_bigint (bigint *this, bigint *that) {
  //Set cases to see if That is Greater than This 
   if((this->negative && !that->negative) || 
      (!this->negative && that->negative)){
      if(this->size > that->size)
         return do_add(this, that);
      else if(this->size < that->size)
         return do_add(that, this);
      else if(this->size == that->size){
         for(size_t i= this->size -1; i >= 0; --i){
            if(this->digits[i] < that->digits[i]){
               return do_add(that, this);
            }if(this->digits[i] > that->digits[i]){
               return do_add(this, that);
            }
         }
      }
   }else if((this->negative && that->negative) || 
            (!this->negative && !that->negative)){
      if(that->size > this->size){
         that->negative = true;
         return do_sub(that, this);
      }else if(this->size > that->size)
         return do_sub(this, that);
      else if(this->size == that->size){
         for(int i= this->size-1; i >=0; --i){
             if(this->digits[i] < that->digits[i]) {
                return do_sub(that, this);
         }else if(this->digits[i] > that->digits[i]){
            return do_sub(this, that);
           } 
          }
         return do_sub(this,that);
      }
   else return do_sub(this, that);
   }
 }


bigint *mul_bigint (bigint *this, bigint *that) {
   bigint *result;
   int carry= 0;
   int rem=0;
   size_t size = this->size + that->size;

   if(size > MIN_CAPACITY)result = new_bigint(size);
                     else result = new_bigint(MIN_CAPACITY);
   result->size= size;

   for(size_t i= 0; i < this->size; ++i){
      carry= 0;
      for(size_t j=0; j < that->size; ++j){
         rem= result->digits[i+j] + this->digits[i] *
            that->digits[j] + carry;
         result->digits[i+j] = rem %10;
         carry = rem/10;
      }result->digits[i + that->size]= carry;
   }
   return result;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

