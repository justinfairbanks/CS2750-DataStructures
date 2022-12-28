/*Justin Fairbanks - Track and Field Scoring Program
Last updated: 9.21.2022																						*/

//Libraries
#include <iostream> 
#include <string> //For string manipulation
#include <iomanip> //For setw() function
#include "eventParticipation.h" //Includes Track & Field Class
using namespace std;


// Prototypes (Called Functions)
	void track(); //Track Athletes
	void field();//Field Athletes
	void driver(); //Driver/Sorts/Outputs (No user input needed)
	


int main() {

	char choice; //For user input
	int verify = 0; //Validates if 't' or 'f' entered

	do {
		cout << "Please enter 't' for Track,'f' for Field, or 'd' for Driver\n";
		cin >> choice;
		choice = toupper(choice);
		verify = 0;

		switch (choice) {

		case 'T': track();
			break;

		case 'F': field();
			break;

		case 'D': driver();
			break;

		default: cout << "\nNot a valid character, try again...\n";
			verify = 1; //Sets while statement to true to repeat user input
			break;
		}
	} while (verify != 0); //Repeats only if default ^ was called




//Closing of Program
	cin.clear();
	cin.ignore();
	cout << "\n\nPress Enter to Close...";
	cin.get();
	return 0;
}

void track() {

//Temp Variables
	string tempFirst, tempLast, tempTeam, tempSex;
	int tempAge = 0, n = 0;
	int const max = 100;
	double tempTime = 0.0;

//Object Defining
	eventParticipation athlete[max]; //Included an array off 100 as an object of eventParticipation class
									//100 is max number of athletes that can be inputted
	trackParticipation tAthlete[max];	//Object for track athletes

	cout << "\nHow many Track athletes would you like to add?\n";
	cin >> n; //For for loop


//User Input

	for (int i = 0; i < n; i++) {

		cout << "\nNAME of Athlete " << 1+i << " (First Last)\n";
		cin >> tempFirst >> tempLast;

		cout << "\nEnter " << tempFirst << "'s TEAM" << endl;
		cin >> tempTeam;

		cout << "\nEnter " << tempFirst << "'s SEX (m/f)" << endl;
		cin >> tempSex;

		cout << "\nEnter " << tempFirst << "'s AGE" << endl;
		cin >> tempAge;

		cout << "\nEnter the TIME Mark (in seconds) for " << tempFirst << " " << tempLast << endl;
		cin >> tempTime;

//Storing temp variables to class

		(athlete + i)->setFirst(tempFirst); //First Name
		(athlete + i)->setLast(tempLast);	//Last Name

		(athlete + i)->setTeam(tempTeam);	//Team Name

		(athlete + i)->setSex(tempSex);		//Sex of Athlete

		(athlete + i)->setAge(tempAge);		//Age of Athlete
	
		(tAthlete + i)->setTime(tempTime);   //Storing tempTime to derived class

	}



	/********************************************************************************************************************/


//SORTING

	string filter; //User input for how they want to filter
	int val = 0;

	cout << "\nHow would you like to sort all of the data you entered for the track event?\n";
	cout << "Enter 'name' to filter by name or 'time' to filter fastest by slowest\n";
 
	do { //runs once and validates if correct input was entered
		cin >> filter;
		val = 0;


//FILTERING BEST TIME

		if (filter == "time" || filter == "Time") { //Filtering by Best Time to Slowest Time
			cout << "Filtered by Time:\n\n";

			int repeat = 0; //For determining if all has been sorted correctly

			while (repeat == 0) {
				repeat = 1;
				for (int i = 0; i < n; i++) {
					if ((tAthlete + i)->getTime() > tAthlete[i + 1].getTime()) {

						//Store in temporary variables
						tempFirst = (athlete + i)->getFirst();
						tempLast = (athlete + i)->getLast();
						tempTeam = (athlete + i)->getTeam();
						tempSex = (athlete + i)->getSex();
						tempAge = (athlete + i)->getAge();
						tempTime = (tAthlete + i)->getTime();

						//Swap next object for previous
						(athlete + i)->setFirst(athlete[i + 1].getFirst());
						(athlete + i)->setLast(athlete[i + 1].getLast());
						(athlete + i)->setTeam(athlete[i + 1].getTeam());
						(athlete + i)->setSex(athlete[i + 1].getSex());
						(athlete + i)->setAge(athlete[i + 1].getAge());
						(tAthlete + i)->setTime(tAthlete[i + 1].getTime());

						//Finishing the Swap
						athlete[i + 1].setFirst(tempFirst);
						athlete[i + 1].setLast(tempLast);
						athlete[i + 1].setTeam(tempTeam);
						athlete[i + 1].setSex(tempSex);
						athlete[i + 1].setAge(tempAge);
						tAthlete[i + 1].setTime(tempTime);

						//Set to 0 as long as there are athletes to compare
						repeat = 0;
					}
				}
			}

//Outputting
			int TAB = 10;

	//Header of List
			cout << setw(20) << "Name" << setw(15) << "Team" << setw(12) << "Gender" << setw(9) << "Age" << setw(17)
				<< "Time(s)" << setw(TAB) << "Place" << endl;
			cout << "\t_____________________________________________________________________________\n";

			string fullName; //Full name for formatting
			n += 1;
		//Athlete List
			for (int i = 1; i < n; i++) {

				fullName = (athlete + i)->getLast() + ", " + (athlete + i)->getFirst(); //Adding first and last name together 
		//Output
				cout << setw(25) << fullName << setw(10) << (athlete + i)->getTeam();
				cout << setw(TAB) << (athlete + i)->getSex() << setw(TAB) << (athlete + i)->getAge() << setw(15) << (tAthlete + i)->getTime() << setw(TAB) << i << endl;
			}

		//Points Awarding 
			cout << "\n\n"; //Extra spacing
			for (int i = 1; i < n; i++) {

				int points = 0;
				switch (i) { //Switch to add points based on the placing of the athletes

				case 1: points = 10;
					break;
				case 2: points = 8;
					break;
				case 3:	points = 6;
					break;
				case 4: points = 4;
					break;
				case 5: points = 2;
					break;
				case 6: points = 2;
					break;
				default: points = 0;
					break;

				}
				cout << (athlete + i)->getFirst() << " " << (athlete + i)->getLast() << " from " << (athlete + i)->getTeam() << " gets " << points << " points!" << endl;
			}


		}


		/********************************************************************************************************************/

//ALPHABETICAL ORDER BY LAST NAME


		else if (filter == "name" || filter == "Name") { //Filtering by Athletes Name in Alphabetical Order
			cout << "Alphabetical Order:\n\n";


			int repeat = 0; //For determining if all has been sorted correctly

//SORTING
			while (repeat == 0) {
				repeat = 1;
				for (int i = 0; i < n; i++) {
					if ((athlete + i)->getLast() > athlete[i + 1].getLast()) {

						//Store in temporary variables
						tempFirst = (athlete + i)->getFirst();
						tempLast = (athlete + i)->getLast();
						tempTeam = (athlete + i)->getTeam();
						tempSex = (athlete + i)->getSex();
						tempAge = (athlete + i)->getAge();
						tempTime = (tAthlete + i)->getTime();

						//Swap next object for previous
						(athlete + i)->setFirst(athlete[i + 1].getFirst());
						(athlete + i)->setLast(athlete[i + 1].getLast());
						(athlete + i)->setTeam(athlete[i + 1].getTeam());
						(athlete + i)->setSex(athlete[i + 1].getSex());
						(athlete + i)->setAge(athlete[i + 1].getAge());
						(tAthlete + i)->setTime(tAthlete[i + 1].getTime());

						//Finishing the Swap
						athlete[i + 1].setFirst(tempFirst);
						athlete[i + 1].setLast(tempLast);
						athlete[i + 1].setTeam(tempTeam);
						athlete[i + 1].setSex(tempSex);
						athlete[i + 1].setAge(tempAge);
						tAthlete[i + 1].setTime(tempTime);

						//Set to 0 as long as there are athletes to compare
						repeat = 0;
					}
				}
			}

//Outputting
			int TAB = 10;

		//Header of List
			cout << setw(20) << "Name" << setw(15) << "Team" << setw(12) << "Gender" << setw(9) << "Age" << setw(17)
				<< "Time(s)" << setw(TAB) << "Place" << endl;
			cout << "\t_____________________________________________________________________________\n";

			string fullName; //Full name for formatting
			n += 1;


		//Athlete List
			for (int i = 1; i < n; i++) {

				fullName = (athlete + i)->getLast() + ", " + (athlete + i)->getFirst(); //Adding first and last name together 
				//Output
				cout << setw(25) << fullName << setw(10) << (athlete + i)->getTeam();
				cout << setw(TAB) << (athlete + i)->getSex() << setw(TAB) << (athlete + i)->getAge() << setw(15) << (tAthlete + i)->getTime() << setw(TAB) << i << endl;
			}


		//Points Awarding 

			cout << "\n\n"; //Extra spacing

			for (int i = 1; i < n; i++) {

				int points = 0;
				switch (i) { //Switch to add points based on the placing of the athletes

				case 1: points = 10;
					break;
				case 2: points = 8;
					break;
				case 3:	points = 6;
					break;
				case 4: points = 4;
					break;
				case 5: points = 2;
					break;
				case 6: points = 2;
					break;
				default: points = 0;
					break;

				}
				cout << (athlete + i)->getFirst() << " " << (athlete + i)->getLast() << " from " << (athlete + i)->getTeam() << " gets " << points << " points!" << endl;
			}


		}

//IF USER INPUT NOT RECOGNIZED
		else {
			cout << "\nPlease enter either 'name' or 'time'\n";  //If input not recognized from if statements above 
			val = 1;
		}
	} while (val == 1);


}

void field() {

//Temp Variables
	string tempFirst, tempLast, tempTeam, tempSex;
	int tempAge = 0, n = 0;
	int const max = 100;
	double tempDist = 0.0;

//Object Definitions
	eventParticipation athlete[max];	//Included an array off 100 as an object of eventParticipation class
										//100 is max number of athletes that can be inputted
	fieldParticipation fAthlete[max];	//Object for track athletes

	cout << "\nHow many Field athletes would you like to add?\n";
	cin >> n; //For for loop


//User Input

	for (int i = 0; i < n; i++) {

		cout << "\nNAME of Athlete " << 1 + i << " (First Last)\n";
		cin >> tempFirst >> tempLast;

		cout << "\nEnter " << tempFirst << "'s TEAM" << endl;
		cin >> tempTeam;

		cout << "\nEnter " << tempFirst << "'s SEX (m/f)" << endl;
		cin >> tempSex;

		cout << "\nEnter " << tempFirst << "'s AGE" << endl;
		cin >> tempAge;

		cout << "\nEnter the DISTANCE (in Meters) for " << tempFirst << " " << tempLast << endl;
		cin >> tempDist;

//Storing temp variables to class

		(athlete + i)->setFirst(tempFirst); //First Name
		(athlete + i)->setLast(tempLast);	//Last Name

		(athlete + i)->setTeam(tempTeam);	//Team Name

		(athlete + i)->setSex(tempSex);		//Sex of Athlete

		(athlete + i)->setAge(tempAge);		//Age of Athlete

		(fAthlete + i)->setDistance(tempDist);   //Storing tempTime to derived class

	}


	/********************************************************************************************************************/


//Filter method sent to sorting function

	string filter;
	int val = 0;

	cout << "\nHow would you like to sort all of the data you entered for the field event?\n";
	cout << "Enter 'name' to filter by name or 'distance' to filter the best to shortest\n";

	do { //runs once and validates if correct input was entered
		cin >> filter;
		val = 0;



//FILTERING BY BEST DISTANCE

		if (filter == "distance" || filter == "Distance") { //Filtering by the longest distance to the shortest
			cout << "\n\nSorting based on furthest distance to the shortest:\n\n";

//SORTING

			int repeat = 0; //For determining if all has been sorted correctly


			while (repeat == 0) {
				repeat = 1;
				for (int i = 0; i < n; i++) {
					if ((fAthlete + i)->getDistance() < fAthlete[i + 1].getDistance()) {

						//Store in temporary variables
						tempFirst = (athlete + i)->getFirst();
						tempLast = (athlete + i)->getLast();
						tempTeam = (athlete + i)->getTeam();
						tempSex = (athlete + i)->getSex();
						tempAge = (athlete + i)->getAge();
						tempDist = (fAthlete + i)->getDistance();
						

						//Swap next object for previous
						(athlete + i)->setFirst(athlete[i + 1].getFirst());
						(athlete + i)->setLast(athlete[i + 1].getLast());
						(athlete + i)->setTeam(athlete[i + 1].getTeam());
						(athlete + i)->setSex(athlete[i + 1].getSex());
						(athlete + i)->setAge(athlete[i + 1].getAge());
						(fAthlete + i)->setDistance(fAthlete[i + 1].getDistance());

						//Finishing the Swap
						athlete[i + 1].setFirst(tempFirst);
						athlete[i + 1].setLast(tempLast);
						athlete[i + 1].setTeam(tempTeam);
						athlete[i + 1].setSex(tempSex);
						athlete[i + 1].setAge(tempAge);
						fAthlete[i + 1].setDistance(tempDist);

						//Set to 0 as long as there are athletes to compare
						repeat = 0;
					}
				}
			}

//Outputting
			int TAB = 10;

		//Header of List
			cout << setw(20) << "Name" << setw(15) << "Team" << setw(12) << "Gender" << setw(9) << "Age" << setw(17)
				<< "Distance(m)" << setw(TAB) << "Place" << endl;
			cout << "\t_____________________________________________________________________________\n";

			string fullName; //Full name for formatting


		//Athlete List
			for (int i = 0; i < n; i++) {

				fullName = (athlete + i)->getLast() + ", " + (athlete + i)->getFirst(); //Adding first and last name together 
		//Output
				cout << setw(25) << fullName << setw(10) << (athlete + i)->getTeam();
				cout << setw(TAB) << (athlete + i)->getSex() << setw(TAB) << (athlete + i)->getAge() << setw(15) << (fAthlete + i)->getDistance() << setw(TAB) << i << endl;
			}


		//Points Awarding 

			cout << "\n\n"; //Extra spacing

			for (int i = 0; i < n; i++) {

				int points = 0;
				switch (i) { //Switch to add points based on the placing of the athletes

				case 0: points = 10;
					break;
				case 1: points = 8;
					break;
				case 2:	points = 6;
					break;
				case 3: points = 4;
					break;
				case 4: points = 2;
					break;
				case 5: points = 2;
					break;
				default: points = 0;
					break;

				}
				cout << (athlete + i)->getFirst() << " " << (athlete + i)->getLast() << " from " << (athlete + i)->getTeam() << " gets " << points << " points!" << endl;
			}
			
		}

		/********************************************************************************************************************/


//FILTERING BY LAST NAME

		if (filter == "name" || filter == "Name") {
			cout << "\n\nFiltering alphabetically by last name:\n\n";


//SORTING

			int repeat = 0; //For determining if all has been sorted correctly


			while (repeat == 0) {
				repeat = 1;
				for (int i = 0; i < n; i++) {
					if ((athlete + i)->getLast() > athlete[i + 1].getLast()) {

						//Store in temporary variables
						tempFirst = (athlete + i)->getFirst();
						tempLast = (athlete + i)->getLast();
						tempTeam = (athlete + i)->getTeam();
						tempSex = (athlete + i)->getSex();
						tempAge = (athlete + i)->getAge();
						tempDist = (fAthlete + i)->getDistance();

						//Swap next object for previous
						(athlete + i)->setFirst(athlete[i + 1].getFirst());
						(athlete + i)->setLast(athlete[i + 1].getLast());
						(athlete + i)->setTeam(athlete[i + 1].getTeam());
						(athlete + i)->setSex(athlete[i + 1].getSex());
						(athlete + i)->setAge(athlete[i + 1].getAge());
						(fAthlete + i)->setDistance(fAthlete[i + 1].getDistance());

						//Finishing the Swap
						athlete[i + 1].setFirst(tempFirst);
						athlete[i + 1].setLast(tempLast);
						athlete[i + 1].setTeam(tempTeam);
						athlete[i + 1].setSex(tempSex);
						athlete[i + 1].setAge(tempAge);
						fAthlete[i + 1].setDistance(tempDist);

						//Set to 0 as long as there are athletes to compare
						repeat = 0;
					}
				}
			}

//Outputting
			int TAB = 10;

		//Header of List
			cout << setw(20) << "Name" << setw(15) << "Team" << setw(12) << "Gender" << setw(9) << "Age" << setw(17)
				<< "Distance(m)" << setw(TAB) << "Place" << endl;
			cout << "\t_____________________________________________________________________________\n";

			string fullName; //Full name for formatting

			n += 1;
		//Athlete List
			for (int i = 1; i < n; i++) {

				fullName = (athlete + i)->getLast() + ", " + (athlete + i)->getFirst(); //Adding first and last name together 
	//Output
				cout << setw(25) << fullName << setw(10) << (athlete + i)->getTeam();
				cout << setw(TAB) << (athlete + i)->getSex() << setw(TAB) << (athlete + i)->getAge() << setw(15) << (fAthlete + i)->getDistance() << setw(TAB) << i << endl;
			}


//Points Awarding 

			cout << "\n\n"; //Extra spacing

			for (int i = 1; i < n; i++) {

				int points = 0;
				switch (i) { //Switch to add points based on the placing of the athletes

				case 1: points = 10;
					break;
				case 2: points = 8;
					break;
				case 3:	points = 6;
					break;
				case 4: points = 4;
					break;
				case 5: points = 2;
					break;
				case 6: points = 2;
					break;
				default: points = 0;
					break;

				}
				cout << (athlete + i)->getFirst() << " " << (athlete + i)->getLast() << " from " << (athlete + i)->getTeam() << " gets " << points << " points!" << endl;
			}

		}

		else {
			cout << "\nPlease enter either 'name' or 'distance'\n"; //If input not recognized from if statements above 
			val = 1;
		}

	} while (val == 1);

}

void driver() {
	
	cout << "\n\nDriver Function\n";
	cout << "Sorting by Last Name:\n\n";

								/* FIELD DRIVER FUNCTION  */


//Max amount of athletes
	const int max = 100;

//Temp Vars
	string tempFirst, tempLast, tempTeam, tempSex;
	int tempAge = 0;
	double tempDist = 0.0;



//Object Vars
	eventParticipation athlete[max];	//Included an array off 100 as an object of eventParticipation class
										//100 is max number of athletes that can be inputted
	fieldParticipation fAthlete[max];	//Object for track athletes


//Hard coding athletes 

		athlete[0].setFirst("Justin"); //First Name
		athlete[0].setLast("Fairbanks");	//Last Name
		athlete[0].setTeam("NNU");	//Team Name
		athlete[0].setSex("Male");		//Sex of Athlete
		athlete[0].setAge(19);		//Age of Athlete
		fAthlete[0].setDistance(100.34);   //Storing tempTime to derived class

		athlete[1].setFirst("Dale"); //First Name
		athlete[1].setLast("Hamilton");	//Last Name
		athlete[1].setTeam("NNU");	//Team Name
		athlete[1].setSex("Male");		//Sex of Athlete
		athlete[1].setAge(32);		//Age of Athlete
		fAthlete[1].setDistance(89.7);   //Storing tempTime to derived class

		athlete[2].setFirst("Camden"); //First Name
		athlete[2].setLast("McGath"); //Last Name
		athlete[2].setTeam("BSU");	//Team Name
		athlete[2].setSex("Male");		//Sex of Athlete
		athlete[2].setAge(21);		//Age of Athlete
		fAthlete[2].setDistance(120.4);   //Storing tempTime to derived class

		athlete[3].setFirst("Jeff"); //First Name
		athlete[3].setLast("Fairbanks");	//Last Name
		athlete[3].setTeam("OSU");	//Team Name
		athlete[3].setSex("Male");		//Sex of Athlete
		athlete[3].setAge(22);		//Age of Athlete
		fAthlete[3].setDistance(111.41);   //Storing tempTime to derived class


		athlete[4].setFirst("Xavier"); //First Name
		athlete[4].setLast("Listo");	//Last Name
		athlete[4].setTeam("LSU");	//Team Name
		athlete[4].setSex("Male");		//Sex of Athlete
		athlete[4].setAge(18);		//Age of Athlete
		fAthlete[4].setDistance(88.11);   //Storing tempTime to derived class


		/********************************************************************************************************************/
//Sorting

		int repeat = 0; //For determining if all has been sorted correctly

		
		while (repeat == 0) {
			repeat = 1;
			for (int i = 0; i < 5; i++) {
				if ((athlete + i)->getLast() > athlete[i + 1].getLast()) {

				//Store in temporary variables
					tempFirst = (athlete + i)->getFirst();
					tempLast = (athlete + i)->getLast();
					tempTeam = (athlete + i)->getTeam();
					tempSex = (athlete + i)->getSex();
					tempAge = (athlete + i)->getAge();
					tempDist = (fAthlete + i)->getDistance();

				//Swap next object for previous
					(athlete + i)->setFirst(athlete[i + 1].getFirst());
					(athlete + i)->setLast(athlete[i + 1].getLast());
					(athlete + i)->setTeam(athlete[i + 1].getTeam());
					(athlete + i)->setSex(athlete[i + 1].getSex());
					(athlete + i)->setAge(athlete[i + 1].getAge());
					(fAthlete + i)->setDistance(fAthlete[i + 1].getDistance());

				//Finishing the Swap
					athlete[i + 1].setFirst(tempFirst);
					athlete[i + 1].setLast(tempLast);
					athlete[i + 1].setTeam(tempTeam);
					athlete[i + 1].setSex(tempSex);
					athlete[i + 1].setAge(tempAge);
					fAthlete[i + 1].setDistance(tempDist);

				//Set to 0 as long as there are athletes to compare
					repeat = 0;
				}
			}
		}

//Outputting
		int TAB = 10;
	
	//Header of List
		cout << setw(20) << "Name" << setw(15) << "Team" << setw(12) << "Gender" << setw(9) << "Age" << setw(17)
			<< "Distance(m)" << setw(TAB) << "Place" << endl;
		cout << "\t_____________________________________________________________________________\n";

		string fullName; //Full name for formatting
	//Athlete List
		for (int i = 1; i < 6; i++) {

			fullName = (athlete + i)->getLast() + ", " + (athlete + i)->getFirst(); //Adding first and last name together 
		//Output
			cout << setw(25) << fullName << setw(10) << (athlete + i)->getTeam();
			cout << setw(TAB) << (athlete + i)->getSex() << setw(TAB) << (athlete + i)->getAge() << setw(15) << (fAthlete + i)->getDistance() << setw(TAB) << i << endl;
		} 

	//Points Awarding 
		cout << "\n\n"; //Extra spacing
		for (int i = 1; i < 6; i++) {

			int points = 0;
			switch (i) { //Switch to add points based on the placing of the athletes

			case 1: points = 10;
				break;
			case 2: points = 8;
				break;
			case 3:	points = 6;
				break;
			case 4: points = 4;
				break;
			case 5: points = 2;
				break;
			default: points = 0;
				break;

			}
			cout << (athlete + i)->getFirst() << " " << (athlete + i)->getLast() << " from " << (athlete + i)->getTeam() << " gets " << points << " points!" << endl;
		}

}
