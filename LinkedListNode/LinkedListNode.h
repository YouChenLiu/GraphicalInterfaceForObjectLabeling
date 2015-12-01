#ifndef _LINKEDLISTNODE_H_
#define _LINKEDLISTNODE_H_

#include "..\common.h"

class LinkedListNode : QObject {
private:
    LinkedListNode* preNode;
    LinkedListNode* nextNode;

    void init(void);
    void deleteNode(void);
public:
    LinkedListNode();
    virtual ~LinkedListNode();

    void setPreNode(LinkedListNode* node);
    LinkedListNode* getPreNode(void);
    void setNextNode(LinkedListNode* node);
    LinkedListNode* getNextNode(void);
    void addNode(LinkedListNode* newNode);
    void push(LinkedListNode* newNode);
    LinkedListNode* pop(void);
};

#endif

