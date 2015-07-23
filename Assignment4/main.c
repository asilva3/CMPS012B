// $Id: main.c,v 1.8 2013-05-08 22:09:41-07 - - $
#include <assert.h>
#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bigint.h"
#include "debug.h"
#include "stack.h"
#include "token.h"

void do_push (stack *stack, char *numstr) {
   bigint *bigint = new_string_bigint (numstr);
   push_stack (stack, bigint);
}

void do_binop (stack *stack, bigint_binop binop) {
   int size = size_stack(stack);
   if(! empty_stack(stack) && size !=1){
      bigint *right = pop_stack (stack);
      bigint *left = pop_stack (stack);
      bigint *answer = binop (left, right);
      push_stack (stack, answer);
      free_bigint (left);
      free_bigint (right);
   }else fprintf(stderr, "dc: stack empty\n");
}

void do_clear (stack *stack) {
   while (! empty_stack (stack)) {
      bigint *bigint = pop_stack (stack);
      free_bigint (bigint);
   }
}

void do_print (stack *stack) {
   int size = size_stack (stack);
   if (size == 0) {
      fprintf(stderr, "stack is empty\n");
      return;
   }
   print_bigint (peek_stack (stack, size-1), stdout);
}

void do_print_all (stack *stack) {
   int size = size_stack (stack);
   if(size == 0){
      fprintf(stderr, "stack is empty\n");
      return;
   }
   for (int index = size-1; index >= 0; --index) {
      print_bigint (peek_stack (stack, index), stdout);
   }
}

void unimplemented (int oper) {
   printf ("%s: ", program_name);
   if (isgraph (oper)) printf ("'%c' (0%o)", oper, oper);
                  else printf ("0%o", oper);
   printf (" unimplemented\n");
}

void scan_options (int argc, char **argv) {
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@': set_debug_flags (optarg);
                   break;
         default : printf ("%s: -%c: invalid option\n",
                           program_name, optopt);
                   break;
      }
   }
}

int main (int argc, char **argv) {
   program_name = basename (argv[0]);
   scan_options (argc, argv);
   stack *stack = new_stack ();
   token *scanner = new_token (stdin);
   for (;;) {
      int token = scan_token (scanner);
      if (token == EOF) break;
      switch (token) {
         case NUMBER: do_push (stack, peek_token (scanner)); break;
         case '+': do_binop (stack, add_bigint); break;
         case '-': do_binop (stack, sub_bigint); break;
         case '*': do_binop (stack, mul_bigint); break;
         case 'c': do_clear (stack); break;
         case 'f': do_print_all (stack); break;
         case 'p': do_print (stack); break;
         default: unimplemented (token); break;
      }
   }
   while(!empty_stack(stack)){
     bigint *old = pop_stack(stack);
      free_bigint(old);
   }
   free_token(scanner);
   free_stack(stack);
   DEBUGF ('m', "EXIT %d\n", exit_status);
   return EXIT_SUCCESS;
}
