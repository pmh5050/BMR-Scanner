#pragma once

#define ALINKEDLIST_DEBUG true

class ALinkedList
{
public:
	ALinkedList();
	~ALinkedList();

	/** ���ο� Node�� Pointer�� �޾Ƽ� ���� Node�� Insert�� */
	void InsertNextNode(ALinkedList* NewNodePtr);

	/** �ڽ��� Node�� ������ ������ ��� Node�� �Ҹ��Ŵ */
	void DestroyNode();

	/** ���� Node�� DataSet Pointer�� ��ȯ�� */
	class AScanDataSet* GetDataSetPtr();
	
	/** ���� Node�� Pointer�� ��ȯ�� */
	ALinkedList* GetNextNodePtr();
private:
	/** ���� Node�� ��ġ�� ����� Pointer */
	ALinkedList* NextNodePtr;
	/** ���� Node�� Data���� ������ class Pointer */
	class AScanDataSet* DataSetPtr;
};