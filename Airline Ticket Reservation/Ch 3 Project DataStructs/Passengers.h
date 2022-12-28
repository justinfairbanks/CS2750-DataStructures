#pragma once
using namespace std;
#include <string> //For string data type


class Passengers //Doubly Linked List
{
	public: 
		string passInfo; //Node Data

		Passengers* next = NULL; //Pointer to the next node in Linked List
		
		Passengers* prev = NULL; //Pointer to the previous node for Doubly Linked List
};
