#include "ArrayFun.h"


ArrayFun::ArrayFun(){

}

ArrayFun::ArrayFun(ArrayFun *funSignals){
	for(int i = 0; i < 16; i++){
		arraySignal[i].blocked = funSignals->arraySignal[i].blocked;
		if (funSignals->arraySignal[i].arrayHandlers == 0)
			arraySignal[i].arrayHandlers = 0;
		else{
			arraySignal[i].arrayHandlers = new SignalHandler[funSignals->arraySignal[i].capacity];
			for (int j = 0; j < funSignals->arraySignal[i].capacity; j++){
				arraySignal[i].arrayHandlers[j] = 0;
			}
			for(int k = 0; k < funSignals->arraySignal[i].numHandlers; k++){
				arraySignal[i].arrayHandlers[k] = funSignals->arraySignal[i].arrayHandlers[k];
			}
		}
		arraySignal[i].capacity = funSignals->arraySignal[i].capacity;
		arraySignal[i].numHandlers = funSignals->arraySignal[i].numHandlers;
	}
}

void ArrayFun::registerHandler(SignalId signal, SignalHandler handler){
	arraySignal[signal].addH(handler);
}

void ArrayFun::unregisterAllHandlers(SignalId id){
	arraySignal[id].removeAllH();
}

void ArrayFun::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	arraySignal[id].swap(hand1, hand2);
}

void ArrayFun::blockSignal(SignalId signal){
	arraySignal[signal].blocked = 1;
}

void ArrayFun::unblockSignal(SignalId signal){
	arraySignal[signal].blocked = 0;
}

void ArrayFun::Field::addH(SignalHandler hand){
	if(arrayHandlers == 0){
		arrayHandlers = new SignalHandler[10];
		for (int i = 0; i < 10; i++)
			arrayHandlers[i] = 0;
		capacity = 10;
		arrayHandlers[0] = hand;
	}else if(numHandlers == capacity){
		SignalHandler *old = arrayHandlers;
		arrayHandlers = new SignalHandler[capacity * 2];
		for(int i = 0; i < capacity * 2; i++)
			arrayHandlers[i] = 0;
		for(int j = 0; j < capacity; j++){
			arrayHandlers[j] = old[j];
			old[j] = 0;
		}
		delete old;
		capacity = capacity * 2;
		arrayHandlers[numHandlers] = hand;
	}else{
		arrayHandlers[numHandlers] = hand;
	}
	numHandlers++;
}

void ArrayFun::Field::removeAllH(){
	for(int i = 0; i < numHandlers; i++){
		arrayHandlers[i] = 0;
	}
	//delete arrayHandlers;??????
	//arrayHandlers = 0;
	numHandlers = 0;
}

void ArrayFun::Field::swap(SignalHandler hand1, SignalHandler hand2){
	int h1 = -1;
	int h2 = -1;
	for(int i = 0; i < numHandlers; i++){
		if(arrayHandlers[i] == hand1)
			h1 = i;
		if(arrayHandlers[i] == hand2)
			h2 = i;
	}
	if(h1 >= 0 && h2 >= 0){
		SignalHandler tmp = arrayHandlers[h1];
		arrayHandlers[h1] = arrayHandlers[h2];
		arrayHandlers[h2] = tmp;
	}
}
