/*
Athor: Loke Strøm
*/
#include "Exception.hpp"

namespace Database
{
	template <typename T>
	struct node {
		T val = 0;
		node<T>* next = nullptr;
	};

	template <typename T>
	Queue<T>::Queue() {
		front = nullptr;
		back = nullptr;
	}

	template <typename T>
	Queue<T>::~Queue() {
		if (front == nullptr)
			return;
		node<T>* lastNode = front;
		for (node<T>* i = front->next; i != back; i = i->next) {
			delete lastNode;
			lastNode = i;
		}
		delete back;
	}

	template <typename T>
	void Queue<T>::pushOn(T val)
	{
		if (front == nullptr) {
			front = new node<T>();
			front->val = val;
			return;
		}
		if (back == nullptr) {
			back = new node<T>();
			back->val = val;
			front->next = back;
			return;
		}
		node<T>* newNode = new node<T>();
		newNode->val = val;
		newNode->next = nullptr;
		back->next = newNode;
		back = newNode;
	}

	template <typename T>
	T Queue<T>::pushOff()
	{
		if (front == nullptr)
			throw LengthError("Can't pushOff an empty queue");

		T returnVal = front->val;
		node<T>* newNode = front->next;
		delete front;
		front = newNode;
		if (back == front) {
			back = nullptr;
		}
		return returnVal;
	}

	template <typename T>
	unsigned int Queue<T>::size()
	{
		unsigned int count = 0;
		for (node<T>* i = front; i != back; i = i->next)
			count++;
		return count;
	}

	template <typename T>
	bool Queue<T>::empty()
	{
		if (front == nullptr)
			return true;
		return false;
	}

	template <typename T>
	void Queue<T>::clear()
	{
		node<T> lastNode = nullptr;
		for (node<T>* i = front; i != back; i = i->next) {
			if (lastNode != nullptr)
				delete lastNode;
			lastNode = i;
		}
		delete back;
	}
}



