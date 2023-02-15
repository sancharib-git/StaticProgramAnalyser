#include <iostream>
#include "LinkedList.h"
using namespace std;

ListNode::ListNode(int n) {
	_statementNumber = n;
	_next = NULL;
}

LinkedList::~LinkedList() {
	while (_head) {
		removeHead();
	}
}

void LinkedList::removeHead() {
	if (_size > 0) {
		ListNode* temp = _head;
		_head = _head->_next;
		delete temp;
		_size--;
	}
}

void LinkedList::insertHead(int statementNumber) {
	ListNode* newNode = new ListNode(statementNumber);
	if (!(_head)) {
		_head = newNode;
		_size++;
		return;
	}
	newNode->_next = _head;
	_head = newNode;
	_size++;
}

void LinkedList::insertTail(int statementNumber) {
	ListNode* newNode = new ListNode(statementNumber);
	if (this->_head == nullptr) {
		this->_head = newNode;
		this->_size++;
		return;
	}
	ListNode* ptr = _head;
	while (ptr) {
		// if next node is null, it is the last element in the linked list, new node should be inserted here.
		if (!(ptr->_next)) {
			ptr->_next = newNode;
			break;
		}
		ptr = ptr->_next;
	}
	_size++;
	return;
}

bool LinkedList::insertAtPos(int statementNumber, int position) {
	ListNode* newNode = new ListNode(statementNumber);
	if (position == 0) {
		newNode->_next = _head;
		_head = newNode;
		_size++;
		return true;
	}

	if (position > (_size - 1)) {
		return false;
	}

	ListNode* curr = _head;
	ListNode* prev = _head;
	for (int i = 0; i < position; i++) {
		prev = curr;
		curr = curr->_next;
	}
	prev->_next = newNode;
	newNode->_next = curr;
	_size++;
	return true;
}

bool LinkedList::insertAtAfter(int statementNumber, int position) {
	ListNode* newNode = new ListNode(statementNumber);

	if (!_head) {
		if (position == 0) {
			_head = newNode;
			_size++;
			return true;
		}
		return false;
	}

	// insert after head
	if (position == 0) {
		if (_head->_next) {
			newNode->_next = _head->_next;
			_head->_next = newNode;
			_size++;
			return true;
		}
		_head = newNode;
		_size++;
		return true;
	}

	if (position > (_size - 1)) {
		return false;
	}
	
	ListNode* curr = _head;
	ListNode* prev = _head;
	for (int i = 0; i <= position; i++) {
		prev = curr;
		curr = curr->_next;
	}
	// insert after last element in current list
	if (!curr) {
		prev->_next = newNode;
		_size++;
		return true;
	}
	prev->_next = newNode;
	newNode->_next = curr;
	_size++;
	return true;
};

int LinkedList::findElementPos(int statementNumber) {
	int position = 0;
	ListNode* ptr;
	for (ptr = _head; ptr; ptr = ptr->_next) {
		if ((ptr->_statementNumber) == statementNumber) {
			return position;
		}
		position += 1;
	}
	return -1; // changed NUll to -1, consider throw error
};

int LinkedList::getElementAtIndex(int position) {
	int element = 0;
	ListNode* curr = _head;
	if (position > (_size - 1)) {
		return -1; // changed NUll to -1, consider throw error
	}
	for (int i = 0; i < position; i++) {
		curr = curr->_next;
	}
	return curr->_statementNumber;
};

void LinkedList::print() {
	ListNode* ptr;
	for (ptr = _head; ptr; ptr = ptr->_next) {
		cout << ptr->_statementNumber << " ";
	}
	cout << endl;
}

bool LinkedList::exist(int n) {
	ListNode* ptr = _head;
	while (ptr) {
		if ((ptr->_statementNumber) == n) {
			return 1;
		}
		ptr = ptr->_next;
	}
	return 0;
}

int LinkedList::getHeadItem()
{
	if (_head) {
		return _head->_statementNumber;
	}
	return -1; // changed NUll to -1, consider throw error
}

int LinkedList::empty() {
	if (_head) {
		return 0;
	}
	return 1;
}

int LinkedList::getTailItem() {
	if (_head) {
		ListNode* ptr = _head;
		while (ptr) {
			if ((ptr->_next) != NULL) {
				ptr = ptr->_next;
			}
			else {
				return ptr->_statementNumber;
			}
		}
	}
	return -1; // changed NUll to -1, consider throw error
}

void LinkedList::removeTail() {
	if (_head && (_head->_next)) {
		ListNode* ptrc = _head;
		ListNode* ptrprev = NULL;
		while (ptrc) {
			if ((ptrc->_next)) {
				ptrprev = ptrc;
				ptrc = ptrc->_next;
			} else {
				ListNode* temp = ptrc;
				ptrprev->_next = NULL;
				delete temp;
				_size--;
				break;
			}
		}
	} else if (_head && ((_head->_next) == NULL)) {
		// if list only has 1 element
		_head = NULL;
		_size--;
	}
}

void LinkedList::removePos(int position) {
	if (_size == 0) {
		return;
	}

	if (position > _size) {
		return;
	}

	// list only has 1 element
	if (position == 0) {
		ListNode* temp = _head;
		if (_size > 1) {
			_head = _head->_next;
			delete temp;
			_size--;
			return;
		}
		_head = NULL;
		delete temp;
		_size--;
		return;
	}
	ListNode* curr = _head;
	ListNode* prev = _head;
	for (int i = 0; i < position; i++) {
		prev = curr;
		curr = curr->_next;
	}
	// removing last element
	if (!(curr->_next)) {
		prev->_next = NULL;
		delete curr;
		_size--;
		return;
	}
	prev->_next = curr->_next;
	delete curr;
	_size--;
	return;
};

bool LinkedList::removeItem(int statementNumber) {
	// list is empty
	if (_size == 0) {
		return false;
	}
	ListNode* curr = _head;
	ListNode* prev = _head;
	for (int i = 0; i < _size; i++) {
		if ((curr->_statementNumber) == statementNumber) {
			// removing first element
			if (i == 0) {
				ListNode* temp = curr;
				_head = _head->_next;
				delete temp;
				_size--;
			}
			// removing last element
			if (!(curr->_next)) {
				ListNode* temp = curr;
				prev->_next = NULL;
				delete temp;
				_size--;
				return true;
			}
			ListNode* temp = curr;
			prev->_next = curr->_next;
			delete temp;
			_size--;
			return true;
		}
		prev = curr;
		curr = curr->_next;
	}
	return false;
};

int LinkedList::getSize() {
	return _size;
};

bool LinkedList::equals(LinkedList* secondList) {
	if (this->getSize() != secondList->getSize()) {
		return false;
	}
	ListNode* firstListNode = this->_head;
	ListNode* secondListNode = secondList->_head;

	for (int i = 0; i < this->getSize(); i++) {
		if (firstListNode->getValue() != secondListNode->getValue()) {
			return false;
		}
		firstListNode = firstListNode->_next;
		secondListNode = secondListNode->_next;
	}
	return true;
}