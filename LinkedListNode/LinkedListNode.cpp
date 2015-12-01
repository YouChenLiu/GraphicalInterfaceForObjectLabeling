#include "LinkedListNode.h"

LinkedListNode::LinkedListNode(void) {
    init();
}

LinkedListNode::~LinkedListNode() {
    deleteNode();
}

void LinkedListNode::init(void) {
    setPreNode(NULL);
    setNextNode(NULL);
}

void LinkedListNode::setPreNode(LinkedListNode* preNode) {
    this->preNode = preNode;
}

LinkedListNode* LinkedListNode::getPreNode(void) {
    return preNode;
}

void LinkedListNode::setNextNode(LinkedListNode* nextNode) {
    this->nextNode = nextNode;
}

LinkedListNode* LinkedListNode::getNextNode(void) {
    return nextNode;
}

void LinkedListNode::addNode(LinkedListNode* newNode) {
    LinkedListNode* next = getNextNode();
    if (next) {
        newNode->setNextNode(next);
        next->setPreNode(newNode);
    }
    setNextNode(newNode);
    newNode->setPreNode(this);
}

void LinkedListNode::deleteNode(void) {
    LinkedListNode* pre = getPreNode();
    LinkedListNode* next = getNextNode();
    if (pre) {
        pre->setNextNode(getNextNode());
    }
    if (next) {
        next->setPreNode(getPreNode());
    }
}

void LinkedListNode::push(LinkedListNode* newNode) {
    LinkedListNode* lastNode = this;
    while (lastNode->getNextNode()) {
        lastNode = lastNode->getNextNode();
    }
    lastNode->addNode(newNode);
}

LinkedListNode* LinkedListNode::pop(void) {
    LinkedListNode* firstNode = this;
    while (firstNode->getPreNode()) {
        firstNode = firstNode->getPreNode();
    }
    LinkedListNode* next = firstNode->getNextNode();
    if (next) {
        next->setPreNode(NULL);
    }
    firstNode->setNextNode(NULL);
    return firstNode;
}