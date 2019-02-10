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
	// NextNode�� Pointer�� �������� ���� ��� Insert�ϰ�, �׷��� ���� ��� NextNode�� �ּҿ��� Insert��
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
		// ���� Node�� DataSet Node�� �ݳ���
		DataSetPtr->DestroyNode();
	}


#if ALINKEDLIST_DEBUG true
	cout << "Destroy the ALikedList Node : " << this << endl;
#endif
	// ���� Node�� �ݳ���
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
		cout << "[ERROR] DataSet Pointer Ž�� ����" << endl;
#endif
		return nullptr;
	}
}

/** ���� Node�� Pointer�� ��ȯ�� */
ALinkedList* ALinkedList::GetNextNodePtr()
{
	return NextNodePtr;
}