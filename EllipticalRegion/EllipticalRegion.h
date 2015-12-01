#ifndef _ELLIPTICALREGION_H_
#define _ELLIPTICALREGION_H_

#include <QSize>

#include "..\common.h"
#include "..\Region\Region.h"

class EllipticalRegion : public Region{
private:
    QPoint center;
    int majorAxis;
    int minorAxis;
    bool isRightSelected;
    bool isBottomSelected;
    void init(void);
    void setCenter(QPoint center);
    void setMajorAxis(int majorAxis);
    void setMinorAxis(int minorAxis);
    void setIsRightSelected(void);
    void clearIsRightSelected(void);
    void setIsBottomSelected(void);
    void clearIsBottomSelected(void);
    bool hitBottomModifyPoint(const QPoint &point);
    bool hitRightModifyPoint(const QPoint &point);
public:
    EllipticalRegion(int serialNumber, QPoint center, int majorAxis, int minorAxis, Shapes shape = Shapes::Ellipse);
    EllipticalRegion(EllipticalRegion &region);
    EllipticalRegion(EllipticalRegion *region);
    virtual ~EllipticalRegion();
    int getMajorAxis(void);
    int getMinorAxis(void);
    QSize getSize(void);
    QPoint getStartPoint(void);
    virtual void modify(int distanceX, int distanceY);
    virtual bool checkAmbit(QPoint point);
    QPoint getCenterPoint(void);
    bool getIsRightSelected(void);
    bool getIsBottomSelected(void);
    virtual bool hitModifyPoints(const QPoint &point);
protected:
    virtual void move(int distanceX, int distanceY);
    virtual void modifyHeight(int distance);
    virtual void modifyWidth(int distance);
    virtual void setIsAllSelected(void);
    virtual void clearIsAllSelected(void);
};
#endif
