#include "IVTEn.h"
#include <dos.h>
#include "ArrayIVT.h"
#include "KerEv.h"
#include <iostream.h>

extern volatile PCB* running;
//extern volatile int context_switch_on_demand;
extern volatile int dispatch_demand;

#define lock running->lockFlag++;\

#define unlock\
	--(running->lockFlag);\
	if(running->lockFlag == 0 && dispatch_demand){\
		dispatch();\
	}\


volatile ArrayIVTEntry *events = new ArrayIVTEntry();

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newInt){
	lock
	numEntry = ivtNo;
	myKernelEv = 0;
	oldRut = getvect(ivtNo);
	setvect(ivtNo, newInt);
	((ArrayIVTEntry*)events)->add(this, ivtNo);
	unlock
}

IVTEntry::~IVTEntry(){
	lock
	setvect(numEntry, oldRut);
	((ArrayIVTEntry*)events)->remove(numEntry);
	myKernelEv = 0;
	unlock
}

IVTEntry* IVTEntry::getIVTEntryByEntry(IVTNo ivtNo){
	return ((ArrayIVTEntry*)events)->get(ivtNo);
}

void IVTEntry::signal(){
	if(myKernelEv != 0){
		myKernelEv->signal();
	}
}

void IVTEntry::callOld(){
	oldRut();
}

void IVTEntry::setMyKernelEv(KernelEv *myKernelEv){
	if(this->myKernelEv == 0 && myKernelEv != 0 && this->numEntry == myKernelEv->numEntry){
		this->myKernelEv = myKernelEv;
	}
}

void IVTEntry::unSetMyKernelEv(){
	this->myKernelEv = 0;
}
