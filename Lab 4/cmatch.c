#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<stdbool.h>

char *program_name = NULL;
int exit_status = EXIT_SUCCESS;
#define STDIN_NAME "-"

typedef struct options {
   bool ignore_case;
   bool filenames_only;
   bool number_lines;
} options;

void catbyline (FILE *input, char *filename, char *key, options *opts) {
   char buffer[1024];
   printf ("FILE %s:\n", filename);
//reads in the array of characters
   for (int linenr = 1;; ++linenr) {
//inserts the character into a character buf
      char *buf = fgets (buffer, sizeof buffer, input);
//compares the character to the buffer
      char *match= strstr(buffer, key);
//case sensitive
      char *cmatch = strcasestr(buffer, key);
      if(buf==NULL) break;
//cases for the options    
       if(opts->ignore_case==true) {
         if(cmatch != NULL) printf("%s", buf);
      }else if(opts->filenames_only==true) { 
         if(cmatch != NULL)  printf("%s\n",filename);
      }else if(opts->number_lines==true) {
         if(cmatch != NULL) printf("%d: %s", linenr, buf);
      }else if(opts->ignore_case== true 
               && opts->filenames_only==true) {
         if(cmatch != NULL) printf("%s\n", filename);
      }else if(opts->number_lines==true 
               && opts->filenames_only==true) {
         if(cmatch != NULL) printf("%s\n", filename);
      }else if(opts->ignore_case==true 
               && opts->number_lines==true) {
         if(cmatch != NULL) printf("%d: %s", linenr, buf);
      }else {
         if(match!=NULL) printf ("%s", buf);
      }      
   }
}

void scan_options (int argc, char **argv, options *opts) {
   opts->ignore_case = false;
   opts->filenames_only = false;
   opts->number_lines = false;
   opterr = false;
   for (;;) {
      int opt = getopt (argc, argv, "ilm");
      if (opt == EOF) break;
      switch (opt) {
         case 'i':
            opts->ignore_case = true;
            break;
         case 'l':
            opts->filenames_only = true;
            break;
         case 'm':
            opts->number_lines = true;
            break;
         default:
            exit_status = EXIT_FAILURE;
            fflush (NULL);
            fprintf (stderr, "%s: -%c: invalid option\n",
                             program_name, optopt);
            break;
      }
   }
}

const char *strbool (bool value) {
   return value ? "true" : "false";
}

int main (int argc, char **argv) {
   options opts;
   scan_options(argc, argv, &opts);
   char *key = argv[1];
   bool files= false;
//Set boolean true if there are more than just one file
   if(optind+2<argc)  files=true;
   program_name = basename (argv[0]);
   if (argc == 1) {
      catbyline (stdin, STDIN_NAME, key, &opts);
   }else {
      for (int argi = 2; argi < argc; ++argi) {
         char *filename = argv[argi];
         if (strcmp (filename, STDIN_NAME) == 0) {
            catbyline (stdin, STDIN_NAME, key, &opts);
         }else {
            FILE *input = fopen (filename, "r");   
                if (input != NULL) {
                catbyline (input, filename, key, &opts);
                fclose (input);
            }else {
               exit_status = EXIT_FAILURE;
               fflush (NULL);
               fprintf (stderr, "%s: %s: %s\n", program_name,
                        filename, strerror (errno));
               fflush (NULL);
            }
         }
      }
   }
   return exit_status;
}

