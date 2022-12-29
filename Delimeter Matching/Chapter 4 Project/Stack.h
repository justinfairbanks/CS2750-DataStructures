#pragma once



#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>

using namespace std;


class Node {

public:
	char data; //Data of Node
	Node* next; //Next pointer for node
};



/* This class implements the Stack itself */


class Stack 
{

private:
	Node* head; //Top of Stack


public:

/* Constructor */
	Stack() {
		head = NULL;
	}

/* Functions of Stack */
	void push(char);
	char pop();
	char peek();
	bool isEmpty();
};
