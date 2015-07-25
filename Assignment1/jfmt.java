// $Id: jfmt.java,v 1.349 2013-10-13 23:00:38-07 - - $
// 
// Kevin Thai, kjthai
// Armando Silva, asilva3
//
//
// Starter code for the jfmt utility.  This program is similar
// to the example code jcat.java, which iterates over all of its
// input files, except that this program shows how to use
// String.split to extract non-whitespace sequences of characters
// from each line.
//

import java.io.*;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import static java.lang.System.*;

class jfmt {
   // Static variables keeping the general status of the program.
   public static final String JAR_NAME = get_jarname();
   public static final int EXIT_SUCCESS = 0;
   public static final int EXIT_FAILURE = 1;
   public static int exit_status = EXIT_SUCCESS;
   public static int width = 65;

   // A basename is the final component of a pathname.
   // If a java program is run from a jar, the classpath is the
   // pathname of the jar.
   static String get_jarname() {
      String jarpath = getProperty ("java.class.path");
      int lastslash = jarpath.lastIndexOf ('/');
      if (lastslash < 0) return jarpath;
      return jarpath.substring (lastslash + 1);
   }


   // Formats a single file.
   static void format (Scanner infile) {
      // Read each line from the opened file, one after the other.
      // Stop the loop at end of file.
      List<String> words = new LinkedList<String>();
      boolean repeat = false;
      for (int linenr = 1; infile.hasNextLine(); ++linenr) {
         String line = infile.nextLine();
         
         // Split the line into words around white space and iterate
         // over the words.
         for (String word: line.split ("\\s+")) {

            // Skip an empty word if such is found.
            if (word.length() == 0)  continue;
                 // Append the word to the end of the linked list.
                 words.add (word);
          }
            // This is for the line between paragraphs
            // The boolean is for any multiple line that may occur      
         if (repeat == false && line.isEmpty()){
              print_paragraph(words);
              out.print("\n"+"\n" );
              words.clear();
              repeat = true;
          }
         if (repeat == true && line.isEmpty() == false){
              repeat = false;
          }
      }
     // Runs and inputs LinkedList words into print_paragraph
     print_paragraph(words); 
     out.println();
   }   
   
   // This functions takes what is inputed and prints it out  
   static void print_paragraph(List<String> words){
         // Keeps track of how many character are counted
         int char_count = 0;
         // Stores the input into the Iterator of Strings
         Iterator<String> row = words.iterator();
         for (words.iterator();row.hasNext();){
            String row_word = row.next();
            // Sets the limit of the string to width; currently 65
            // Prints out the input
            if ( char_count + row_word.length() > width){
                 out.print("\n" + row_word+ " ");
                 char_count =0+row_word.length()+1;
              }
            else{
                 char_count += row_word.length()+1;
                 out.print(row_word+ " ");
             } 
           }  
       }


   // Main function scans arguments and opens/closes files.
   public static void main (String[] args) {
      if (args.length == 0) {
         // There are no filenames given on the command line.
         format (new Scanner (in));
      }  

         // Iterate over each filename given on the command line.
      for (int argix = 0; argix < args.length; ++argix) {
             String filename = args[argix];
         // Sets the width to what the user inputed.
         if (argix == 0 && args[0].matches("-\\d+")){
             width= Integer.parseInt(args[0]) * -1;
             if (args.length == 1) {
                format (new Scanner (in));
             }
             continue;
          }
           // Prints an error message for an invalid input.
          else if (argix == 0 && args[0].matches("-.+")){
             err.printf("Invalid Option: %s %n", args[0]);
             exit_status= EXIT_FAILURE;
             continue;
          }
          if (filename.equals ("-")) {
               // Treat a filename of "-" to mean System.in.
             format (new Scanner (in));
          }
          else {
               // Open the file and read it, or error out.
          try  {

              Scanner infile = new Scanner (new File (filename));
              format (infile);
              infile.close();
              }
          catch (IOException error) {
                 exit_status = EXIT_FAILURE;
                 err.printf ("%s: %s%n", JAR_NAME,
                              error.getMessage());
                                      }
               }
          }
   exit (exit_status);
       }
}
