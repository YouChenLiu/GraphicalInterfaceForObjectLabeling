#include "CircularRegion.h"

CircularRegion::CircularRegion(int serialNumber, QPoint center, int radius) : EllipticalRegion(serialNumber, center, radius, radius, Shapes::Circle) {
    setRadius(radius);
}

CircularRegion::CircularRegion(CircularRegion &region) : EllipticalRegion(&region) {
    setRadius(region.getRadius());
}

CircularRegion::CircularRegion(CircularRegion *region) : EllipticalRegion(region) {
    setRadius(region->getRadius());
}

CircularRegion::~CircularRegion() {}

void CircularRegion::init(void){

}

void CircularRegion::setRadius(int radius){
    this->radius = radius;
}

int CircularRegion::getRadius(void){
    return radius;
}

void CircularRegion::setIsAllSelected(void) {
    EllipticalRegion::setIsAllSelected();
    setIsRightBottomSelected();
}

void CircularRegion::clearIsAllSelected(void) {
    EllipticalRegion::clearIsAllSelected();
    clearIsRightBottomSelected();
}

void CircularRegion::setIsRightBottomSelected(void) {
    isRightBottomSelected = true;
}

void CircularRegion::clearIsRightBottomSelected(void) {
    isRightBottomSelected = false;
}

bool CircularRegion::getIsRightBottomSelected(void) {
    return isRightBottomSelected;
}

void CircularRegion::modify(int distanceX, int distanceY) {
    if (getIsAllSelected()) {
        move(distanceX, distanceY);
    } else if (getIsRightBottomSelected()) {
        modifyRadius(distanceX + distanceY);
    }
}

void CircularRegion::move(int distanceX, int distanceY) {
    EllipticalRegion::move(distanceX, distanceY);
}

void CircularRegion::modifyRadius(int distance) {
    EllipticalRegion::modifyHeight(distance);
    EllipticalRegion::modifyWidth(distance);
    setRadius(getRadius() + distance);
}

bool CircularRegion::checkAmbit(QPoint point){
    bool result = false;
    if (distance(point, getCenterPoint()) <= SQUARE(getRadius())) {
        result = true;
        setIsSelected();
    } else {
        clearIsSelected();
    }
    
    return result;
}

bool CircularRegion::hitModifyPoints(const QPoint &point) {
    bool result = EllipticalRegion::hitCenterPoint(point);
    if (!result) {
        result = hitRightBottomModifyPoint(point);
    }
    return result;
}

bool CircularRegion::hitRightBottomModifyPoint(const QPoint &point) {
    bool result = false;
    QPoint center = getCenterPoint();
    int radius = getRadius();
    QPoint rightBottomModifyPoint(center.x() + (int)(radius * cos(45 * PI / 180)),
                                  center.y() + (int)(radius * sin(45 * PI / 180)));    //右下角的調整點座標
    if (Region::distance(point, rightBottomModifyPoint) <= SQUARE(MODIFY_POINT_RADIUS)) {
        result = true;
        setIsRightBottomSelected();
    } else {
        clearIsRightBottomSelected();
    }
    return result;
}
