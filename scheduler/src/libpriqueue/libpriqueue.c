/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.

  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
	q->comp = comparer;
	q->size = 0;
	q->head = NULL;
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue,
	where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
	node* node = malloc(sizeof(node));
	node->next = NULL;
	node->process = ptr;
	if(q->size == 0)
	{
		q->head = node;
		q->size++;
		return 0;
	}
	// what special about size 1 que??
	else if (q->size == 1)
	{
		if(q->comp(q->head, node) < 0)
		{
			node->next = q->head;
			q->head = node;
			q->size++;
			return 0;
		}
		else
		{
			q->head->next = node;
			q->size++;
			return 1;
		}
	}//end queue of size 1 case
	else
	{
		current = q->head;
		previous = current;
		int tracker = 0;
		while(tracker < q->size)
		{
			if(q->comp(current, node) < 0)
			{
				previous->next = node;
				node->next = current;
				q->size++;
				return tracker;
			}
			else
			{
				previous = current;
				current = current->next;
			}
		}
		//need to be checked
		return tracker;
	}
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
	if(q->size == 0)
	{
		return NULL;
	}
	else
	{
		return q->head;
	}
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
	if(q->size == 0)
	{
		return NULL;
	}
	else
	{
		node* returnNode = q->head;
		q->head = q->head->next;
		q->size--;
		return returnNode;
		// this node might need to be freed, to solve it I would create an accessor
		// function to get the value and free it.
	}
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
	node* temp = q->head;
	int tracker = 0;
	if(q->size < index)
	{
		return NULL;
	}
	else
	{
		// this will iterate the whole que, another condition is needed
		while(tracker < index && index != tracker)
		{
			temp = temp->next;
			tracker++;
		}
		if (index == tracker)
			return temp;
		else
			return NULL;
	}
}


/**
  Removes all instances of ptr from the queue.

  This function should not use the comparer function,
	but check if the data contained in each element of the queue is equal (==) to ptr.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
	if (q->size == 0)
		return 0;
	else{
		current = q->head;
		previous = current;
		int tracker = 0;
		int initialSize = q->size;
		while(tracker < initialSize)
		{
			if(current->next == NULL)
			{
				return 0;
			}
			if(current->process == ptr)
			{
				previous = current -> next;
				q->size--;
				free(current);
				current = previous -> next;
			}
			tracker++;
			previous = current;
			current = current -> next;
		}//end while
		return 0;
	}
}//end funtion


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
	//I do not really think it is a good way to do it, remember how to delete elem in linkedlist
	current = q->head;
	previous = current;
	int tracker = 0;
	while(tracker < index)
	{
		previous = current;
		current = current -> next;
	}
	previous = current -> next;
	free(current);
	q->size--;
}


/**
  Returns the number of elements in the queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
	return q->size;
}


/**
  Destroys and frees all the memory associated with q.

  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
	current = q->head;
	//previous = q->head;
	while(q->size > 0)
	{
		temp = current->next;
		free(current);
		current = temp;
		q->size--;
	}
	q->head = NULL;
}
