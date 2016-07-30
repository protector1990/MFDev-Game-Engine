/** Copyright 2016 MarFil Studios. All rights reserved. **/


#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE

#include "ICollection.h"
#include "IComparer.h"
#include <stack>

template <class T>
class PriorityQueue : public ICollection<T> {
protected:
	size_t _capacity;
	size_t _size;
	void reorderFromBottom(size_t);
	void reorderFromTop(size_t);
	IComparer<T>* _comparer;
	// Public only for testing purposes
protected:
	T* _array;
public:
	PriorityQueue() : _capacity(16), _size(0)
	{
		_array = new T[_capacity];
		_comparer = new DefaultComparer<T>();
	}
	~PriorityQueue() override {
		delete[] _array;
	}
	bool contains(T t) const override;
	void insert(T t) override;
	T remove(T t) override;
	T top() const;
	void pop();
	void mUnion(const ICollection<T>* other) override;
	void append(const ICollection<T>* other) override;
	size_t size() const override;
	void setComparer(IComparer<T>* comparer);
	bool _containsEqual(T t);
};

template <class T>
void PriorityQueue<T>::reorderFromBottom(size_t startNode) {
	size_t swapPosition = startNode;
	while (swapPosition > 1)
	{
		size_t parent = swapPosition / 2;
		if (_comparer->compare(_array[swapPosition], _array[parent]))
		{
			std::swap(_array[swapPosition], _array[parent]);
		}
		swapPosition /= 2;
	}
}

// These names are potentially misleading. This is useful only when popping top value.
template <class T>
void PriorityQueue<T>::reorderFromTop(size_t startNode) {
	while (startNode * 2 + 1 <= _size)
	{
		size_t smallerChildIndex = 0;
		smallerChildIndex = _comparer->compare(_array[startNode * 2], _array[startNode * 2 + 1]) ? startNode * 2 : startNode * 2 + 1;
		
		if (_comparer->compare(_array[smallerChildIndex], _array[startNode]))
		{
			std::swap(_array[smallerChildIndex], _array[startNode]);
		}
		startNode = smallerChildIndex;
	}
}


// This is the primary reason for not using STL priority queue
// Fuck STL, no major C++ software is using it extensively... I suppose people avoid using it for performance critical stuff,
// and most C++ code is for performance critical stuff... For other things just use Java or Python
template <class T>
bool PriorityQueue<T>::contains(T t) const {
	// Binary tree search here
	// Implemented as DFS.
	//TODO: measure performance. See what happens with BFS. See if there are any performane differences if we use plain array iteration.
	//TODO: also see what happens if we use recursion. From what I know, recursion should be slower because we have function call for every step\
	// although we probably also have similar context switch when we do {}
	std::stack<size_t> openSet;
	openSet.push(1);
	while (!openSet.empty())
	{
		size_t currentIndex = openSet.top();
		openSet.pop();
		if (t == _array[currentIndex])
		{
			return true;
		}
		if (_comparer->compare(t, _array[currentIndex]))
		{
			openSet.push(currentIndex * 2);
			openSet.push(currentIndex * 2 + 1);
		}
	}
	return false;
}

template <class T>
void PriorityQueue<T>::insert(T t) {
	if (_size + 1 >= _capacity)
	{
		// TODO: implement custom expansion policies
		_capacity *= 2;
		T* newArray = new T[_capacity];
		memcpy(newArray, _array, _size);
		delete[] _array;
		_array = newArray;
	}
	++_size;
	_array[_size] = t;
	reorderFromBottom(_size);
}

template <class T>
T PriorityQueue<T>::remove(T t) {
	return 0;
}

template <class T>
T PriorityQueue<T>::top() const {
	return _array[1];
}

template <class T>
void PriorityQueue<T>::pop() {
	_array[1] = _array[_size--];
	reorderFromTop(1);
}

template <class T>
void PriorityQueue<T>::mUnion(const ICollection<T>* other) {
	// Not implemented yet
}

template <class T>
void PriorityQueue<T>::append(const ICollection<T>* other) {
	// Not implemented yet
}

template <class T>
size_t PriorityQueue<T>::size() const {
	return _size;
}

template <class T>
void PriorityQueue<T>::setComparer(IComparer<T>* comparer) {
	if (_comparer)
	{
		delete _comparer;
	}
	_comparer = comparer;
}

// Budz, budz... Videti sta i kako sa ovim...
template <class T>
bool PriorityQueue<T>::_containsEqual(T t) {
	for (int i = 0; i < _size; ++i)
	{
		if (_array[i]->equals(t))
		{
			return true;
		}
	}
	return false;
}
#endif
