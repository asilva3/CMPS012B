// $Id: listmap.java,v 1.36 2013-10-27 17:10:33-07 - - $


//Armando Silva, asilva3

import java.util.Iterator;
import java.util.Map.Entry;
import java.util.NoSuchElementException;
import static java.lang.System.*;

class listmap implements Iterable<Entry<String,intqueue>> {

   private class node implements Entry<String,intqueue> {
      String key;
      intqueue queue = new intqueue();
      node link;
      public String getKey() {
         return key;
      }
      public intqueue getValue() {
         return queue;
      }
      public intqueue setValue (intqueue queue) {
         throw new UnsupportedOperationException();
      }
   }
   private node head = null;

   public listmap() {
      // Not needed, since head defaults to null anyway.
   }

   public void insert (String key, int linenr) {   
      node prev = null;
      int cmp = 1;
      node curr = head;
    //Check where to insert node if node isn't equal to null     
         while(curr != null){
            cmp = curr.key.compareTo(key);
        //if node is the same word as another break the loop
            if (cmp >=0) break;
            prev = curr;
            curr = curr.link;
         }
          if(cmp !=0){
             node temp= new node();
             temp.key= key;
             temp.link = curr;
             if(prev == null){
                 head = temp;
                 head.queue.insert(linenr);
                 }
             else {prev.link = temp;
                  temp.queue.insert(linenr);
             } 
          }
         //Case for if the node is the same word as another.
          else if(cmp ==0){
              curr.queue.insert(linenr);
      }
   }

   public Iterator<Entry<String,intqueue>> iterator() {
      return new iterator();
   }


   private class iterator
           implements Iterator<Entry<String,intqueue>> {
      node curr = head;

      public boolean hasNext() {
         return curr != null;
      }

      public Entry<String,intqueue> next() {
         if (curr == null) throw new NoSuchElementException();
         node next = curr;
         curr = curr.link;
         return next;
      }

      public void remove() {
         throw new UnsupportedOperationException();
      }

   }

}
