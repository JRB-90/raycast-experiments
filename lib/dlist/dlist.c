#include "dlist.h"
#include <stdlib.h>

LList CreateLinkedList()
{
	LList list =
	{
		.head = NULL,
		.tail = NULL,
		.size = 0
	};

	return list;
}

void DestroyLinkedList(LList* list)
{
	LLNode* current = list->head;

	if (current != NULL)
	{
		while (current != NULL)
		{
			LLNode* next = current->next;
			free(current);
			current = next;
		}
	}

	free(list);
}

void PushLLNode(LList* list, void* data)
{
	LLNode* tail = list->tail;
	LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
	newNode->data = data;
	newNode->next = NULL;

	if (list->tail == NULL)
	{
		list->head = newNode;
	}
	else
	{
		tail->next = newNode;
	}
	
	list->size = list->size + 1;
	list->tail = newNode;
}

void* PopLLNode(LList* list)
{
	if (list->head == NULL)
	{
		return NULL;
	}

	LLNode* tail = list->tail;
	LLNode* current = list->head;

	if (tail == current)
	{
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
	else
	{
		while (current->next != tail)
		{
			current = current->next;
		}

		list->tail = current;
		list->size = list->size - 1;
		current = current->next;
	}

	void* data = current->data;
	free(current);

	return data;
}

void* LLAt(LList* list, uint32_t index)
{
	if (index + 1 > list->size)
	{
		return NULL;
	}
	
	LLNode* node = list->head;

	for (int i = 0; i < index; i++)
	{
		node = node->next;
	}

	return node->data;
}

DLList CreateDoubleLinkedList()
{
	DLList list =
	{
		.head = NULL,
		.tail = NULL,
		.size = 0
	};

	return list;
}

void DestroyDoubleLinkedList(DLList* list)
{
	DLLNode* current = list->head;

	if (current != NULL)
	{
		while (current != NULL)
		{
			DLLNode* next = current->next;
			free(current);
			current = next;
		}
	}

	free(list);
}

void PushDLLNode(DLList* list, void* data)
{
	DLLNode* tail = list->tail;
	DLLNode* newNode = (DLLNode*)malloc(sizeof(DLLNode));
	newNode->data = data;
	newNode->next = NULL;
	newNode->prev = NULL;

	if (list->tail == NULL)
	{
		list->head = newNode;
	}
	else
	{
		tail->next = newNode;
		newNode->prev = tail;
	}

	list->size = list->size + 1;
	list->tail = newNode;
}

void* PopDLLNode(DLList* list)
{
	if (list->head == NULL)
	{
		return NULL;
	}

	DLLNode* oldTtail = list->tail;

	if (oldTtail == list->head)
	{
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
	else
	{
		DLLNode* newTail = oldTtail->prev;
		list->tail = newTail;
		list->size = list->size - 1;
		newTail->next = NULL;
	}

	void* data = oldTtail->data;
	free(oldTtail);

	return data;
}

void* DLLAt(DLList* list, uint32_t index)
{
	if (index + 1 > list->size)
	{
		return NULL;
	}

	DLLNode* node = list->head;

	for (int i = 0; i < index; i++)
	{
		node = node->next;
	}

	return node->data;
}
