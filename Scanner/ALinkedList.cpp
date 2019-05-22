#include "ALinkedList.h"
#include "AScanDataSet.h"

#include <iostream>
using namespace std;

ALinkedList::ALinkedList()
{
	NextNodePtr = nullptr;

	DataSetPtr = new AScanDataSet();

	LinkedListLength = 1;
}

/** 새로운 Node의 Pointer를 받아서 다음 Node에 Insert함 */
void ALinkedList::InsertNextNode(ALinkedList* NewNodePtr)
{
	// NextNode의 Pointer가 존재하지 않을 경우 Insert하고, 그렇지 않을 경우 NextNode의 주소에서 Insert함
	if (NextNodePtr == nullptr)
	{
#if ALINKEDLIST_DEBUG
		cout << "Insert current node : " << this << endl;
#endif
		NextNodePtr = NewNodePtr;
	}
	else
	{
#if ALINKEDLIST_DEBUG
		cout << "Jump next node : " << NextNodePtr << endl;
#endif
		NextNodePtr->InsertNextNode(NewNodePtr);
	}
	LinkedListLength++;
}

/** 자신의 Node를 포함한 하위의 모든 Node를 소멸시킴 */
void ALinkedList::DestroyNode()
{

	if (NextNodePtr != nullptr)
	{
#if ALINKEDLIST_DEBUG
		cout << "Call NextNode's Destroy function : " << NextNodePtr << endl;
#endif
		NextNodePtr->DestroyNode();
	}

	if (DataSetPtr != nullptr)
	{
#if ALINKEDLIST_DEBUG
		cout << "Destroy the DataSetPtr Node : " << DataSetPtr << endl;
#endif
		// 현재 Node의 DataSet Node를 반납함
		DataSetPtr->DestroyNode();
	}


#if ALINKEDLIST_DEBUG
	cout << "Destroy the ALikedList Node : " << this << endl;
#endif
	// 현재 Node를 반납함
	delete this;
}

/** 현재 Node의 DataSet Pointer를 반환함 */
class AScanDataSet* ALinkedList::GetDataSetPtr()
{
	if (DataSetPtr != nullptr)
	{
		return DataSetPtr;
	}
	else
	{
#if ALINKEDLIST_DEBUG
		cout << "[ERROR] DataSet Pointer 탐색 실패" << endl;
#endif
		return nullptr;
	}
}

/** 다음 Node의 Pointer를 반환함 */
ALinkedList* ALinkedList::GetNextNodePtr()
{
	return NextNodePtr;
}

/** 현재 Node를 기준으로 LinkedList의 길이를 반환합니다. */
int ALinkedList::GetLinkedListLength()
{
	return LinkedListLength;
}