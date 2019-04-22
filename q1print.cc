//q1print.cc

#include "q1print.h"

using namespace std;

string Printer::mySpace(int x) {
	string a = "";
	for (int i = 0; i < x; ++i) 
		a = a + " ";
	return a;
}

void Printer::putStrIntoStates(unsigned int id, string str) {
	if (states[id].valid) {
		//cout << "a" << endl;
		for (unsigned int i = 0; i < voters; ++i) {
			//cout << "i = " << i << " id = " << id << endl;
			if (states[i].valid) {
				cout << states[i].message;
			} else {
				cout << mySpace(8);
			}
			states[i].valid = false;
		}
		cout << endl;
		states[id].valid = true;
		states[id].message = str;
	} else {
		//cout << "b" << endl;
		states[id].valid = true;
		states[id].message = str;
	}
}

Printer::Printer(unsigned int voters) : voters(voters) {
	states = new printerMessage[voters];
	for (unsigned int i = 0; i < voters; ++i) {
		cout << "V" << i << mySpace(6);
		states[i].valid = false;
	}
	cout << endl;
	for (unsigned int i = 0; i < voters; ++i) {
		cout << "******* ";
	}
	cout << endl;
}

Printer::~Printer() {
	for (unsigned int i = 0; i < voters; ++i) {
		if (states[i].valid) {
			cout << states[i].message;
		} else {
			cout << mySpace(8);
		}
		states[i].valid = false;
	}
	cout << endl;
	delete [] states;
	cout << "*****************" << endl;
	cout << "All tours started" << endl;
}

void Printer::print( unsigned int id, Voter::States state ) {
	stringstream ss;
	ss << (char)state << mySpace(7);
	putStrIntoStates(id, ss.str());
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {
	stringstream ss;
	ss << (char)state << " " << (char)tour << mySpace(5);
	putStrIntoStates(id, ss.str());
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ) {
	stringstream ss;
	ss << (char)state << " " << ballot.picture << "," << ballot.statue << "," << ballot.giftshop << " ";
	putStrIntoStates(id, ss.str());
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
	stringstream ss;
	ss << (char)state << " " << numBlocked << mySpace(5);
	putStrIntoStates(id, ss.str());
}