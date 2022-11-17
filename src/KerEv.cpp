#include "KerEv.h"
#include "IVTEn.h"
#include "ArrayIVT.h"
#include <iostream.h>


extern volatile PCB* running;

KernelEv::KernelEv(IVTNo ivtNo, Event *myEv){
	val = 0;
	this->myEv = myEv;
	numEntry = ivtNo;
	owner = (PCB*)running;
	blocked = 0;
	myIVTEntry = IVTEntry::getIVTEntryByEntry(ivtNo);
	myIVTEntry->setMyKernelEv(this);
}


KernelEv::~KernelEv(){
	myEv = 0;
	if (myIVTEntry != 0)
		myIVTEntry->unSetMyKernelEv();
	myIVTEntry = 0;
	owner = 0;
	blocked = 0;
}

void KernelEv::signal(){
	myEv->signal();
}
