#include "eventParticipation.h" //Includes eventParticipation class file

void eventParticipation::setFirst(string f)
{
	first = f; //Takes first name passed into function and sets it to local var in class
}

string eventParticipation::getFirst() const
{
	return string(first);	//Returns the local var that was assigned passed in string from source.cpp
}

void eventParticipation::setLast(string l)
{
	last = l;
}

string eventParticipation::getLast() const
{
	return string(last);
}

void eventParticipation::setTeam(string t)
{
	team = t;
}

string eventParticipation::getTeam() const
{
	return string(team);
}

void eventParticipation::setSex(string s)
{
	sex = s;
}

string eventParticipation::getSex() const
{
	return string(sex);
}

void eventParticipation::setAge(int a)
{
	age = a;
}

int eventParticipation::getAge() const
{
	return age;
}

//Derived Classes


void fieldParticipation::setDistance(double d)
{
	distance = d;
}

double fieldParticipation::getDistance() const
{
	return distance;
}

void trackParticipation::setTime(double ti)
{
	time = ti;
}

double trackParticipation::getTime() const
{
	return time;
}
