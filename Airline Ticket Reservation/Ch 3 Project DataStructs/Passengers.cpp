#include "Passengers.h"
#include <iostream>
using namespace std;

/* Prototypes for Merge Sort Functions */
	Passengers* SortedMerge(Passengers*, Passengers*);
	void FrontBackSplit(Passengers*, Passengers**, Passengers**); 


/* Push / Append functions adapted from https ://www.geeksforgeeks.org/doubly-linked-list/?ref=lbp
												Contributed by shivanisinghss2110						*/

/* For adding to the top */
void push(Passengers** head_ref, string new_passenger)
{
	Passengers* person = new Passengers();	//Allocating of Node


	person->passInfo = new_passenger; //Add data to node


	person->next = (*head_ref); //Make next new node as head and previous as NULL
	person->prev = nullptr;


	if ((*head_ref) != nullptr) //Change prev of head node to new node
		(*head_ref)->prev = person;

	(*head_ref) = person; //Move the head to point to the new node

}



/* For adding after the existing nodes */
void append(Passengers** head_ref, string new_passenger)
{
	//ALLOCATE NODES
	Passengers* newNode = new Passengers();

	Passengers* last = *head_ref;


	newNode->passInfo = new_passenger; //Add to data


	newNode->next = NULL; //Last node so null


	if (*head_ref == NULL) { //If list is empty make new head
		newNode->prev = NULL;
		*head_ref = newNode;
		return;
	}


	while (last->next != NULL) //Else go to last node
		last = last->next;


	last->next = newNode; //Change the next of the last node


	newNode->prev = last; //Make last node as previous of new node

}




/* For Removing Specified Passenger */
/* deleteN Adapted from https://www.geeksforgeeks.org/cpp-program-for-deleting-a-node-in-a-linked-list/ */

/* Deletes Passenger Based on Exact String Stored in Node */
void deleteN(Passengers** head_ref, string pass)
{

	// Store head node
	Passengers* temp = *head_ref;
	Passengers* previous = nullptr;

	// If head node itself holds the pass to be deleted
	if (temp != NULL && temp->passInfo == pass)
	{
		*head_ref = temp->next; // Changed head
		delete temp;            // free old head
		return;
	}

	// Else Search for the pass to be deleted, keep track of the previous node as we need to change 'prev->next' */
	else
	{
		while (temp != NULL && temp->passInfo != pass)
		{
			previous = temp;
			temp = temp->next;
		}

		// If key was not present in linked list
		if (temp == NULL)
			return;

		// Unlink the node from linked list
		previous->next = temp->next;

		//Free memory
		delete temp; //null not needed bc local in this function
	}
}


/* Deletes Passenger Based On Position of Node */
void deletePass(Passengers** head_ref, int position)
{
	position += 1;
	//If linked list is empty
	if (*head_ref == NULL)
		return;

	//Store head node
	Passengers* temp = *head_ref;

//If head needs to be removed
	if (position == 0) {

	//Change head
		*head_ref = temp->next;

	//Delete old head
		delete temp;
		return;
	}

//Find previous node of the node to be deleted
	for (int i = 0; temp != NULL && i < position - 2; i++)
		temp = temp->next;

//If position is more than number of nodes
	if (temp == NULL || temp->next == NULL)
		return;

//Node temp->next is the node to be deleted
//Store pointer to the next of node to be deleted
	Passengers* next = temp->next->next;

//Unlink the node from linked list
	delete temp->next; // Free memory

//Unlink the deleted node from list
	temp->next = next;
}





/* Sort Functions of Existing Linked Lists */
/* Following Functions Adapted from https://www.geeksforgeeks.org/merge-sort-for-linked-list/ */

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(Passengers** headRef)
{
	Passengers* head = *headRef;
	Passengers* a;
	Passengers* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	FrontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	MergeSort(&a);
	MergeSort(&b);

	/* answer = merge the two sorted lists together */
	*headRef = SortedMerge(a, b);
}

Passengers* SortedMerge(Passengers* a, Passengers* b)
{
	Passengers* result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->passInfo <= b->passInfo) {
		result = a;
		result->next = SortedMerge(a->next, b);
	}
	else {
		result = b;
		result->next = SortedMerge(a, b->next);
	}
	return (result);
}

/* Uses the fast/slow pointer strategy. */
void FrontBackSplit(Passengers* source, Passengers** frontRef, Passengers** backRef)
{
	Passengers* fast;
	Passengers* slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}