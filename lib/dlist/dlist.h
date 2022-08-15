#ifndef _DLIST_H_
#define _DLIST_H_

#include <stdint.h>

typedef struct LLNode {
	void* data;
	struct SLNode* next;
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

extern LList* CreateLinkedList();
extern void DestroyLinkedList(LList* list);
extern void PushLLNode(LList* list, void* data);
extern void* PopLLNode(LList* list);
extern void* LLAt(LList* list, uint32_t index);

extern DLList* CreateDoubleLinkedList();
extern void DestroyDoubleLinkedList(DLList* list);
extern void PushDLLNode(DLList* list, void* data);
extern void* PopDLLNode(DLList* list);
extern void* DLLAt(DLList* list, uint32_t index);

#endif // !_DLIST_H_
