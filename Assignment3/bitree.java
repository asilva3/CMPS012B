// $Id: bitree.java,v 1.3 2013-10-17 18:34:13-07 - - $
//
// NAME
//    class bitree - starter class for bitree implementation.
//

class bitree {
   char symbol;
   bitree left;
   bitree right;

   bitree (char symbol_, bitree left_, bitree right_) {
      symbol = symbol_;
      left = left_;
      right = right_;
   }

   bitree (char symbol_) {
      this (symbol_, null, null);
   }
   

   //Specifies which expression is right or left
   public String toString () {
      String read = "";
      if(right == null & left == null) return "" + symbol;
      read += "(" + left.toString();
      read += symbol;
      read += right.toString() + ")";
      return read;
   }
 
   //Reads a case, adds the evaluted expressions and stores
   public double eval(symbol_table table){
      double amount=0;   
      switch (symbol){
         case '+':
            amount += left.eval(table) + right.eval(table);
            break;
         case '-': 
            amount += left.eval(table) - right.eval(table);
            break;
         case '*':
            amount += left.eval(table) * right.eval(table);
            break;
         case '/': 
            amount += left.eval(table) / right.eval(table);
            break;
         default: 
            return table.get_value(symbol); 
        }return amount;
      }
  }
