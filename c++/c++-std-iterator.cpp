/*
	描述：
		std::iterator 简单测试例子
*/

#include <iostream>

template<typename T>
struct ListNode {
	T value;
	ListNode* next;
	ListNode() {
		next = 0;
	}
	ListNode(T val, ListNode *p = nullptr) :
		value(val), next(p) {
	}
};

template<typename T>
class List {
private:
	ListNode<T> *m_pHead;
	ListNode<T> *m_pTail;
	int m_nSize;
public:
	List() {
		m_pHead = nullptr;
		m_pTail = nullptr;
		m_nSize = 0;
	}
	//从链表尾部插入元素
	void push_back(T value) {
		if (m_pHead == nullptr) {
			m_pHead = new ListNode<T>(value);
			m_pTail = m_pHead;
		}
		else {
			m_pTail->next = new ListNode<T>(value);
			m_pTail = m_pTail->next;
		}

	}

	//打印链表元素
	void print(std::ostream &os = std::cout) const {
		for (ListNode<T> *ptr = m_pHead; ptr != m_pTail->next; ptr = ptr->next)
			std::cout << ptr->value << " ";
		os << std::endl;
	}

	//内置迭代器
	class iterator {
	private:
		ListNode<T> *m_ptr;
	public:
		iterator(ListNode<T>* p = nullptr) :
			m_ptr(p) {
		}

		T operator*() const {
			return m_ptr->value;
		}
		ListNode<T>* operator->() const {
			return m_ptr;
		}
		iterator& operator++() {
			m_ptr = m_ptr->next;
			return *this;
		}
		iterator operator++(int) {
			ListNode<T>* tmp = m_ptr;
			m_ptr = m_ptr->next;
			return iterator(tmp);
		}

		bool operator==(const iterator &arg) const {
			return arg.m_ptr == this->m_ptr;
		}

		bool operator!=(const iterator &arg) const {
			return arg.m_ptr != this->m_ptr;
		}

	};

	//返回链表头部指针
	iterator begin() const {
		return iterator(m_pHead);
	}

	//返回链表尾部指针
	iterator end() const {
		return iterator(m_pTail->next);
	}

	//其它成员函数

};

int main() {
	List<int> l;
	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.print();
	for (List<int>::iterator it = l.begin(); it != l.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	return 0;
}


