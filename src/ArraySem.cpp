#include "ArraySem.h"

#include "KerSem.h"


ArraySem:: ArraySem(int num){
	array = new KernelSem*[num];
	for(int i = 0; i < num; i++)
		array[i] = 0;
	numSem = 0;
	available = 0;
	capacity = num;
}

ArraySem:: ~ArraySem(){
	for(int i = 0; i < capacity; i++)
			array[i] = 0;
	delete array;
	array = 0;
	numSem = 0;
	available = 0;
	capacity = 0;
}

ID ArraySem::add(KernelSem* newSem){//uvek imamo prazno mesto a kad dodamo novi elem onda proveravamo i resavamo ako je doslo do prepunjenja
	array[available] = newSem;
	ID id = available;
	numSem++;
	if (numSem == capacity){
		KernelSem** old = array;
		array = new KernelSem* [capacity*2];

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

KernelSem* ArraySem:: remove(ID id){
	if (id < 0 || id > capacity - 1) return 0;
	if (id < available)
		available = id;
	KernelSem* old = array[id];
	array[id] = 0;
	numSem--;
	return old;
}

KernelSem* ArraySem:: get(ID id){
	if (id < 0 || id > capacity - 1) return 0;
	return array[id];
}

int ArraySem::getCapacity(){
	return capacity;
}

