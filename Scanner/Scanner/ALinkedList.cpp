#include "ALinkedList.h"
#include "AScanDataSet.h"

#include <iostream>
using namespace std;

ALinkedList::ALinkedList()
{
	NextNodePtr = nullptr;

	DataSetPtr = new AScanDataSet();
}

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
}

void ALinkedList::DestroyNode()
{

	if (NextNodePtr != nullptr)
	{
#if ALINKEDLIST_DEBUG true
		cout << "Call NextNode's Destroy function : " << NextNodePtr << endl;
#endif
		NextNodePtr->DestroyNode();
	}

	if (DataSetPtr != nullptr)
	{
#if ALINKEDLIST_DEBUG true
		cout << "Destroy the DataSetPtr Node : " << DataSetPtr << endl;
#endif
		// 현재 Node의 DataSet Node를 반납함
		DataSetPtr->DestroyNode();
	}


#if ALINKEDLIST_DEBUG true
	cout << "Destroy the ALikedList Node : " << this << endl;
#endif
	// 현재 Node를 반납함
	delete this;
}

ALinkedList::~ALinkedList()
{

}

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