#pragma once
using namespace std;
#include <string>

class eventParticipation {

	//Protected Members of the Class
protected:

	//Variables that apply to all athletes

	string first, last, team, sex;
	int age = 0;

	//Public Member Variables (Prototypes)

public:

	void setFirst(string);
	string getFirst() const;		//First Name

	void setLast(string);
	string getLast() const;			//Last Name

	void setTeam(string);
	string getTeam() const;			//Team Name

	void setSex(string);
	string getSex() const;			//Sex of Athlete

	void setAge(int);
	int getAge() const;				//Age of Athlete

};


//Derived classes


class fieldParticipation : protected eventParticipation {

protected: 
	double distance = 0.0; //Distance of Field Events

public:
	void setDistance(double);
	double getDistance() const;		//Distance of Field Event
};



class trackParticipation : protected eventParticipation{

protected: 
	double time = 0.0; //Time of Track Events

public:
	void setTime(double);
	double getTime() const;			//Time of Track Event
};