#ifndef ROTLIST_H
#define ROTLIST_H

template<class T>
class  RotList
{
private:
	struct Ptr {
		Ptr *next;
		T *elem;
		Ptr() : next(NULL) , elem(NULL) {}
	};
public:
	RotList()
	 : head(NULL)
	 , current(NULL)
	{
	}
	T *ptr() { return current->elem; }
	void reset() { current = head; }
	void add(T *e) {
		if ( !head )
		{
			Serial.println("addNew");
			head = new Ptr();
			head->next = head;
			head->elem = e;
			current = head;
		} else {
			Serial.println("add");
			Ptr *n = new Ptr();
			current->next = n;
			n->elem = e;
			n->next = head;
			current = n;
		}
	}
	T *next() {
		current = current->next;
		return current->elem;
	}
private:
	Ptr *head;
	Ptr *current;
};
#endif