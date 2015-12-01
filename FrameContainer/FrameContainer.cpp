#include "FrameContainer.h"

FrameContainer::FrameContainer(QPixmap &frame, int frameNumber) {
    setFrame(frame);
    setFrameNumber(frameNumber);
}

FrameContainer::~FrameContainer() {

}

void FrameContainer::init(void) {
    QPixmap tmp(VIDEO_VIEEWER_WIDTH, VIDEO_VIEEWER_HEIGHT);
    QPainter painter(&tmp);
    painter.drawText(QPoint(320, 240), QString("Not Initial Frame"));
    setFrame(tmp);
    frameNumber = -1;
}

void FrameContainer::setFrame(const QPixmap& frame) {
    if (frame.isNull()) {
        qDebug("Not Valid Frame");
        return;
    }
    this->frame = frame.copy();
}

QPixmap FrameContainer::getFrame(void) const {
    if (!frame.isNull()) {
        return frame.copy();
    } else {
        return QPixmap(VIDEO_VIEEWER_WIDTH, VIDEO_VIEEWER_HEIGHT);
    }
}

void FrameContainer::setFrameNumber(int frameNumber) {
    if (frameNumber < 0) {
        //cout << "Invalid Frame Number" << endl;
        return;
    }
    this->frameNumber = frameNumber;
}

int FrameContainer::getFrameNumber(void) const {
    return frameNumber;
}

FrameContainer* FrameContainer::getPreFrameContainer() {
    return (FrameContainer*)getPreNode();
}

FrameContainer* FrameContainer::getNextFrameContainer() {
    return (FrameContainer*)getNextNode();
}

void FrameContainer::push(FrameContainer* newFrameContainer) {
    ((LinkedListNode*)(this))->push(newFrameContainer);
}
