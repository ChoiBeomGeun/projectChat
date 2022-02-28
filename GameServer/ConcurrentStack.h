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
			//참조권획득 (externalCount를 현시점기준 +1한 애가 이김)
			IncreaseHeadCount(oldHead);
			// 최소한 externalCOunt >=2 일테니 삭제X (안전하게 접근할 수 있는)
			Node* ptr = oldHead.ptr;

			//데이터없음
			if (ptr == nullptr)
			{
				return shared_ptr<T>();
			}
			//소유권 획득(ptr->nexr로 head를 바꿔치기 한 애가 이김)

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
	// 1) 새 노드를 만들고
	// 2) 새 노드의 next = head;
	// 3) head = 새 노드
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
		// 이 사이에 새치기 당하면?
		//_head = node;
	}
	//1) head 읽기
	//2) head->next 읽기
	//3) head = head->next
	//4) data 추출해서 반환
	//5) 추출한 노드를 삭제
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
		//잠시 삭제 보류
		//delete oldHeadl;
		return true;
	}

	void TryDelete(Node * oldHead)
	{
		//나 외에 누가 있는가?
		if (_popCount == 1)
		{
			//나 혼자네?

			//이왕 혼자인거, 삭제 예약된 다른 데이터들도 삭제해보자
			Node* node = _pendingList.exchange(nullptr);
			if (--_popCount == 0)
			{
				// 끼어든 애가 없음 -> 삭제 진행
				//이제 와서 끼어들어도, 어차피 데이터는 분리해둔 상태!
				DeleteNodes(node);
			}
			else if(node)
			{
				ChainPendingNode(node);
			}

			//내 데이터는 삭제
			delete oldHead;
		}
		else
		{
			//누가 끼어들었으니 다시 갖다 놓자ㅣ
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
	atomic<uint32> _popCount = 0;//pop을 실행중인 쓰레드 개수
	atomic<Node*> _pendingList; //삭제 되어야할 노드들
};

*/