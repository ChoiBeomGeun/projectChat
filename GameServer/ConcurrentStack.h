#pragma once
#include <mutex>
#include "pch.h"
template<typename T>
class LockStack
{
public:
	LockStack() {}
	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);

		if (_stack.empty())
			return false;

		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}
private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};


template<typename T>
class LockFreeStack
{
	struct Node;
	struct CountedNodePtr
	{
		int32 externalCount = 0;
		Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value))
		{

		}

		Node* next;

		shared_ptr<T> data;
		atomic<int32>internalCount = 0;
		CountedNodePtr next;
		
	};



public:

	void Push(const T& value)
	{
		CountedNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1;
		node.ptr->next = _head;
		while (_head.compare_exchange_weak(node.ptr->next,node) == false)
		{
			
		}
	}

	bool TryPop(T& value)
	{
		CountedNodePtr oldHead = _head;
		while (true)
		{
			//������ȹ�� (externalCount�� ���������� +1�� �ְ� �̱�)
			IncreaseHeadCount(oldHead);
			// �ּ��� externalCOunt >=2 ���״� ����X (�����ϰ� ������ �� �ִ�)
			Node* ptr = oldHead.ptr;

			//�����;���
			if (ptr == nullptr)
			{
				return shared_ptr<T>();
			}
			//������ ȹ��(ptr->nexr�� head�� �ٲ�ġ�� �� �ְ� �̱�)

			if (_head.compare_exchange_weak(oldHead, ptr->next))
			{

			}
			else
			{

			}
		}
	}	

	void IncreaseHeadCount(CountedNodePtr & oldCounter)
	{
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;

			if(_head.compare_exchange_strong(oldCounter,newCounter))
			{
				break;
			}
		}
	}

private:

	atomic<CountedNodePtr> _head;

};




/* pre lockFreeStack

template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value), next(nullptr)
		{

		}

		T data;
		Node* next;
	};

public: 
	// 1) �� ��带 �����
	// 2) �� ����� next = head;
	// 3) head = �� ���
	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head;


		//if (_head == node->next)
		//{
		//	_head = node;
		//	return true;
		//}
		//else
		//{
		//	node->next = _head;
		//	return false;
		//}



		while (_head.compare_exchange_weak(node->next, node) == false)
		{

		}
		// �� ���̿� ��ġ�� ���ϸ�?
		//_head = node;
	}
	//1) head �б�
	//2) head->next �б�
	//3) head = head->next
	//4) data �����ؼ� ��ȯ
	//5) ������ ��带 ����
	bool TryPop(T& value)
	{
		++_popCount;

		Node* oldHead = _head;

		//if (_head == oldHead)
		//{
		//	_head = oldHead->next;
		//	return true;
		//}
		//else
		//{
		//	oldHead = _head;
		//	return false;
		//}


		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{

		}
		if (oldHead == nullptr)
		{
			--_popCount;
			return false;
		}
		//Exception X
		value = oldHead->data;
		TryDelete(oldHead);
		//��� ���� ����
		//delete oldHeadl;
		return true;
	}

	void TryDelete(Node * oldHead)
	{
		//�� �ܿ� ���� �ִ°�?
		if (_popCount == 1)
		{
			//�� ȥ�ڳ�?

			//�̿� ȥ���ΰ�, ���� ����� �ٸ� �����͵鵵 �����غ���
			Node* node = _pendingList.exchange(nullptr);
			if (--_popCount == 0)
			{
				// ����� �ְ� ���� -> ���� ����
				//���� �ͼ� �����, ������ �����ʹ� �и��ص� ����!
				DeleteNodes(node);
			}
			else if(node)
			{
				ChainPendingNode(node);
			}

			//�� �����ʹ� ����
			delete oldHead;
		}
		else
		{
			//���� ���������� �ٽ� ���� ���ڤ�
			ChainPendingNodeList(oldHead);
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList; 

		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{

		}
	}

	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;

		while (last->next)
		{
			last = last->next;
		}

		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}

private:

	atomic<Node*> _head;
	atomic<uint32> _popCount = 0;//pop�� �������� ������ ����
	atomic<Node*> _pendingList; //���� �Ǿ���� ����
};

*/