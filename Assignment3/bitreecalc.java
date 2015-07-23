// $Id: bitreecalc.java,v 1.1 2013-10-16 12:58:29-07 - - $
import java.io.*;
import java.util.Scanner;
import java.util.NoSuchElementException;
import static java.lang.System.*;

class bitreecalc {
   private static int exit_status= 0;
   private static final String STDIN_FILENAME = "-";

//For each filename, open the file, cross reference it, 
//and print.
   private static void bitree_filename (String filename, symbol_table table,
                                            linked_stack stack) {
      if (filename.equals (STDIN_FILENAME)) {
         bitree_file (filename, new Scanner (System.in), table, stack);
      }else {
          try { 
             Scanner file = new Scanner (new File (filename));
             bitree_file(filename, file, table, stack);
             file.close();
          }catch (IOException error) {
           System.err.println("Error, not read from Scanner"); 
          }
      }
    }
 
    private static void colon(String line, symbol_table table){
       String numb = line.substring(0,2);
       String sub= line.substring(2);
       try{
          //reads after the colon as a double stores into a table
          char var = line.charAt(0);
          double number = Double.parseDouble(sub);
          table.put(var, number, table.get_tree(var));
          System.out.printf("%s  %g\n",numb,number);
          System.out.printf("%s\n", table.get_tree(var));
       }catch(NoSuchElementException error){
           err.printf("%s is not a number%n", sub);
           exit(exit_status);
       }
    }

    private static void equals(String line, symbol_table table, 
                                   linked_stack stack){
       String place= line.substring(2);
       for(int i=0; i<=place.length()-1; ++i)
          { 
            //checks if letter
            if((place.charAt(i)>='A' && place.charAt(i)<='Z') ||
               (place.charAt(i)>='a' && place.charAt(i)<='z')){
               bitree tree = new bitree(place.charAt(i));
               stack.push(tree); 
            }
            //checks assignment operator
            else if(place.charAt(i)=='+' || place.charAt(i)=='-'
                  || place.charAt(i)=='*' || place.charAt(i)=='/'){  
               bitree right = (bitree)stack.pop();
               bitree left = (bitree)stack.pop();
               if(right==null || left==null){
               System.out.print("STACK UNDERFLOOOOOOOOW!");
               exit_status=1;
               } 
               bitree tree = new bitree(place.charAt(i), left, right);
               stack.push(tree);
             }else err.println("Assigned it the wrong value man!");
          }
         //pop final tree and eval
         bitree tree = (bitree)stack.pop();
         double answer = tree.eval(table);
         System.out.printf("%c= ", line.charAt(0));
         System.out.printf("%g\n", answer);
         System.out.println(tree);
         table.put(line.charAt(0), answer, tree);
      }	
 
    private static void qmark(String line, symbol_table table){
      //if var is empty get the orgianl value stored
      if(table.get_tree(line.charAt(0))==null){
         double empty= table.get_value(line.charAt(0));
         System.out.printf("%c= %g\n", line.charAt(0), empty);
      }else{
         double amount= (table.get_tree(line.charAt(0)).eval(table));  
         System.out.printf("%c= %g\n", line.charAt(0), amount);
         System.out.printf(" ", table.get_tree(line.charAt(0)));
         table.put(line.charAt(0), amount, table.get_tree(line.charAt(0)));
      }
    }
   

    private static void bitree_file (String filename,
                  Scanner file, symbol_table table, linked_stack stack){
       String line = ""; 
       //reads in the file
       while(file.hasNextLine()){
           line = file.nextLine();
           line = line.replace(" ", "");
           //ignore white space and #'s
           if (line.isEmpty()) continue;
           if (line.startsWith("#")) continue;
           char var = line.charAt(0);
           char oper = line.charAt(1);
           //If first thing is a letter then go into operation
           if(Character.isLetter(var)){
              if(oper == ':' ){
                 colon(line, table);
              }else if(oper == '='){
                    equals(line, table,stack);
              }else if (oper == '?'){
                  qmark(line, table);
              }else {
                 err.println("-Invalid Operator");
              }
           }else System.err.print("Variable not assigned");
        }
    } 
    
 
    public static void main (String[] args){
       symbol_table table= new symbol_table();
       linked_stack<bitree> stack = new linked_stack<bitree>();
       if (args.length == 0) {
           bitree_filename (STDIN_FILENAME, table, stack);
       }else {
           for (String filename: args){
             //Print final table
              bitree_filename (filename, table, stack);
              System.out.print("****FINAL SYMBOL TABLE****");
                 table.dump();
          }
       }
        exit (exit_status);
    }
}


