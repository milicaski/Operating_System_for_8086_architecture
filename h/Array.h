#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "PCB.h"


class Array{
public:
	Array(int num = 100);
	~Array();
	ID add(PCB* newPCB);
	PCB* get(ID id);
	PCB* remove(ID id);
private:
	PCB **array;
	ID available;//najnize slobodno mesto
	int numThreads;
	int capacity;
};




#endif
