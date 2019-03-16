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

/** ���ο� Node�� Pointer�� �޾Ƽ� ���� Node�� Insert�� */
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
	LinkedListLength++;
}

/** �ڽ��� Node�� ������ ������ ��� Node�� �Ҹ��Ŵ */
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
		// ���� Node�� DataSet Node�� �ݳ���
		DataSetPtr->DestroyNode();
	}


#if ALINKEDLIST_DEBUG
	cout << "Destroy the ALikedList Node : " << this << endl;
#endif
	// ���� Node�� �ݳ���
	delete this;
}

/** ���� Node�� DataSet Pointer�� ��ȯ�� */
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

/** ���� Node�� �������� LinkedList�� ���̸� ��ȯ�մϴ�. */
int ALinkedList::GetLinkedListLength()
{
	return LinkedListLength;
}