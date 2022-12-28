												/*	Justin Fairbanks - Airline Ticket Reservation Program 	*/

											   /* Add, Remove, Sort, & Output Passengers from Flight Linked Lists */
											 /* Driver Function Shows Inserting of Passengers into Designated Nodes */
											  /* Then Removing of Passenger and Inserting into New Flight (Node) */
//Included Libraries	
#include <iostream> 
#include <algorithm> //For transform() function
#include <iomanip> //For setw() function
#include "Passengers.h" //Includes Passengers Class

using namespace std;

//PROTOTYPES

  /* Manual Input Functions */
	void reservations();
	string addPass(string); //Gets user input for passenger to add
	int remPass(Passengers* node); //List of Passengers with numbers next to them to select which passenger to remove

  /* Driver Function */
	void driver(); 

  /* Output / Formatting */
	void printList(Passengers*);
	void reverse(Passengers*,int);


  /* Node Managment Functions */
 
	/* Passengers.cpp File  */
	void push(Passengers**, string); //Insert Node at top of list (for flight numbers only)
	void append(Passengers**, string); //Inserting Node after existing nodes
	void deleteN(Passengers**, string); //Deleting Node
	void deletePass(Passengers**, int); //For Deleting Passenger given their node position

	/* Sorts existing lists */
	void MergeSort(Passengers**); //Function called to sort list (following 2 functions are called from this one)
		Passengers* SortedMerge(Passengers* a, Passengers* b);
		void FrontBackSplit(Passengers* source, Passengers** frontRef, Passengers** backRef);


/* MAIN MENU (used only in first iteration) */
int main() {
//LOCAL VARS
	int choice = 0;
	char repeat = 'n';

	cout << setw(80) << "==== Justin Fairbank's Program ====" << endl << endl;

//MAIN MENU

	do {
		repeat = 'n';

		cout << "  === MAIN MENU ===\n";
		cout << "1 - Make a Reservation\n";
		cout << "2 - For Driver\n";
		cout << "0 - Exit Program\n";

		cin >> choice;

		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter 0-3:\n";
			cin >> choice;
		}

		switch (choice) {

		case 0:	repeat = 'n';
				break;


		case 1: reservations(); // Make or Change a Reservation
				repeat = 'n';
				break;

		case 2: driver(); // Driver Function
				repeat = 'y';
				break;


		default: repeat = 'y'; //Sets choice to 5 to re ask for user input in do while loop
			cout << "\nInput not recognized, please enter # 0-3\n";
			break;
		}

	} while (repeat == 'y'); //If user input was not recognized in switch statement repeats loop



//CLOSING PROGRAM
	cin.clear();
	cin.ignore();
	cout << "\n\nPress Enter to Close...";
	cin.get();
	return 0;
}


/* ADD || REMOVE || DISPLAY PASSENGERS */
void reservations()
{
// LOCAL VARS
	int input = 0, choice = 1;

	char repeat;

	string tempFL, currFlight;

	int tempD; //Int for position of deleted passenger

	int a = 0, b = 0; // a For flight number array position 
				  // b Counter so that flight number is only included in node on first iteration

	/* Array for temporary storing of flight numbers */
	string flightNum[5];

	/* Array of Pointers to Flight Nodes*/
	const int maxFlights = 5;
	Passengers* fl[maxFlights]{};


	int c = 0; //For while loop so main menu isn't ran first iteration 


/* MAIN MENU */
		do {
			if (c == 1)
			{
				repeat = 'n'; //So loop doesn't repeat unless requested

				cout << "\t=== MAIN MENU ===\n";
				cout << "1 - Make or Change a Reservation\n";
				cout << "2 - Print all Manifests\n";
				cout << "3 - For Driver\n";
				cout << "0 - Exit Program\n";

				cin >> choice;

				while (cin.fail()) //Prevents menu breaking if string entered
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please enter 0-3:\n";
					cin >> choice;
				}

				switch (choice) {

	/* EXIT PROGRAM */
				case 0:
					return; //Closes Program


	/* MAKE OR CHANGE RESERVATION */

				case 1: 
					break; break; //Breaks out of loops to 'Flight Reservation Menu'


	/* PRINT ALL MANIFESTS */

				case 2:

					for (int i = 0; i < maxFlights; i++) {
						MergeSort(&fl[i]); /* Sorts All the Flights */
						printList(fl[i]); /* Outputs All the Flights */
					}

				break;

	/* DRIVER FUNCTION */

				case 3: driver(); //Driver Function
					return;
					break;


				default: repeat = 'y'; //Sets repeat to y to re ask for user input in do while loop
					cout << "\nInput not recognized, please enter # 0-3\n";
					cin.clear(); //Clears unrecognized input
					break; break;
				}
			}

/* ENTER FLIGHT NUMBER */
			int iFlNum = 0; //Integer for flight number (after validation converted to string)
			c = 1;

			if (choice == 1) {
				cout << "\nPlease Enter a Flight Number:\n";
				cin >> iFlNum;
				
				while (cin.fail()) //Tests if number was entered
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please enter a valid number:\n";
					cin >> iFlNum;
				}

				tempFL = to_string(iFlNum); //Converts int flight number to string for storing 

				for (int i = 0; i < 5; i++) {			//seeing if flight number already created
					if (flightNum[i] == tempFL) {
						a = i;							//records the position where they are the same
						b = 1;						    //So that flight number is not inserted into node again
					}
					else {
						flightNum[a] = tempFL; //If unique flight number adds to a position
						b = 0;				  //Sets b to 0 so that flight number is inserted into node
					}
				}
				currFlight = flightNum[a]; //For passing
				a++;
			}

/* FLIGHT RESERVATION MENU */

				do {
					if (choice == 2)
						break;
					cout << "\n=== List of Passengers ===\n";
					cout << "1 - Insert passenger onto flight " << currFlight << "." << endl;
					cout << "2 - Remove passenger from flight " << currFlight << "." << endl;
					cout << "3 - List all passengers on flight " << currFlight << "." << endl;
					cout << "4 - List passengers by reverse order." << endl;
					cout << "0 - Exit Flight" << endl;

					cin >> input;
					
					while (cin.fail()) //Prevents menu breaking if string entered
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Please enter 0-4:\n";
						cin >> input;
					}

				/* LOCAL VAR */
					string tempPass;


					/* INPUT || REMOVE || PRINT LIST of current flight */
					switch (input) {

	/* IF FLIGHT EXITED */

					case 0:
						input = 6;
						repeat = 'y'; //If flight is exited goes back to first do while loop to enter a new flight number
						break;

	/* ADDING PASSENGERS TO FLIGHT */

					case 1:

						/* ADD PASSENGERS FUNCTION */
						tempPass = addPass(currFlight);					   //Returns the passenger name
						if (b == 0) { push(&fl[a], currFlight); b = 1; }  //Flight Number at the top of the linked list 
						append(&fl[a], tempPass);						  //Sets b to 1 so flight num not added over and over
																		  //Add Passengers Following Flight Number

						repeat = 2; //Goes back to menu for flight
						b = 1; //So we know that flight number has been added to flight

						break;

	/* REMOVE PASSENGERS FROM FLIGHT SELECTED */

					case 2:

						MergeSort(&fl[a]);			//Sorts before showing list of passengers to delete
						tempD = remPass(fl[a]);		//Function returns position of passenger to delete
						deletePass(&fl[a], tempD);	//Deletes Specified Position (Node) of Passenger
						printList(fl[a]);			//Prints out updated passenger list

						break;


	/* PRINTS CURRENT FLIGHT (IN ALPHABETICAL ORDER) */


					case 3:

						MergeSort(&fl[a]);	//Sorts current list before output
						printList(fl[a]);	//Prints out Passenger List of Selected Flights

						break;


	/* PRINT CURRENT FLIGHT (IN REVERSE ORDER) */

					case 4:

						MergeSort(&fl[a]);  //Sorts the list alphabetically
						reverse(fl[a], 0);	//Prints out Passenger List in Reverse Order
											//0 so it only shows header of flight first iteration as the function repeatedly calls itslef

						break;
					}

				} while (input != 6); //Repeats if input not recognized


/* BACK TO MAIN MENU ? */

			cout << "\n\nBack to Main Menu? (y/n)\n"; //if anything other than 'y' or 'Y' entered the program closes
			cin >> repeat;

			repeat = tolower(repeat); //Makes all responses lower case

			if (repeat != 'y') //if anything but 'y' closes program
				repeat = 'n';

		} while (repeat != 'n');
}


/* Gets user input for passenger to add to flight */
string addPass(string flightNum)
{
//LOCAL VARS
	string name;
	string first;
	string last;

	Passengers* temp = nullptr;

//Insert Passengers into Flight

	cout << "\n\n***Inserting Passenger onto Flight " << flightNum << "***\n\n";


	/* User Input for Passenger to be added */

		cout << "\nPassenger first and last name (space separated): ";
		cin >> first >> last;

		name = last + ", " + first; //Forms into one string to add to array, then linked list

		transform(name.begin(), name.end(), name.begin(), tolower); //Makes all names all lower case so sorts the same


/* INPUTTING ADDED PASSENGERS TO LINKED LIST */

	push(&temp, flightNum); //Flight Number at the top of the linked list
	append(&temp, name);	//Passengers Following



cout << "Passenger " << first << " " << last << " successfully inserted into flight " << flightNum << ".\n\n";

return name;
}

/* Prints list of passengers with ID numbers for removing */
int remPass(Passengers* node)
{
	//VARS
	string name;
	string flightNum;
	int passID = 1;

	Passengers* last = nullptr; //Pointer for displaying passenger list

	if (node == NULL) //If list is empty
		cout << "LIST EMPTY";

	flightNum = node->passInfo;
	cout << "\n\n***Removing Passenger from Flight " << flightNum << "***\n\n";

	//DISPLAY PASSENGERS ON FLIGHT WITH ID NUMBERS


	last = node;
	node = node->next;
	while (node != NULL) {
		cout << passID << "  " << node->passInfo << endl;
		passID++;
		last = node;
		node = node->next;
	}

	cout << endl << endl;



	cout << "\nPlease enter the number to the left of the passenger that you would like to remove\n";
	cin >> passID;

	while (cin.fail()) //Prevents menu breaking if string entered
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter number next t	o passenger:\n";
		cin >> passID;
	}

	return passID;

}


/* DRIVER FUNCTION ACCORDING TO CANVAS */
void driver()
{
	cout << endl << endl;
	cout << setw(75) << "=== DRIVER FUNCTION ===\n\n\n";


//ALLOCATING NODES

	Passengers* Flight2430 = NULL; //Flight 2430
	Passengers* Flight2515 = NULL; //Flight 2515
	Passengers* Flight2750 = NULL; //Flight 2750

	/* Flight Numbers later pushed to top of each linked list */

	string fl1 = "2430";
	string fl2 = "2515";
	string fl3 = "2750";

//STORING PASSENGERS IN NODES


/* FLIGHT 2430 */

	push(&Flight2430, fl1); //Adding flight number as node

	append(&Flight2430, "Hamilton, Dale"); //Append inserts data sequentially rather than pushing the names to the top every time
		cout << "Dale Hamilton was inserted into flight " << fl1 << endl;

	append(&Flight2430, "Hamilton, Leslie");
		cout << "Leslie Hamilton was inserted into flight " << fl1 << endl;

	append(&Flight2430, "Hamilton, Jonathan");
		cout << "Jonathan Hamilton was inserted into flight " << fl1 << endl;

	append(&Flight2430, "Hamilton, Nicholas");
		cout << "Nicholas Hamilton was inserted into flight " << fl1 << endl;
	
	append(&Flight2430, "Hamilton, Annalisa");
		cout << "Annalisa Hamilton was inserted into flight " << fl1 << endl;


   /* SORT LIST FUNCTION */

	MergeSort(&Flight2430); //Sorts existing linked list


  /* OUTPUTS FLIGHT */
	cout << endl;
	printList(Flight2430);
	cout << endl;



/* FLIGHT 2515 */

		push(&Flight2515, fl2); //Adding flight number as node

								//Add passengers following flight number
		append(&Flight2515, "Absorka, Thor");
			cout << "Absorka, Thor was inserted into flight " << fl1 << endl;
		
		append(&Flight2515, "Snowwisper, Nora");
			cout << "Snowwisper, Nora was inserted into flight " << fl1 << endl;

		append(&Flight2515, "Loki, the Mutt");
			cout << "Loki, the Mutt was inserted into flight " << fl1 << endl;

	/* SORT LIST FUNCTION */

		MergeSort(&Flight2515); //Sorts existing linked list


	/* OUTPUT FLIGHT */
		cout << endl;
		printList(Flight2515);
		cout << endl;


	/* FLIGHT 2750 */

		push(&Flight2750, fl3); //Adding flight number as node

		//No Passengers in Flight 2750 at this point


/* MOVING LOKI OFF FLIGHT */

	/* Temp local var for Loki */
string tempPass = "Loki, the Mutt"; //Assuming this was inputted from the user to remove Loki from the flight

//REMOVE LOKI FROM FLIGHT
	deleteN(&Flight2515, tempPass); //Loki's current flight and then Loki's name which will be removed from this flight
	cout << tempPass << " was successfully saved from running in an Iditerod" << endl;


//LOKI ADDED TO NEW FLIGHT
	cout << tempPass << " was inserted into flight " << fl3 << endl << endl;
	append(&Flight2750, tempPass);


	printList(Flight2430);
	cout << endl;

	printList(Flight2515);
	cout << endl;

	printList(Flight2750);
	cout << endl;
}

/* ALPHABETICAL ORDER */
void printList(Passengers* node)
{
	Passengers* last = nullptr;	

	if (node == NULL) //If list is empty
		return;

  /* Read the top of each linked list which is the flight number */

	cout << "\n=== Flight Number: " << node->passInfo << " ===" << endl << endl;



  /* OUTPUTTING NODES IN ALPHABETICAL ORDER */

	last = node;
	node = node->next;
	while (node != NULL) {
		cout << " " << node->passInfo << endl;
		last = node;
		node = node->next;
	}

	cout << endl << endl;
}

/* REVERSE ORDER */
void reverse(Passengers* node, int a) 
{
	if (a == 0) {
		cout << "\n=== Flight Number: " << node->passInfo << " ===" << endl;
		cout << "  (in reverse order)" << endl << endl;
	}
//FOR REVERSING ORDER OF NODES 

	if (node == NULL)	//Once the last node is read then the loop stops
		return;

//print the list in reverse
		reverse(node->next,1);

		string t = node->passInfo;
		int s = t.length();

//After everything else is printed, print head
	if (s > 6) // If number is detected will not output number following reverse list (this is assuming flight number under 6 digits)
		cout << node->passInfo << endl;
}