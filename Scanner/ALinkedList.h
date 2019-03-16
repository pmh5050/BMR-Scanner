#pragma once

#define ALINKEDLIST_DEBUG false;

class ALinkedList
{
public:
	ALinkedList();

	/** 새로운 Node의 Pointer를 받아서 다음 Node에 Insert함 */
	void InsertNextNode(ALinkedList* NewNodePtr);
	/** 자신의 Node를 포함한 하위의 모든 Node를 소멸시킴 */
	void DestroyNode();

	/** Accessor funtion */

	/** 현재 Node의 DataSet Pointer를 반환함 */
	class AScanDataSet* GetDataSetPtr();
	/** 다음 Node의 Pointer를 반환함 */
	ALinkedList* GetNextNodePtr();
	/** 현재 Node를 기준으로 LinkedList의 길이를 반환합니다. */
	int GetLinkedListLength();

private:
	ALinkedList* NextNodePtr; // 다음 Node의 위치를 기록할 Pointer
	class AScanDataSet* DataSetPtr; // 현재 Node의 Data들을 저장할 class Pointer
	int LinkedListLength; // 현재 Node를 포함하여 총 List의 길이를 저장할 변수

};