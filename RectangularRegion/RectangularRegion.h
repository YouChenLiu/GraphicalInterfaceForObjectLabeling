#ifndef _RECTANGULARREGION_H_
#define _RECTANGULARREGION_H_

#include "..\common.h"
#include "..\Region\Region.h"

class RectangularRegion : public Region{
private:
    QPoint leftTopPoint;
    QPoint rightBottomPoint;
    void init(void);
    void setLeftTopPoint(QPoint leftTopPoint);
    void setRightBottomPoint(QPoint rightBottomPoint);
    void setRightBottomPoint(QPoint startPoint, int width, int height);
    bool isTopSelected;
    bool isBottomSelected;
    bool isLeftSelected;
    bool isRightSelected;
    bool hitTopModifyPoint(const QPoint &point);
    bool hitBottomModifyPoint(const QPoint &point);
    bool hitLeftModifyPoint(const QPoint &point);
    bool hitRightModifyPoint(const QPoint &point);
    void move(int distanceX, int distanceY);
    void modifyHeight(int distance);
    void modifyWidth(int distance);
    void setIsTopSelected(void);
    void clearIsTopSelected(void);
    void setIsBottomSelected(void);
    void clearIsBottomSelected(void);
    void setIsRightSelected(void);
    void clearIsRightSelected(void);
    void setIsLeftSelected(void);
    void clearIsLeftSelected(void);
public:
    RectangularRegion(int serialNumber, QPoint leftTop, QPoint rightBottom, Shapes shape = Shapes::Rectangle);
    RectangularRegion(int serialNumber, QPoint leftTop, int width, int height, Shapes shape = Shapes::Rectangle);
    RectangularRegion(RectangularRegion &region);
    RectangularRegion(RectangularRegion *region);
    void modify(int distanceX, int distanceY);
    bool checkAmbit(QPoint point);
    QPoint getLeftTopPoint(void);
    QPoint getRightBottomPoint(void);
    bool getIsTopSelected(void);
    bool getIsBottomSelected(void);
    bool getIsLeftSelected(void);
    bool getIsRightSelected(void);
    virtual bool hitModifyPoints(const QPoint &point);
protected:
    void setIsAllSelected(void);
    void clearIsAllSelected(void);
};

#endif
