#include "Stack.h"
#include <iostream>
#include <cstdlib>
#include <exception>

using namespace std;


/* Pushes the given data onto the Stack by creating new node and making it the head. */

void Stack::push(char data) 
{

	Node* newnode = new Node();

	newnode->data = data;

	newnode->next = head;

	head = newnode;
}




/* Pops data from stack (deletes the head node) */

char Stack::pop() {

	char returnData = head->data;

	Node* deleteNode = head; //Set data of node to deletedNode Pointer

	head = head->next; //Assign next element to the head of the stack

	delete deleteNode; //Deletes the pointer storing Node's Data

	return returnData; //Returns the deleted node
}


/* For looking at the head of the stack */

char Stack::peek()
{
	return head->data;
}


/* Bool for whether stack is empty or not */

bool Stack::isEmpty() 
{
	if (head == NULL) return true;

	else return false;
}