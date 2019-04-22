//q1tallyVotesBAR.cc

#include "q1tallyVotes.h"
#include "q1print.h"
#include "q1voter.h"

void TallyVotes::block( unsigned int id ) {
	if (uBarrier::waiters() + 1 < group) {
		printer.print(id, Voter::Block, uBarrier::waiters() + 1);
		uBarrier::block();
		if (result == Failed) {
			return;
		}
		printer.print(id, Voter::Unblock, uBarrier::waiters());
	} else {
		if (remain < group) {
			result = Failed;
			uBarrier::block();
			return;
		}
		printer.print(id, Voter::Complete);
		CalResult();
		ClearStat();
		uBarrier::block();
	}
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

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) : 
	uBarrier(group), voters(voters), group(group), printer(printer), 
	remain(voters), curVoters(0) {
		ClearStat();
	}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
	if (remain < group) {
		return Failed;
	}

	BallotStat(ballot);

	block(id);

	//cout << "result = " << result << endl;
	return result;

}

void TallyVotes::done() {
	remain--;
	if (remain + 1  <= group)
		uBarrier::block();
}