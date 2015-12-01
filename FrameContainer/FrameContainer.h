#ifndef _FRAMECONTAINER_H_
#define _FRAMECONTAINER_H_

#include "..\common.h"
#include "..\LinkedListNode\LinkedListNode.h"

#include <QPixmap>
#include <QPainter>

class FrameContainer : protected LinkedListNode {
private:
    QPixmap frame;
    int frameNumber;

    void setFrame(const QPixmap& frame);
    void setFrameNumber(int frameNumber);
    void init(void);
public:
    FrameContainer(QPixmap &frame, int frameNumber);
    virtual ~FrameContainer();
    QPixmap getFrame(void) const;
    int getFrameNumber(void) const;
    void push(FrameContainer* newFrameContainer);
    FrameContainer* getPreFrameContainer();
    FrameContainer* getNextFrameContainer();
};

#endif
