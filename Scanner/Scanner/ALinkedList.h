#pragma once

#define ALINKEDLIST_DEBUG true

class ALinkedList
{
public:
	ALinkedList();
	~ALinkedList();

	/** 새로운 Node의 Pointer를 받아서 다음 Node에 Insert함 */
	void InsertNextNode(ALinkedList* NewNodePtr);

	/** 자신의 Node를 포함한 하위의 모든 Node를 소멸시킴 */
	void DestroyNode();

	/** 현재 Node의 DataSet Pointer를 반환함 */
	class AScanDataSet* GetDataSetPtr();
	
	/** 다음 Node의 Pointer를 반환함 */
	ALinkedList* GetNextNodePtr();
private:
	/** 다음 Node의 위치를 기록할 Pointer */
	ALinkedList* NextNodePtr;
	/** 현재 Node의 Data들을 저장할 class Pointer */
	class AScanDataSet* DataSetPtr;
};