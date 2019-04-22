//q1tallyVotesSEM.cc

#include "q1tallyVotes.h"
#include "q1print.h"
#include "q1voter.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) : 
	waitOtherSem(0), signalFlag(0), voters(voters), group(group), printer(printer), 
	remain(voters), curVoters(0) {
		ClearStat();
	}

void TallyVotes::BallotStat(Ballot ballot) {
	stats.picture += ballot.picture;
	stats.statue += ballot.statue;
	stats.giftshop += ballot.picture;
}

void TallyVotes::ClearStat() {
	stats.picture = stats.statue = stats.giftshop = 0;
}

void TallyVotes::CalResult() {
	if (stats.statue > stats.picture && stats.statue > stats.giftshop) {
		result = Statue;
	} else if (stats.picture > stats.giftshop) {
		result = Picture;
	} else {
		result = GiftShop;
	}
}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
	if (remain < group) {
		while(!waitOtherSem.empty()) waitOtherSem.V();
		return Failed;
	}
	if (signalFlag) {
		printer.print(id, Voter::Barging);
		if (bargeSem.empty()) signalFlag = false;
	}
	bargeSem.P();
	mutex.P();

	//count
	curVoters++;
	BallotStat(ballot);
	if (curVoters < group) {
		printer.print(id, Voter::Block, curVoters);
		bargeSem.V();
		waitOtherSem.P(mutex);

		//check fail
		//if (remain < group && waitOtherSem.empty()) {
		//	while(!waitOtherSem.empty()) waitOtherSem.V();
		//	mutex.V();
		//	return Failed;
		//}
		printer.print(id, Voter::Unblock, curVoters);
	} else {
 		printer.print(id, Voter::Complete);
		CalResult();
		ClearStat();
		curVoters -= group;
	}

	//call others
	signalFlag = true;
	if (bargeSem.empty() && waitOtherSem.empty()) signalFlag = false;
	if (!waitOtherSem.empty()) {
		waitOtherSem.V();
	} else {
		bargeSem.V();
	}
	Tour myresult = result;
	mutex.V();
	return myresult;
}

void TallyVotes::done() {
	remain--;
}