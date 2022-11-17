#include "ArrayIVT.h"

ArrayIVTEntry:: ArrayIVTEntry(){
	events = new IVTEntry*[256];
	for(int i = 0; i < 256; i++)
		events[i] = 0;
	numEvents = 0;
	capacity = 256;
}

ArrayIVTEntry:: ~ArrayIVTEntry(){
	for(int i = 0; i < capacity; i++)
			events[i] = 0;
	delete events;
	events = 0;
	numEvents = 0;
	capacity = 0;
}

void ArrayIVTEntry::add(IVTEntry* newIVTEntry, IVTNo ivtNo){
	if(events[ivtNo] != 0) return;
	events[ivtNo] = newIVTEntry;
	numEvents++;
}

IVTEntry* ArrayIVTEntry:: remove(IVTNo ivtNo){
	if (ivtNo > capacity - 1) return 0;

	IVTEntry* old = events[ivtNo];
	events[ivtNo] = 0;
	numEvents--;
	return old;
}

IVTEntry* ArrayIVTEntry:: get(IVTNo ivtNo){
	if (ivtNo > capacity - 1) return 0;
	return events[ivtNo];
}


