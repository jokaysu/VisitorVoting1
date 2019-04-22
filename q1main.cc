//q1main.cc

#include "q1print.h"
#include "q1voter.h"
#include "q1tallyVotes.h"
#include "MPRNG.h"
#include <iostream>

using namespace std;

MPRNG mprng;

int main(int argc, char *argv[]) {
	unsigned int voters = 6, group = 3, votes = 1, seed = getpid();
	try {
		if (argc >= 2) voters = atoi(argv[1]);
		if (argc >= 3) group = atoi(argv[2]);
		if (argc >= 4) votes = atoi(argv[3]);
		if (argc >= 5) seed = atoi(argv[4]);
		if (argc > 5) {
			cout << "Usage: vote [ voters |’x’[ group |’x’[ votes |’x’[ seed |’x’] ] ]" << endl;
			return 0;
		}
	} catch (...) {
		cout << "Usage: vote [ voters |’x’[ group |’x’[ votes |’x’[ seed |’x’] ] ]" << endl;
		return 0;
	}

	//set seed
	mprng.set_seed(seed);
	//cout << "seed = " << seed << endl;

	Printer printer(voters);
	TallyVotes tv(voters, group, printer);
	Voter *voter[voters];
	for (unsigned int i = 0; i < voters; ++i) {
		voter[i] = new Voter(i, votes, tv, printer);
	}
	for (unsigned int i = 0; i < voters; ++i) {
		delete voter[i];
	}
}