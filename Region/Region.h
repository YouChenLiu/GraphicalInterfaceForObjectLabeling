#ifndef _REGION_H_
#define _REGION_H_

#include <QSize>

#include "..\common.h"
#include "..\LinkedListNode\LinkedListNode.h"

class Region : protected LinkedListNode{
private:
    //框選範圍起始點
    QPoint startPoint;
    //框選範圍高
    int height;
    //框選範圍寬
    int width;
    //框選範圍的流水號
    int serialNumber;
    //框選範圍的實際形狀
    Shapes shape;
    //是否選擇到此區域
    bool isSelected;
    //是否選取整體區域
    bool isAllSelected;
    //設定流水號
    void setSerialNumber(int serialNumber);
    //設定框選範圍的實際形狀
    void setShape(Shapes shape);
    //初始化
    void init(void);
    //設定框選範圍起點
    void setStartPoint(QPoint startPoint);
    //設定框選範圍的高度
    void setHeight(int height);
    //設定框選範圍的寬度
    void setWidth(int width);
public:
    Region(int serialNumber, QPoint startPoint, int height, int width, Shapes shape = Shapes::Undefine);
    Region(Region &region);
    Region(Region *region);
    virtual ~Region();
    //於串列中新增框選區域
    void addRegion(Region* newRegion);
    //取得框選範圍的流水號
    int getSerialNumber(void);
    //取得框選範圍起始點
    QPoint getStartPoint(void);
    //取得框選範圍的高度
    int getHeight(void);
    //取得框選範圍的寬度
    int getWidth(void);
    QSize getSize(void);
    //取得框選範圍的形狀
    Shapes getShape(void);
    //設定下一個節點
    void setNextRegion(Region* node);
    //設定上一個節點
    void setPreRegion(Region* node);
    //取得下一個節點
    Region* getNextRegion(void);
    //取得上一個節點
    Region* getPreRegion(void);
    void push(Region* newRegion);
    Region* pop(void);
    //取得選取狀態
    bool getIsSelected(void);
    //調整框選範圍
    virtual void modify(int distanceX, int distanceY) = 0;
    //測試point是否落在框選範圍內
    virtual bool checkAmbit(QPoint point) = 0;
    //取得中心座標
    virtual QPoint getCenterPoint(void);
    //計算兩點間的距離(回傳值為其距離平方)
    static int distance(const QPoint &p1, const QPoint &p2);
    //
    virtual bool getIsAllSelected(void);
    //測試point是否落在任一調整點中
    virtual bool hitModifyPoints(const QPoint &point) = 0;
protected:
    //移動框選範圍
    virtual void move(int distanceX, int distanceY);
    //調整框選範圍的高度
    void modifyHeight(int distance);
    //調整框選範圍的寬度
    virtual void modifyWidth(int distance);
    void setIsSelected(void);
    void clearIsSelected(void);
    virtual void setIsAllSelected(void);
    virtual void clearIsAllSelected(void);
    //測試point是否落在中心的調整點上
    virtual bool hitCenterPoint(QPoint point);
};
#endif
