#ifndef LISTSIG_H_
#define LISTSIG_H_

class ListSig {
public:
	struct Node {
		int signal;
		Node *next;
		Node *prev;
		Node(int newSignal) {
			signal = newSignal;
			next = 0;
			prev = 0;
		}
		~Node(){
			next = 0;
			prev = 0;
		}
	};
	ListSig();
	~ListSig();
	void put(int newSignal);
	int get();
	int remove();
	void skip();

	int getNumSignals();
private:
	Node *head;
	Node *tail;
	Node *front;
	int numSignals;

};

#endif
