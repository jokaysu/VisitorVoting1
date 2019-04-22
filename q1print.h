//q1print.h

#ifndef Q1PRINT_H
#define Q1PRINT_H

#include "q1voter.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

_Monitor Printer {    // chose one of the two kinds of type constructor
	unsigned int voters;
	struct printerMessage{
		string message;
		bool valid;
	};
	printerMessage *states;

	string mySpace(int x);
	void putStrIntoStates(unsigned int id, string str);
  public:
    Printer( unsigned int voters );
    ~Printer();
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
    void print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif