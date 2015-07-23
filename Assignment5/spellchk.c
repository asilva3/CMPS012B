// $Id: spellchk.c,v 1.6 2013-05-21 19:58:24-07 - - $

#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

#include "debugf.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

void print_error (char *object, char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", Exec_Name, object, message);
   fflush (NULL);
   Exit_Status = NUMBER_DICTS;
}

FILE *open_infile (char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (char *filename, hashset *hashset) {
   yylineno = 1;

   for (;;) {
      int token = yylex ();
      if (token == 0) break;
//If the upper case is not found check the lower case
      if(!has_hashset(hashset, yytext)){
         for(unsigned int i=0 ; i<strlen(yytext); ++i){
            yytext[i] = tolower(yytext[i]); 
      }
//If lower case not found then failure
      if(!has_hashset(hashset, yytext)){
         printf("%s: %s\n", filename, yytext);
         Exit_Status = EXIT_FAILURE;
         }
      }
   }
}
void load_dictionary (char *dictionary_name, hashset *hashset) {
   //Can't find dictionary, return 
   if (dictionary_name == NULL) return;
   
   //reads in the dictionary name
   FILE *file = fopen(dictionary_name, "r");
   if(file == NULL) print_error("Filename", "File not found");
   char buff[1024];
   for(;;){
//buffer takes in the file
      char *buf = fgets(buff, sizeof buff, file);
      if (buf == NULL) break;
      buf = strchr (buff, '\n');
      if (buf != NULL) *buf = '\0';
      buf = strdup(buff);
      assert(buf != NULL);
      put_hashset(hashset, buf);
   }
   fclose(file);
}


int main (int argc, char **argv) {
   Exec_Name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
//Use boolean for options, set to false
   bool optx = false;
   bool optxx = false;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;

   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': user_dictionary = optarg;
                   break;
         case 'n': default_dictionary = NULL;
                   break;
         case 'x': if(optx) optxx = true;
                   optx = true;
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debugflags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

//If optx is true then call the x_dump method to print the list
   if(optx) x_dump (hashset, optxx);

   // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   free_hashset(hashset); 
   yylex_destroy ();
   return Exit_Status;
}

