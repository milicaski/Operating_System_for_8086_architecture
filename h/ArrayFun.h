#ifndef ARRAYFUN_H_
#define ARRAYFUN_H_

typedef void (*SignalHandler)();
typedef unsigned SignalId;

class ArrayFun{
public:
	struct Field{
		int blocked;
		SignalHandler *arrayHandlers;
		int numHandlers;
		int capacity;
		Field(){
			blocked = 0;
			arrayHandlers = 0;
			numHandlers = 0;
			capacity = 0;
		}
		~Field(){
			removeAllH();
			delete arrayHandlers;
			blocked = 0;
			arrayHandlers = 0;
			numHandlers = 0;
			capacity = 0;
		}
		void addH(SignalHandler hand);
		void removeAllH();
		void swap(SignalHandler hand1, SignalHandler hand2);

	};
	friend class PCB;
	ArrayFun();
	ArrayFun(ArrayFun *funSignals);
	//~ArrayFun();
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);
private:
	Field arraySignal[16];
};



#endif
