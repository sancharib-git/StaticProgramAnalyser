# pragma once

class ListNode {
private:
	int _statementNumber;
	ListNode* _next;
public:
	ListNode(int value);
	int getValue() {
		return _statementNumber;
	};
	friend class LinkedList;
};


class LinkedList {
private:
	int _size;
	ListNode* _head;

public:
	LinkedList() {
		_size = 0;
		_head = nullptr;
	}

	~LinkedList();

	void print();
	void removeHead();
	void removeTail();

	//remove element at that position
	void removePos(int position);

	//remove the element present in the list
	bool removeItem(int statementNumber);

	void insertHead(int statementNumber);
	void insertTail(int statementNumber);

	// throw exception, if position is outside of current list size?
	bool insertAtPos(int statementNumber, int position);

	// throw exception, if position is outside of list size?
	bool insertAtAfter(int statementNumber, int position);

	// if element don'TYPE exist, return NULL
	int findElementPos(int statementNumber);

	//return NULL if position is more than size of list
	int getElementAtIndex(int position);

	bool exist(int statementNumber);

	// if head is empty, returns NULL
	int getHeadItem();

	// check if empty, returns value is boolean, in terms of 0 and 1
	int empty();

	// if list is empty, tail item does not exist, returns NULL
	int getTailItem();

	int getSize();

	bool equals(LinkedList* secondList);

};