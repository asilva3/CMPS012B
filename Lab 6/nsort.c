// $Id: nsort.c,v 1.1 2011-04-29 19:46:42-07 - - $
// Armando Silva, asilva3
#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

   int exit_status= EXIT_SUCCESS;
   bool d = false;
   typedef struct node node;
   struct node {
      double value;
      node *link;
   };
   
   int main (int argc, char **argv) {
      //Check if the stdin is -d 
      if(argv[1]!=NULL){
         if(strcmp(argv[1], "-d")== 0){
            d=true;
            exit_status= EXIT_FAILURE;
         }else printf("Not a valid option.");
      }
      (void) argc;
      char *progname = basename (argv[0]);
      char err_buffer[82];
      char err_buf_fmt[16];
      sprintf (err_buf_fmt, "%%%lds", sizeof err_buffer-1);
 
      node *head = NULL;
      for(;;) {
         double number;
         int rc = scanf ("%lg", &number);
         node *prev = NULL;
         node *curr = head;
         
        if(rc == EOF){
           break;
        }else if(rc ==1){
            //Find the insertion position
            while (curr!=NULL){
               if(curr->value > number) break;
               prev = curr;
               curr = curr->link;
            }
            //Do the insertion
            node *tmp = malloc (sizeof (struct node));
            assert (tmp != NULL);
            tmp->value = number;
            tmp->link = curr;
            if (prev == NULL) head = tmp;
            else prev->link = tmp;
         }else{
            int rc = scanf (err_buf_fmt, err_buffer);
            assert (rc  == 1);
            printf("%s: bad number  \"%s\"\n",progname, err_buffer);
            exit_status = EXIT_FAILURE;
         }
       }
       //Prints the final 
       while(head !=NULL){
         node* old = head;
         if (d) printf("[%p]:%24.15g\n", old, old->value);
         else printf("%24.15g\n",head->value);
         head = head->link;
         free(old);
       }
       return exit_status;
   }
