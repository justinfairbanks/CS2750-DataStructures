								/* Justin Fairbanks - Big Heap Sort */
									/* Last Updated - 12.17.2022 */
/* Libraries */
#include <iostream>
#include <string> //For Strings
#include <fstream> //For use of files
#include <vector> //STL Vector

using namespace std; 


/* Prototypes */
void readFile(string, string, string); //Read Data of File into STL Vector
void print(vector < int>); //Print Contents of Vector

/* Heap Sort Functions*/
void heapSort(vector<int>&, int, bool);
void heapify(vector<int>&, int, int, bool);


/* Start of Program */
int main(int argc, char* argv[])
{

	/* Variables */
	string file_path;
	string output;
	string order;

	ifstream temp; //To open file for validation



	switch (argc) //Argument Count
	{

/* No Arguments */
	case 1: 

		cout << "No Command Line Arguments Entered..." << endl << endl;

		/* Paramater Instructions */
		cout << "Please enter the first parameter as '-a' or '-d' for order type of the data" << endl
			<< "second parameter as input file and the third parameter as optional output file." << endl;

		cout << endl << endl << "Running Test File: ch9nums1.dat" << endl << endl << endl;

		file_path = "ch9nums1.dat"; //Relative Test File Path
		order = "-a"; //Sort Data in Ascending Order
		output = "NULL"; //No Output File

		temp.open(file_path); //Open Test File

		if (!temp) //If file doesn't open for some reason
		{
			cout << endl << "Error opening hardcoded test file...\nPlease try command line args" << endl;
			break; //Breaks out to close program
		}

		readFile(order, file_path, output);

		temp.close(); //Closes File

		break;

	
/* Only Order Style Given (No Input File) */

	case 2:

		cout << "Incorrect Command Line Args..." << endl << endl;

		/* Paramater Instructions */
		cout << "Please enter the 1st parameter as '-a' or '-d' for order type of the data" << endl
			<< "2nd parameter as input file and the 3rd parameter as optional output file." << endl;
		
		break;


/* Input File Given in Command Line */

	case 3:

		order = argv[1]; //Ascending or Descending for Heap 

		while (order != "-a" && order != "-d")
		{
			cout << "Please enter either '-a' or '-d' for the sorting method of the Heap: \n";
			cin >> order;
		}

		file_path = argv[2]; //Input File


	/* Input File Validation */

		temp.open(file_path);

		while (!temp)
		{
			cout << "Input file not found, please try again: " << endl;
			getline(cin, file_path);
			temp.open(file_path);
		}

		temp.close();

		output = "NULL"; //No output file given

		readFile(order, file_path, output);

		break;


/* Input and Output File Given in Command Line */

	case 4:

		order = argv[1]; //Ascending or Descending for Heap 

		while (order != "-a" && order != "-d")
		{
			cout << "Please enter either '-a' or '-d' for the sorting method of the Heap: \n";
			cin >> order;
		}

		file_path = argv[2]; //Input File

		output = argv[3]; //Output File



	/* Input File Validation */

		temp.open(file_path); //Open Temp Input File

		while (!temp)
		{
			cout << "Input file not found, please try again: " << endl;
			getline(cin, file_path);
			temp.open(file_path);
		}

		temp.close(); //Close Temp Input File



	/* Output File Validation */

		temp.open(output); //Open Temp Output File

		while (!temp)
		{
			cout << "Output file not found, please try again: " << endl;
			getline(cin, output);
			temp.open(output);
		}

		temp.close(); //Close Temp Output File

		cout << "Output wrote to: " << output << endl;

		readFile(order, file_path, output);

		break;


/* Too many command line args entered */

	default:
		cout << endl << "Too Many Command Line Arguments!" << endl;
		break;
	}

/* Closing of Program */

	cin.clear();
	cout << endl << endl << endl << "Press Enter to Close..." << endl;
	cin.get();

	return 0;
}

/* Reads Numbers from file and sorts them using Heap Sort*/
void readFile(string order, string input, string output)
{
	/* File Objects */
	ifstream in; //For source data
	ofstream out; //Output file


	/* Temp Vars */
	bool ascending; //Ascending/Descending (user input)
	int tempNum = 0; //For reading in numbers from file
	vector<int> data; //STL Vector to input file data
	int fileSize = 0; //Size of Vector



	if (order == "-a")
		ascending = true; // True for ascending order 
	else
		ascending = false; // False for descending order


	in.open(input); //Open input file

	if (output != "NULL")
		out.open(output); //Open output file


/* Load File into Vector */

	while (in >> tempNum) //Reads number of file into tempNum
		data.push_back(tempNum); //Loads Number into Vector

	fileSize = data.size(); //Size of Vector


	/* Output Inputted File */
	if (output == "NULL") //If outputting to Console
	{
		cout << "\t\t== Input File ==" << endl << endl;
		print(data); //Output contents of Vector
	}


/* Load vector into Heap Data Structure */

	heapSort(data, fileSize, ascending); //Sort Vector


	if (output == "NULL") //Writing to Console
	{
		if (ascending == true)
			cout << endl << endl << "\t== Heap Sorted in Ascending Order ==" << endl << endl;
		else
			cout << endl << endl << "\t== Heap Sorted in Descending Order ==" << endl << endl;
	}

	else //Writing to file
	{
		if (ascending == true)
			out << "\t== Heap Sorted in Ascending Order ==" << endl << endl;
		else
			out << "\t== Heap Sorted in Descending Order ==" << endl << endl;
	}


/* Output Heap Sorted Vector */

	if (output == "NULL") //Outputting to console
		print(data); //Outputs now sorted contents of Vector

	else //Outputting to File
	{
		for (auto it = data.begin(); it != data.end(); it++) //Output contents of Vector
			out << *it << " ";
	}


/* Close Files */
	in.close();

	if (output != "NULL")
		out.close();
}

/* Output Contents of Vector */
void print(vector<int> data)
{
	for (auto it = data.begin(); it != data.end(); it++) //Output contents of Vector
		cout << *it << " ";
}


/* Following Functions Adapted from https://www.geeksforgeeks.org/cpp-program-for-heap-sort/ */
void heapSort(vector<int>& data, int fileSize, bool ascending)
{

	/* Build heap(rearrange array) */
	for (int i = fileSize / 2 - 1; i >= 0; i--)
		heapify(data, fileSize, i, ascending);

	/* Extract Elements One by One From Heap */
	for (int i = fileSize - 1; i >= 0; i--) {
		swap(data[0], data[i]); //Swap current root to end

		heapify(data, i, 0, ascending); //Call max heapify on the reduced heap
	}
}

void heapify(vector<int>& data, int fileSize, int i, bool ascending)
{

	int largest = i; //Initialize largest as root
	int l = 2 * i + 1; //left
	int r = 2 * i + 2; //right

	/* Ascending Order */
	if (ascending == true)
	{
		if (l < fileSize && data[l] > data[largest])
			largest = l;

		//If right child is larger than largest so far
		if (r < fileSize && data[r] > data[largest])
			largest = r;

		//If largest is not root
		if (largest != i) {
			swap(data[i], data[largest]);

			// Recursively heapify the affected sub-tree
			heapify(data, fileSize, largest, ascending);
		}
	}

	/* Descending Order */
	else
	{
		if (l < fileSize && data[l] < data[largest])
			largest = l;

		//If right child is larger than largest so far
		if (r < fileSize && data[r] < data[largest])
			largest = r;

		//If largest is not root
		if (largest != i) {
			swap(data[i], data[largest]);

			// Recursively heapify the affected sub-tree
			heapify(data, fileSize, largest, ascending);
		}
	}
}