#include "EllipticalRegion.h"

EllipticalRegion::EllipticalRegion(int serialNumber, QPoint center, int majorAxis, int minorAxis, Shapes shape) : Region(serialNumber, center, minorAxis, majorAxis, shape) {
    setCenter(center);
    setMajorAxis(majorAxis);
    setMinorAxis(minorAxis);
    init();

}

EllipticalRegion::EllipticalRegion(EllipticalRegion &region) : Region(region.getSerialNumber(), region.getCenterPoint(), region.getMajorAxis(), region.getMinorAxis(), region.getShape()) {
    setCenter(region.getCenterPoint());
    setMajorAxis(region.getMajorAxis());
    setMinorAxis(region.getMinorAxis());
    init();
}

EllipticalRegion::EllipticalRegion(EllipticalRegion *region) : Region(region->getSerialNumber(), region->getCenterPoint(), region->getMajorAxis(), region->getMinorAxis(), region->getShape()) {
    setCenter(region->getCenterPoint());
    setMajorAxis(region->getMajorAxis());
    setMinorAxis(region->getMinorAxis());
    init();
}

EllipticalRegion::~EllipticalRegion() {}

void EllipticalRegion::init(void) {

}

void EllipticalRegion::setCenter(QPoint center) {
    this->center.setX(center.x());
    this->center.setY(center.y());
}

void EllipticalRegion::setMajorAxis(int majorAxis) {
    this->majorAxis = majorAxis;
}

int EllipticalRegion::getMajorAxis(void) {
    return majorAxis;
}

void EllipticalRegion::setMinorAxis(int minorAxis) {
    this->minorAxis = minorAxis;
}

int EllipticalRegion::getMinorAxis(void) {
    return minorAxis;
}

QSize EllipticalRegion::getSize(void) {
    return QSize(getMajorAxis(), getMinorAxis());
}

void EllipticalRegion::modify(int distanceX, int distanceY) {
    if (getIsAllSelected()) {
        move(distanceX, distanceY);
    } else if (getIsRightSelected()) {
        modifyWidth(distanceX);
    } else if (getIsBottomSelected()) {
        modifyHeight(distanceY);
    }
}

void EllipticalRegion::setIsAllSelected(void) {
    Region::setIsAllSelected();
    setIsRightSelected();
    setIsBottomSelected();
}

void EllipticalRegion::clearIsAllSelected(void) {
    Region::clearIsAllSelected();
    clearIsRightSelected();
    clearIsBottomSelected();
}

void EllipticalRegion::setIsRightSelected(void) {
    isRightSelected = true;
}

void EllipticalRegion::clearIsRightSelected(void) {
    isRightSelected = false;
}

bool EllipticalRegion::getIsRightSelected(void) {
    return isRightSelected;
}

void EllipticalRegion::setIsBottomSelected(void) {
    isBottomSelected = true;
}

void EllipticalRegion::clearIsBottomSelected(void) {
    isBottomSelected = false;
}

bool EllipticalRegion::getIsBottomSelected(void) {
    return isBottomSelected;
}

bool EllipticalRegion::checkAmbit(QPoint point) {
    bool result = false;
    QPoint center = getCenterPoint();
    double x = (point.x() < center.x()) ? center.x() - point.x() : point.x() - center.x(),
           y = (point.y() < center.y()) ? center.y() - point.y() : point.y() - center.y();
    double a = getMajorAxis(), b = getMinorAxis();
    if (SQUARE(x) / SQUARE(a) + SQUARE(y) / SQUARE(b) <= 1) {
        result = true;
        setIsSelected();
    } else {
        clearIsSelected();
    }

    return result;
}

bool EllipticalRegion::hitModifyPoints(const QPoint &point) {
    bool result = hitCenterPoint(point);
    if (!result) {
        result = hitBottomModifyPoint(point) | hitRightModifyPoint(point);
    }
    return result;
}

bool EllipticalRegion::hitBottomModifyPoint(const QPoint &point) {
    bool result = false;
    QPoint bottomModifyPoint(getCenterPoint().x(), getCenterPoint().y() + getMinorAxis());
    if (distance(point, bottomModifyPoint) <= SQUARE(MODIFY_POINT_RADIUS)) {
        result = true;
        setIsBottomSelected();
    } else {
        clearIsBottomSelected();
    }
    return result;
}

bool EllipticalRegion::hitRightModifyPoint(const QPoint &point) {
    bool result = false;
    QPoint rightModifyPoint(getCenterPoint().x() + getMajorAxis(), getCenterPoint().y());
    if (Region::distance(point, rightModifyPoint) <= SQUARE(MODIFY_POINT_RADIUS)) {
        result = true;
        setIsRightSelected();
    } else {
        clearIsRightSelected();
    }
    return result;
}

QPoint EllipticalRegion::getStartPoint(void) {
    return getCenterPoint();
}

QPoint EllipticalRegion::getCenterPoint(void) {
    return center;
}

void EllipticalRegion::move(int distanceX, int distanceY) {
    Region::move(distanceX, distanceY);
    setCenter(QPoint(getCenterPoint().x() + distanceX, getCenterPoint().y() + distanceY));
}

void EllipticalRegion::modifyHeight(int distance) {
    Region::modifyHeight(distance);
    setMinorAxis(getMinorAxis() + distance);
}

void EllipticalRegion::modifyWidth(int distance) {
    Region::modifyWidth(distance);
    setMajorAxis(getMajorAxis() + distance);
}
