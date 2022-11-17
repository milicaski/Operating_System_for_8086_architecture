#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_



#define PREPAREENTRY(numEntry, doCallOld)\
	void interrupt interRut##numEntry(...);\
	IVTEntry ivtEntry##numEntry(numEntry, interRut##numEntry); \
	void interrupt interRut##numEntry(...) {\
			ivtEntry##numEntry.signal();\
			if (doCallOld == 1)\
				ivtEntry##numEntry.callOld();\
}


typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newInt);
	~IVTEntry();
	static IVTEntry* getIVTEntryByEntry(IVTNo ivtNo);
	void signal();
	void callOld();
	void setMyKernelEv(KernelEv *myKernelEv);
	void unSetMyKernelEv();
private:
	KernelEv *myKernelEv;
	IVTNo numEntry;
	pInterrupt oldRut;
};



#endif
