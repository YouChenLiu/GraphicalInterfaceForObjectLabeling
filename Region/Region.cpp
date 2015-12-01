#include "Region.h"

Region::Region(int serialNumber, QPoint startPoint, int height, int width, Shapes shape){
    init();
    setSerialNumber(serialNumber);
    setStartPoint(startPoint);
    setHeight(height);
    setWidth(width);
    setShape(shape);
}

Region::Region(Region &region) {
    init();
    setSerialNumber(region.getSerialNumber());
    setStartPoint(region.getStartPoint());
    setHeight(region.getHeight());
    setWidth(region.getWidth());
    setShape(region.getShape());
}

Region::Region(Region *region) {
    init();
    setSerialNumber(region->getSerialNumber());
    setStartPoint(region->getStartPoint());
    setHeight(region->getHeight());
    setWidth(region->getWidth());
    setShape(region->getShape());
}

Region::~Region() {}

void Region::init(void){
    setStartPoint(QPoint(-1, -1));
    height = width = serialNumber = -1;
    shape = Shapes::Undefine;
    clearIsSelected();
    clearIsAllSelected();
    setPreRegion(NULL);
    setNextRegion(NULL);
}

void Region::setSerialNumber(int serialNumber){
    this->serialNumber = serialNumber;
}

int Region::getSerialNumber(void){
    return serialNumber;
}

void Region::setStartPoint(QPoint startPoint){
    this->startPoint.setX(startPoint.x());
    this->startPoint.setY(startPoint.y());
}

QPoint Region::getStartPoint(void){
    return startPoint;
}

void Region::setHeight(int height){
    this->height = height;
}

int Region::getHeight(void){
    return height;
}

void Region::setWidth(int width){
    this->width = width;
}

int Region::getWidth(void){
    return width;
}

QSize Region::getSize()
{
    return QSize(getWidth(), getHeight());
}

void Region::setShape(Shapes shape){
    this->shape = shape;
}

Shapes Region::getShape(void){
    return shape;
}

void Region::setPreRegion(Region* region){
    ((LinkedListNode*)this)->setPreNode(region);
}

Region* Region::getPreRegion(void){
    return ((Region*)getPreNode());
}

void Region::setNextRegion(Region* region){
    ((LinkedListNode*)this)->setNextNode(region);
}

Region* Region::getNextRegion(void){
    return ((Region*)getNextNode());
}

void Region::addRegion(Region* newRegion){
    push(newRegion);
}

void Region::setIsSelected(void){
    isSelected = true;
}

void Region::clearIsSelected(void){
    isSelected = false;
	clearIsAllSelected();
}

bool Region::getIsSelected(void){
    return Region::isSelected;
}

void Region::move(int distanceX, int distanceY) {
    setStartPoint(QPoint(getStartPoint().x() + distanceX, getStartPoint().y() + distanceY));
}

void Region::modifyHeight(int distance) {
    setHeight(getHeight() + distance);
}

void Region::modifyWidth(int distance) {
    setWidth(getWidth() + distance);
}

int Region::distance(const QPoint &p1, const QPoint &p2) {
    return (SQUARE(p1.x() - p2.x()) + SQUARE(p1.y() - p2.y()));
}

void Region::setIsAllSelected(void) {
    isSelected = true;
    isAllSelected = true;
}

void Region::clearIsAllSelected(void) {
    isAllSelected = false;
}

bool Region::getIsAllSelected(void) {
    return isAllSelected;
}

QPoint Region::getCenterPoint(void) {
    return QPoint(getStartPoint().x() + getWidth() / 2, getStartPoint().y() + getHeight() / 2);
}

bool Region::hitCenterPoint(QPoint point) {
    bool result = false;
    if (Region::distance(getCenterPoint(), point) <= SQUARE(MODIFY_POINT_RADIUS)){
        result = true;
        setIsAllSelected();
    } else {
        clearIsAllSelected();
    }
    return result;
}

void Region::push(Region* newRegion) {
    ((LinkedListNode*)this)->push(newRegion);
}

Region* Region::pop(void) {
    return (Region*)((LinkedListNode*)this)->pop();
}
