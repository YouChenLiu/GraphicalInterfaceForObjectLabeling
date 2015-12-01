#include "RectangularRegion.h"

RectangularRegion::RectangularRegion(int serialNumber, QPoint leftTop, QPoint rightBottom, Shapes shape) : Region(serialNumber, leftTop, rightBottom.y() - leftTop.y(), rightBottom.x() - leftTop.x(), shape){
    setLeftTopPoint(leftTop);
    setRightBottomPoint(rightBottom);
    clearIsAllSelected();
}

RectangularRegion::RectangularRegion(int serialNumber, QPoint leftTop, int width, int height, Shapes shape) : Region(serialNumber, leftTop, height, width, shape){
    setLeftTopPoint(leftTop);
    setRightBottomPoint(QPoint(leftTop.x() + width, leftTop.y() + height));
}

RectangularRegion::RectangularRegion(RectangularRegion &region) : Region(region) {
    setLeftTopPoint(region.getStartPoint());
    setRightBottomPoint(region.getStartPoint(), region.getWidth(), region.getHeight());
}

RectangularRegion::RectangularRegion(RectangularRegion *region) : Region(region){
    setLeftTopPoint(region->getStartPoint());
    setRightBottomPoint(region->getStartPoint(), region->getWidth(), region->getHeight());
}

void RectangularRegion::init(void){

}

void RectangularRegion::setLeftTopPoint(QPoint leftTopPoint){
    this->leftTopPoint.setX(leftTopPoint.x());
    this->leftTopPoint.setY(leftTopPoint.y());
}

QPoint RectangularRegion::getLeftTopPoint(void){
    return leftTopPoint;
}

void RectangularRegion::setRightBottomPoint(QPoint rightBottomPoint){
    this->rightBottomPoint.setX(rightBottomPoint.x());
    this->rightBottomPoint.setY(rightBottomPoint.y());
}

void RectangularRegion::setRightBottomPoint(QPoint startPoint, int width, int height) {
    this->rightBottomPoint.setX(startPoint.x() + width);
    this->rightBottomPoint.setY(startPoint.y() + height);
}

QPoint RectangularRegion::getRightBottomPoint(void){
    return rightBottomPoint;
}

void RectangularRegion::modify(int distanceX, int distanceY) {
    if (getIsAllSelected()) {
        move(distanceX, distanceY);
    } else if (getIsTopSelected()) {
        move(0, distanceY);
        modifyHeight(-distanceY);
    } else if (getIsBottomSelected()) {
        modifyHeight(distanceY);
    } else if (getIsLeftSelected()) {
        move(distanceX, 0);
        modifyWidth(-distanceX);
    } else if(getIsRightSelected()) {
        modifyWidth(distanceX);
    }
}

void RectangularRegion::move(int distanceX, int distanceY) {
    Region::move(distanceX, distanceY);
    setLeftTopPoint(getStartPoint());
    setRightBottomPoint(QPoint(getStartPoint().x() + getWidth(), getStartPoint().y() + getHeight()));
}

void RectangularRegion::modifyHeight(int distance) {
    Region::modifyHeight(distance);
    setLeftTopPoint(getStartPoint());
    setRightBottomPoint(QPoint(getStartPoint().x() + getWidth(), getStartPoint().y() + getHeight()));
}

void RectangularRegion::modifyWidth(int distance) {
    Region::modifyWidth(distance);
    setLeftTopPoint(getStartPoint());
    setRightBottomPoint(QPoint(getStartPoint().x() + getWidth(), getStartPoint().y() + getHeight()));
}

bool RectangularRegion::checkAmbit(QPoint point){
    bool result = false;
    QPoint leftTop = getLeftTopPoint();
    QPoint rightBottom = getRightBottomPoint();

    if (leftTop.x() <= point.x() && point.x() <= rightBottom.x() &&
        leftTop.y() <= point.y() && point.y() <= rightBottom.y()) {
        setIsSelected();
        result = true;
    } else{
        clearIsSelected();
    }

    return result;
}

void RectangularRegion::setIsAllSelected(void) {
    Region::setIsAllSelected();
    setIsTopSelected();
    setIsBottomSelected();
    setIsLeftSelected();
    setIsRightSelected();
}

void RectangularRegion::clearIsAllSelected(void) {
    Region::clearIsAllSelected();
    clearIsBottomSelected();
    clearIsLeftSelected();
    clearIsRightSelected();
    clearIsTopSelected();
}

void RectangularRegion::setIsTopSelected(void) {
    isTopSelected = true;
}

void RectangularRegion::clearIsTopSelected(void) {
    isTopSelected = false;
}

bool RectangularRegion::getIsTopSelected(void) {
    return isTopSelected;
}

void RectangularRegion::setIsBottomSelected(void) {
    isBottomSelected = true;
}

void RectangularRegion::clearIsBottomSelected(void) {
    isBottomSelected = false;
}

bool RectangularRegion::getIsBottomSelected(void) {
    return isBottomSelected;
}

void RectangularRegion::setIsLeftSelected(void) {
    isLeftSelected = true;
}

void RectangularRegion::clearIsLeftSelected(void) {
    isLeftSelected = false;
}

bool RectangularRegion::getIsLeftSelected(void) {
    return isLeftSelected;
}

void RectangularRegion::setIsRightSelected(void) {
    isRightSelected = true;
}

void RectangularRegion::clearIsRightSelected(void) {
    isRightSelected = false;
}

bool RectangularRegion::getIsRightSelected(void) {
    return isRightSelected;
}

bool RectangularRegion::hitModifyPoints(const QPoint &point) {
    bool result = hitCenterPoint(point);
    if (!result) {
        result = hitTopModifyPoint(point) | hitBottomModifyPoint(point) |
                 hitLeftModifyPoint(point) | hitRightModifyPoint(point);
    }
    return result;
}

bool RectangularRegion::hitTopModifyPoint(const QPoint &point) {
    bool result = false;
    QPoint topModifyPoint(getStartPoint().x() + getWidth() / 2, getStartPoint().y());
    if (Region::distance(point, topModifyPoint) <= SQUARE(MODIFY_POINT_RADIUS)){
        result = true;
        setIsTopSelected();
    } else {
        clearIsTopSelected();
    }
    return result;
}

bool RectangularRegion::hitBottomModifyPoint(const QPoint &point) {
    bool result = false;
    QPoint bottomModifyPoint(getStartPoint().x() + getWidth() / 2, getStartPoint().y() + getHeight());
    if (Region::distance(point, bottomModifyPoint) <= SQUARE(MODIFY_POINT_RADIUS)){
        result = true;
        setIsBottomSelected();
    } else {
        clearIsBottomSelected();
    }
    return result;
}

bool RectangularRegion::hitLeftModifyPoint(const QPoint &point) {
    bool result = false;
    QPoint leftModifyPoint(getStartPoint().x(), getStartPoint().y() + getHeight() / 2);
    if (Region::distance(point, leftModifyPoint) <= SQUARE(MODIFY_POINT_RADIUS)) {
        result = true;
        setIsLeftSelected();
    } else {
        clearIsLeftSelected();
    }
    return result;
}

bool RectangularRegion::hitRightModifyPoint(const QPoint &point) {
    bool result = false;
    QPoint rightModifyPoint(getStartPoint().x() + getWidth(), getStartPoint().y() + getHeight() / 2);
    if (Region::distance(point, rightModifyPoint) <= SQUARE(MODIFY_POINT_RADIUS)){
        result = true;
        setIsRightSelected();
    } else {
        clearIsRightSelected();
    }
    return result;
}
