//q1tallyvotes.h

#ifndef Q1TALLYVOTES_H
#define Q1TALLYVOTES_H

class Printer;

#if defined( MC )                    // mutex/condition solution
// includes for this kind of vote-tallier
class TallyVotes {
    uOwnerLock mutex;
    uCondLock bargeLock;
    uCondLock waitOtherLock;
    bool signalFlag;
    // private declarations for this kind of vote-tallier
#elif defined( SEM )                // semaphore solution
#include <uSemaphore.h>
// includes for this kind of vote-tallier
class TallyVotes {
    uSemaphore mutex;
    uSemaphore bargeSem;
    uSemaphore waitOtherSem;
    bool signalFlag;
    //uSemaphore endingSem;
    // private declarations for this kind of vote-tallier
#elif defined( BAR )                // barrier solution
#include <uBarrier.h>
// includes for this kind of vote-tallier
_Cormonitor TallyVotes : public uBarrier {
    void block( unsigned int id );
    // private declarations for this kind of vote-tallier
#else
    #error unsupported voter type
#endif
    // common declarations
  public:
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g', Failed = 'f' };
  private:
    unsigned int voters;
    unsigned int group;
    Printer & printer;

    unsigned int remain;
    unsigned int curVoters;
    Ballot stats;
    Tour result;

    void BallotStat(Ballot ballot);
    void ClearStat();
    void CalResult();
  public:                            // common interface
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    Tour vote( unsigned int id, Ballot ballot );
    void done();
};

#endif