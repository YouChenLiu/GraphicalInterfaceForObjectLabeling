#ifndef _REGIONHEADER_H_
#define _REGIONHEADER_H_

#include "..\common.h"
#include "..\Region\Region.h"
#include "..\RectangularRegion\RectangularRegion.h"
#include "..\CircularRegion\CircularRegion.h"
#include "..\EllipticalRegion\EllipticalRegion.h"

class RegionHeader : protected LinkedListNode {
private:
    //畫格編號
    //代表Header所託管的Region屬於影片第frameNumber個畫格之框選範圍
    int frameNumber;
    //Header所儲存的Region總數
    int totalRegion;
    //第一個Region所在位置
    Region* firstRegion;
    //最後一個Region所在位置
    Region* lastRegion;
    //初始化
    void init(void);
    //設定畫格編號
    void setFrameNumber(int frameNumber);
public:
    RegionHeader(int frameNumbet);
	RegionHeader(int frameNumber, RegionHeader* header);
    ~RegionHeader();
    //取得Header中的第一個框選區域
    Region* getFirstRegion(void);
    //取得Header中的最後一個框選區域
    Region* getLastRegion(void);
    //於Header中新增框選區域
    void addRegion(Region* region);
    //
    void addRectangularRegion(int serialNumber, QPoint leftTop, int width, int height);
    void addEllipticalRegion(int serialNumber, QPoint center, int majorAxis, int minorAxis);
    void addCircularRegion(int serialNumber, QPoint center, int radius);
    //於Header中刪除框選區域
    void deleteRegion(int serialNumber);
    //於串列中加入Header
    void addRegionHeader(RegionHeader* newHeader);
    //設定下一個節點
    void setNextHeader(RegionHeader* header);
    //設定上一個節點
    void setPreHeader(RegionHeader* header);
    //取得下一個節點
    RegionHeader* getNextHeader(void);
    //取得上一個節點
    RegionHeader* getPreHeader(void);
    //取得畫格編號
    int getFrameNumber(void);
    //取得Header所儲存的Region總數
    int getTotalRegion(void);
    //刪除Header中的所有框選結果
    void deleteAllRegion(void);
    //刪除所有的RegionHeader
    void deleteAllHeader(void);
    //以自身為起點向後搜尋特定畫格的Header
    RegionHeader* seachHeader(int targetFrameNumber);
    //取得尚未被使用的流水號
    int getUnusedSerialNumber(void);
    void hitRegion(int serialNumber);
    void sendModifyMessage(int distanceX, int distanceY);
    void sendCleanMessage(void);
    void sendRemoveMessage(void);
    void sendHitMessage(const QPoint point);
protected:
};
#endif
