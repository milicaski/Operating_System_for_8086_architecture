#ifndef _ARRAYSEM_H_
#define _ARRAYSEM_H_

#include "KerSem.h"


class ArraySem{
public:
	ArraySem(int num = 100);
	~ArraySem();
	ID add(KernelSem* newSem);
	KernelSem* get(ID id);
	KernelSem* remove(ID id);
	int getCapacity();
private:
	KernelSem **array;
	ID available;//najnize slobodno mesto
	int numSem;
	int capacity;
};



#endif
