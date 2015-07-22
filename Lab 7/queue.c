// $Id: queue.c,v 1.26 2013-11-20 16:59:12-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"

#define STUBPRINTF(...) fprintf (stderr, __VA_ARGS__);

typedef struct queue_node queue_node;
struct queue_node {
   queue_item_t item;
   queue_node *link;
};

struct queue {
   queue_node *front;
   queue_node *rear;
};

queue *new_queue (void) {
   queue *new_queue = malloc (sizeof (struct queue));
   return new_queue;
}

void free_queue (queue *this) {
   assert (isempty_queue (this));
   free (this);
}

void insert_queue (queue *this, queue_item_t item) { 
   queue_node *temp =  malloc (sizeof (struct queue_node));
   temp->item = item;
   temp->link = NULL;
   if(isempty_queue(this)) this->front = temp;
                      else this->rear->link= temp;
   this->rear= temp;
}

queue_item_t remove_queue (queue *this) {
   queue_item_t *temp = this->front->item;
   queue_node *prev = this->front;
   assert (! isempty_queue (this));
   this->front= this->front->link;
   free (prev);
   return temp;
}

bool isempty_queue (queue *this) {
   return this->front == NULL;
}

