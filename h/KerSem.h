#ifndef _KERNELSEM_H_
#define _KERNELSEM_H_

#include "semaphor.h"
#include "ListSem.h"
typedef int ID;



class KernelSem{
public:
	KernelSem(int init, Semaphore *mySem);
	~KernelSem();
	int getVal() const;
	int block(int maxTimeToWait);
	int deblock(int num);
	ID getId();
	static void freeIfTime();
private:
	friend class Semaphore;
	Semaphore *mySem;
	int val;
	ID id;
	ListSem *waitingOnSem;
};



#endif
