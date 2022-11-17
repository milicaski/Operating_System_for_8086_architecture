#ifndef _ARRAYIVT_H_
#define _ARRAYIVT_H_

#include "IVTEn.h"

class ArrayIVTEntry{
public:
	ArrayIVTEntry();
	~ArrayIVTEntry();
	void add(IVTEntry* newPCB, IVTNo ivtNo);
	IVTEntry* get(IVTNo ivtNo);
	IVTEntry* remove(IVTNo ivtNo);
private:
	IVTEntry **events;
	int numEvents;
	int capacity;
};


#endif
