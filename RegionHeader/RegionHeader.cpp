#include "RegionHeader.h"

RegionHeader::RegionHeader(int frameNumber){
    init();
    setFrameNumber(frameNumber);
}

RegionHeader::RegionHeader(int frameNumber, RegionHeader* header){
	init();
	setFrameNumber(frameNumber);
	Region* region = header->getFirstRegion();
	while (region) {
        Region* newRegion = NULL;
        switch (region->getShape()) {
        case Shapes::Rectangle:
            newRegion = new RectangularRegion((RectangularRegion*)region);
            break;
        case Shapes::Circle:
            newRegion = new CircularRegion((CircularRegion*)region);
            break;
        case Shapes::Ellipse:
            newRegion = new EllipticalRegion((EllipticalRegion*)region);
            break;
        default:
            qDebug() << "shape error when HegionHeader constructor." << endl;
            break;
        }
        addRegion(newRegion);
		region = region->getNextRegion();
	}
}


RegionHeader::~RegionHeader(){
    deleteAllRegion();
}

void RegionHeader::init(void){
    firstRegion = lastRegion = NULL;
    totalRegion = 0;
}

void RegionHeader::setFrameNumber(int frameNumber){
    this->frameNumber = frameNumber;
}

int RegionHeader::getTotalRegion(void){
    return totalRegion;
}

Region* RegionHeader::getFirstRegion(void){
    return firstRegion;
}

Region* RegionHeader::getLastRegion(void) {
    return lastRegion;
}

void RegionHeader::addRegion(Region* region){
    if (!region) {
        return;
    }
    region->checkAmbit(QPoint(-1, -1));
    region->hitModifyPoints(QPoint(-1, -1));
    if (lastRegion) {
        lastRegion->push(region);
        lastRegion = region;
    } else if (!firstRegion) {
        firstRegion = lastRegion = region;
    }
    totalRegion++;
}

void RegionHeader::addRectangularRegion(int serialNumber, QPoint leftTop, int width, int height) {
    addRegion(new RectangularRegion(serialNumber, leftTop, width, height));
}

void RegionHeader::addEllipticalRegion(int serialNumber, QPoint center, int majorAxis, int minorAxis) {
    addRegion(new EllipticalRegion(serialNumber, center, majorAxis, minorAxis));
}

void RegionHeader::addCircularRegion(int serialNumber, QPoint center, int radius) {
    addRegion(new CircularRegion(serialNumber, center, radius));
}

void RegionHeader::deleteRegion(int serialNumber){
    if (serialNumber < 0) {
        return;
    }

    Region* region = getFirstRegion();
    while (region) {
        if (region->getSerialNumber() == serialNumber) {
            if (firstRegion == region) {
                firstRegion = region->getNextRegion();
            }
            if (lastRegion == region) {
                lastRegion = region->getPreRegion();
            }
            delete region;
            totalRegion--;
            break;
        }
        region = region->getNextRegion();
    }
}

void RegionHeader::addRegionHeader(RegionHeader* newHeader){
    RegionHeader* header = this;
    RegionHeader* nextHeader = header->getNextHeader();
    int newFrameNumber = newHeader->getFrameNumber();
    while (nextHeader) {
        if (nextHeader->getFrameNumber() > newFrameNumber) {
            break;
        }
        header = header->getNextHeader();
        nextHeader = nextHeader->getNextHeader();
    }
    
    header->addNode(newHeader);

    if (header->getFrameNumber() == newFrameNumber) {
        delete header;
    }
}

void RegionHeader::setNextHeader(RegionHeader* header){
    setNextNode(header);
}

void RegionHeader::setPreHeader(RegionHeader* header){
    setPreNode(header);
}

RegionHeader* RegionHeader::getNextHeader(void){
    return ((RegionHeader*)getNextNode());
}

RegionHeader* RegionHeader::getPreHeader(void){
    return ((RegionHeader*)getPreNode());
}

int RegionHeader::getFrameNumber(void){
    return frameNumber;
}

void RegionHeader::deleteAllRegion() {
    Region* region = getFirstRegion();
    while (region) {
        Region* tmp = region;
        region = region->getNextRegion();
        delete tmp;
    }
}

void RegionHeader::deleteAllHeader(void) {
    RegionHeader* header = this;
    while (header->getPreHeader()) {
        header = header->getNextHeader();
    }
    while (header) {
        RegionHeader* tmp = header;
        header = header->getNextHeader();
        delete tmp;
    }
}

RegionHeader* RegionHeader::seachHeader(int targetFrameNumber) {
    RegionHeader* targetHeader = this;
    if (targetFrameNumber < 0) {
        targetFrameNumber = 0;
    }
    while (targetHeader) {
        if (targetHeader->getFrameNumber() == targetFrameNumber) {
            break;
        }
        targetHeader = targetHeader->getNextHeader();
    }

    return targetHeader;
}

int RegionHeader::getUnusedSerialNumber(void) {
    int value = 0;
    
    if (!totalRegion) {
        return value;
    }

    Region* region = firstRegion;
    int* usedSerialNumber = new int[totalRegion];

    for (int i = 0; i < totalRegion; i++, region = region->getNextRegion()) {
        usedSerialNumber[i] = region->getSerialNumber();
    }
    
    for (int i = 0; i < INT_MAX; i++) {
        bool isExist = false;
        for (int index = 0; index < totalRegion; index++) {
            if (i == usedSerialNumber[index]) {
                isExist = true;
                break;
            }
        }
        if (!isExist) {
            value = i;
            break;
        }
    }

    delete[] usedSerialNumber;
    return value;
}

void RegionHeader::hitRegion(int serialNumber)
{
    Region *region = getFirstRegion();
    while (region != NULL) {
        region->hitModifyPoints(region->getSerialNumber() == serialNumber ?
                                    region->getCenterPoint() : QPoint(-1, -1));
        region = region->getNextRegion();
    }
}

void RegionHeader::sendModifyMessage(int distanceX, int distanceY)
{
    Region* region = getFirstRegion();
    while (region) {
        if (region->getIsSelected()) {
            region->modify(distanceX, distanceY);
        }
        region = region->getNextRegion();
    }
}

void RegionHeader::sendCleanMessage(void)
{
    Region* region = getFirstRegion();
    while (region) {
        region->checkAmbit(QPoint(-1, -1));
        region = region->getNextRegion();
    }
}

void RegionHeader::sendRemoveMessage()
{
    Region *region = getFirstRegion();
    while (region) {
        if (region->getIsAllSelected()) {
            Region *tmp = region;
            region = region->getNextRegion();
            deleteRegion(tmp->getSerialNumber());
            continue;
        }
        region = region->getNextRegion();
    }
}

void RegionHeader::sendHitMessage(const QPoint point)
{
    //尋找有在點擊範圍內的區域
    Region *region = getFirstRegion();
    while (region) {
        if (region->getIsSelected()) {               //針對已被選取的區域進一步確認是否點擊在調整點上
            if (!region->hitModifyPoints(point)) {
                region->checkAmbit(point);           //沒有點擊到調整點則確認是否點在區域內
            }
        } else {
            region->checkAmbit(point);               //確認是否點在區域內
        }
        region = region->getNextRegion();
    }
}
