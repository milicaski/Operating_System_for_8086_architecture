#ifndef LIST_H_
#define LIST_H_

class PCB;

class List{
public:
	struct Node{
		PCB* pcb;
		Node *next;
		Node *prev;
		Node(PCB* newPCB){
			pcb = newPCB;
			next = 0;
			prev = 0;
		}
		~Node(){
			pcb = 0;
			next = 0;
			prev = 0;
		}
	};
	List();
	~List();
	void add(PCB *newPCB);
	PCB* getFirst();
	PCB* removeFirst();
	void currAtFirst();
	void moveCurr();
	int isCurr();
	PCB* getCurr();
	PCB* removeCurr();
	int getNumNodes();
private:
	Node *head;
	Node *tail;
	Node *curr;
	int numNodes;
};



#endif
