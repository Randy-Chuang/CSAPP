/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    /* q stores a non-NULL address, we can visit the memory block and initialize the structure */
    if(q){
      q->head = q->tail = NULL;
      q->size = 0;
    }
    /* If malloc return a NULL pointer, then we would skip the initialization and return NULL (q) */
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if(!q)
      return;
    list_ele_t *temp = q->head, *curr = NULL;
    while(temp){
      curr = temp;
      temp = temp->next;
      if(curr->value)
        free(curr->value);
      free(curr);
    }
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if(!q)
      return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if(newh){
      newh->value = malloc(sizeof(char) * (strlen(s) + 1));
      if(newh->value){
        strncpy(newh->value, s, strlen(s)+1);
        if(++(q->size) == 1){
          q->tail = newh;
        }
        newh->next = q->head;
        q->head = newh;
        return true;
      }
      /* Fail to allocate memory block for storing string */
      free(newh);
    }
    return false;    
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* What should you do if the q is NULL? */
    if(!q)
      return false;
    
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if(newt){
      newt->value = malloc(sizeof(char) * (strlen(s) + 1));
      if(newt->value){
        strncpy(newt->value, s, strlen(s)+1);
        newt->next = NULL;
        if(++(q->size) == 1){
          q->head = newt;
        }
        q->tail->next = newt;
        q->tail = newt;
        return true;
      }
      /* Fail to allocate memory block for storing string */
      free(newt);
    }
    return false; 
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    /* What should you do if the q is NULL? */
    if(!q)
      return false;
    list_ele_t *curr = q->head;  
    if(curr){
      if(bufsize > 0){
        strncpy(sp, curr->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
      }
      q->head = q->head->next;
      free(curr->value);
      free(curr);
      if(--(q->size) == 0){
        q->tail = NULL;
      }
      return true;
    }
    return false;    
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    return q ? q->size : 0;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
    if(!q || q->size < 2)
      return;

    q->tail = q->head;    
    list_ele_t *next = q->head->next, *prev = NULL;
    while(next){
      q->head->next = prev;
      prev = q->head;
      q->head = next;
      next = next->next;
    }
    q->head->next = prev;
}

