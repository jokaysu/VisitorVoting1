//q1tallyvotesMC.cc

#include "q1tallyVotes.h"
#include "q1print.h"
#include "q1voter.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) : 
	signalFlag(false), voters(voters), group(group), printer(printer), 
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
		waitOtherLock.broadcast();
		return Failed;
	}
	mutex.acquire();
	if (signalFlag) {
		printer.print(id, Voter::Barging);
		bargeLock.wait(mutex);
		if (bargeLock.empty()) signalFlag = false;
	}

	//count voters
	curVoters++;
	if (curVoters < group) {
		printer.print(id, Voter::Block, curVoters);
		BallotStat(ballot);
		bargeLock.signal();
		waitOtherLock.wait(mutex);

		/* 
			seems unneccesary because we can use failing one to broadcast

			discovered this after making the SEM version.
		*/
		//check failed
		//if (remain < group) {
		//	waitOtherLock.broadcast();
		//	mutex.release();
		//	return Failed;
		//}
		printer.print(id, Voter::Unblock, curVoters);
	} else { 
		//last one
		BallotStat(ballot);
		printer.print(id, Voter::Complete);
		CalResult();
		ClearStat();
		curVoters -= group;
	}

	//call others
	signalFlag = true;
	if (bargeLock.empty() && waitOtherLock.empty()) signalFlag = false;
	if (!waitOtherLock.empty()) {
		waitOtherLock.signal();
	} else {
		bargeLock.signal();
	}
	Tour myResult = result;
	mutex.release();
	return myResult;
}

void TallyVotes::done() {
	remain--;
}