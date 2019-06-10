#pragma once

#define ALINKEDLIST_DEBUG false;

class ALinkedList
{
public:
	ALinkedList();

	/** ���ο� Node�� Pointer�� �޾Ƽ� ���� Node�� Insert�� */
	void InsertNextNode(ALinkedList* NewNodePtr);
	/** �ڽ��� Node�� ������ ������ ��� Node�� �Ҹ��Ŵ */
	void DestroyNode();

	/** Accessor funtion */

	/** ���� Node�� DataSet Pointer�� ��ȯ�� */
	class AScanDataSet* GetDataSetPtr();
	/** ���� Node�� Pointer�� ��ȯ�� */
	ALinkedList* GetNextNodePtr();
	/** ���� Node�� �������� LinkedList�� ���̸� ��ȯ�մϴ�. */
	int GetLinkedListLength();

private:
	ALinkedList* NextNodePtr; // ���� Node�� ��ġ�� ����� Pointer
	class AScanDataSet* DataSetPtr; // ���� Node�� Data���� ������ class Pointer
	int LinkedListLength; // ���� Node�� �����Ͽ� �� List�� ���̸� ������ ����

};