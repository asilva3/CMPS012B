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
      char *String ;
      node *link;
   };
   
   int main (int argc, char **argv) {
      //Check if the stdin is -d 
      if(argv[1]!=NULL){
      if(strcmp(argv[1], "-d")== 0){
         d=true;
      }else printf("Invalid option");}
      (void) argc;
      char *progname = basename (argv[0]);
      char buffer[82];
      int linenr;
      node *head = NULL;
      
      for(linenr=1; ;++linenr) {
         char *gotline = fgets (buffer, sizeof buffer, stdin);
       //If character of line reaches more than 80 print err 
         if (gotline == NULL) break;
         if(strlen(gotline)>80){
            printf("Line Overflow:\n");
            exit_status= EXIT_FAILURE;
         }
         char *nlpos = strchr (buffer, '\n');
         node *prev= NULL;
         node *curr = head;
         
        if(nlpos !=NULL){
            *nlpos= '\0';
        }else{
            fprintf (stderr, "%s: %d: unterminated line: %s\n",
                     progname, linenr, buffer);
            exit_status = EXIT_FAILURE;
         }
            //Find the insertion position
            while (curr!=NULL){
               if(strcmp(curr->String, buffer) > 0) break;
               prev = curr;
               curr = curr->link;
            }
            //Do the insertion
            node *tmp = malloc (sizeof (struct node));
            assert (tmp != NULL);
            tmp->String = strdup (buffer);
            tmp->link = curr;
            if(prev == NULL) head = tmp;
            else prev->link= tmp;
      }
 
       //Prints the final 
       while(head !=NULL){
         node* old = head;
         if (d) printf("[%p]:%s\n", old, old->String);
         else printf("%s\n",head->String);
         head = head->link;
        free(old->String);
        free(old);
       }
       return exit_status;
   }
