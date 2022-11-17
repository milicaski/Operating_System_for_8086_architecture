#include "Array.h"
#include <iostream.h>


Array:: Array(int num){
	array = new PCB*[num];
	for(int i = 0; i < num; i++)
		array[i] = 0;
	numThreads = 0;
	available = 0;
	capacity = num;
}

Array:: ~Array(){
	for(int i = 0; i < capacity; i++)
		array[i] = 0;
	delete array;
	array = 0;
	numThreads = 0;
	available = 0;
	capacity = 0;
}

ID Array::add(PCB* newPCB){//uvek imamo prazno mesto a kad dodamo novi elem onda proveravamo i resavamo ako je doslo do prepunjenja
	array[available] = newPCB;
	ID id = available;
	numThreads++;
	if (numThreads == capacity){
		PCB** old = array;
		array = new PCB* [capacity*2];

		for(int j = 0; j < capacity*2; j++)
				array[j] = 0;

		for (int i = 0; i < capacity; i++){
			array[i] = old[i];
			old[i] = 0;
		}
		delete old;//valjda ok
		available = capacity;
		capacity *= 2;
	}else{
		for (int i = available + 1; i < capacity; i++){
			if (array[i] == 0){
				available = i;
				break;
			}
		}
	}
	return id;
}

PCB* Array:: remove(ID id){
	if (id < 0 || id > capacity - 1) return 0;
	if (id < available)
		available = id;
	PCB* old = array[id];
	array[id] = 0;
	numThreads--;
	return old;
}

PCB* Array:: get(ID id){
	if (id < 0 || id > capacity - 1) return 0;
	return array[id];
}
