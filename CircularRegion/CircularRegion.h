#ifndef _CIRCULARREGION_H_
#define _CIRCULARREGION_H_

#include "..\common.h"
#include "..\EllipticalRegion\EllipticalRegion.h"

class CircularRegion : public EllipticalRegion{
private:
    //半徑
    int radius;
    //是否選取右下角的調整點
    bool isRightBottomSelected;
    //初始設定
    void init(void);
    //設定半徑
    void setRadius(int radius);
    //選取右下角的調整點
    void setIsRightBottomSelected(void);
    //取消選取右下角的調整點
    void clearIsRightBottomSelected(void);
    //測試point是否落在右下角的調整點中
    bool hitRightBottomModifyPoint(const QPoint &point);
    //選取整個區域範圍
    void setIsAllSelected(void);
    //移動區域
    void move(int distanceX, int distanceY);
    //調整區域半徑
    void modifyRadius(int distance);
public:
    CircularRegion(int serialNumber, QPoint center, int radius);
    CircularRegion(CircularRegion &region);
    CircularRegion(CircularRegion *region);
    ~CircularRegion();
    //取的半徑
    int getRadius(void);
    //調整區域
    void modify(int distanceX, int distanceY);
    //測試point是否落在區域範圍內
    bool checkAmbit(QPoint point);
    //測試point是否落在任一調整點中
    virtual bool hitModifyPoints(const QPoint &point);
    //取得右下調整點的選取狀態
    bool getIsRightBottomSelected(void);
protected:
    void clearIsAllSelected(void);
};

#endif
