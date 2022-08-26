#ifndef _DLIST_H_
#define _DLIST_H_

#include <stdint.h>

typedef struct LLNode {
	void* data;
	struct LLNode* next;
} LLNode;

typedef struct LList {
	LLNode* head;
	LLNode* tail;
	uint32_t size;
} LList;

typedef struct DLLNode {
	void* data;
	struct DLLNode* next;
	struct DLLNode* prev;
} DLLNode;

typedef struct DLList {
	DLLNode* head;
	DLLNode* tail;
	uint32_t size;
} DLList;

extern LList CreateLinkedList();
extern void ClearLinkedList(LList* const list);
extern void PushLLNode(LList* const list, void* data);
extern void* PopLLNode(LList* const list);
extern void* LLAt(LList* const list, uint32_t index);

extern DLList CreateDoubleLinkedList();
extern void ClearDoubleLinkedList(DLList* const list);
extern void PushDLLNode(DLList* const list, void* data);
extern void* PopDLLNode(DLList* const list);
extern void* DLLAt(DLList* const list, uint32_t index);

#endif // !_DLIST_H_
