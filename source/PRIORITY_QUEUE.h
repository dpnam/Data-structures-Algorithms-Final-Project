#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H
#define MAX_SIZE 1000

template <class T> class PRIORITY_QUEUE {
private:
	T *items;
	int rear;
	int maxsize;

	void heapify(int);
public:
	PRIORITY_QUEUE();
	PRIORITY_QUEUE(int);
	PRIORITY_QUEUE(const PRIORITY_QUEUE&);
	~PRIORITY_QUEUE();

	bool isEmpty();
	bool enQueue(T);
	bool deQueue(T&);
	bool minValue(T&);
};

template <class T> PRIORITY_QUEUE<T>::PRIORITY_QUEUE() {
	maxsize = MAX_SIZE;
	rear = -1;
	items = new T[maxsize];
}
template <class T> PRIORITY_QUEUE<T>::PRIORITY_QUEUE(int size) {
	maxsize = size;
	rear = -1;
	items = new T[maxsize];
}
template <class T> PRIORITY_QUEUE<T>::PRIORITY_QUEUE(const PRIORITY_QUEUE &aQueue) {
	if (this->items == aQueue.items) return;
	delete[] items;

	this->maxsize = aQueue.maxsize;
	this->items = new T[maxsize];
	for (int i = 0; i <= aQueue.rear; i++)
		this->items[i] = aQueue.items[i];
	this->rear = aQueue.rear;
}
template <class T> PRIORITY_QUEUE<T>::~PRIORITY_QUEUE() {
	if (!isEmpty()) {
		delete[] items;
		items = nullptr;
	}
	rear = -1;
	maxsize = 0;
}
template <class T> void PRIORITY_QUEUE<T>::heapify(int pos) {
	T saved = items[pos];
	while (pos <= rear / 2) {
		int child = 2 * pos + 1;
		if (child <= rear)
			if (items[child] > items[child + 1]) child++;
		if (saved <= items[child]) break;
		items[pos] = items[child];
		pos = child;
	}
	items[pos] = saved;
}
template <class T> bool PRIORITY_QUEUE<T>::isEmpty() {
	return (rear == -1) || (items == nullptr);
}
template <class T> bool PRIORITY_QUEUE<T>::enQueue(T newItem) {
	if (rear == maxsize - 1) return false;
	rear++;
	items[rear] = newItem;
	int pos = rear;
	T saved = items[pos];
	while (1) {
		int parent = (pos - 1) / 2;
		if (saved >= items[parent]) break;
		items[pos] = items[parent];
		pos = parent;
		if (pos == 0) break;
	}
	items[pos] = saved;
	return true;
}
template <class T> bool PRIORITY_QUEUE<T>::deQueue(T &item) {
	if (isEmpty()) return false;
	item = items[0];
	items[0] = items[rear];
	rear--;
	heapify(0);
	return true;
}
template <class T> bool PRIORITY_QUEUE<T>::minValue(T &item) {
	if (isEmpty()) return false;
	item = items[0];
	return true;
}

#endif