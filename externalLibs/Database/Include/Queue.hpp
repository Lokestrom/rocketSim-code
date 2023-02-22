/*
Athor: Loke Strøm
*/
#pragma once

namespace Database {
	template <typename T>
	struct node;

	template <typename T>
	class Queue {
	private:
		node<T>* front = nullptr;
		node<T>* back = nullptr;
	public:
		Queue();
		~Queue();

		void pushOn(T val);
		T pushOff();

		unsigned int size();
		bool empty();
		void clear();
	};
}

#include "TeplateFiles/Queue.tpp"