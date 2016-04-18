#ifndef _VIDEOPROCESSOR_H_
#define _VIDEOPROCESSOR_H_

#include "../common.h"
#include "../RegionHeader/RegionHeader.h"
#include "../RectangularRegion/RectangularRegion.h"
#include "../EllipticalRegion/EllipticalRegion.h"
#include "../CircularRegion/CircularRegion.h"
#include "../FrameContainer/FrameContainer.h"
#include "../ImageSequence/ImageSequence.h"
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <memory>

class VideoProcessor : QObject {

public:
    enum class SourceType {
        VIDEO = 0,
        SEQUENCE
    };

public:
    VideoProcessor(QString filePath);
    VideoProcessor(QString path, QString prefix, QString extension, int paddingLength, int firstNumber);
    virtual ~VideoProcessor();
    //取得目前video的Frame
    QPixmap getFrame(void);
    //讀取下一個Frame
    bool queryNextFrame(void);
    //讀取上一個Frame
    bool queryPreFrame(void);
    //取得目前Frame的編號
    int getFrameNumber(void);
    //跳躍至特定的畫格數
    void jumpFrame(int targetFrameNumber);
    //畫出header中所含的框選區域
    void drawRegion(RegionHeader* header);
    //清除暫時畫布
    void cleanBuffer(void);
    //取得檔案路徑
    QString getFilePath(void);
    QString getFileName(void);
    double getWidthRatio(void) const;
    double getHeightRatio(void) const;

private:
    //影片路徑
    QString filePath;
    cv::VideoCapture* video;
    //內部進行繪製的畫格
    QPixmap bufferFrame;
    //畫格編號
    int queryingFrameNumber;
    //影片的第一個畫格
    FrameContainer* firstFrameContainer;
    //目前處理的畫格
    FrameContainer* currentFrameContainer;
    //
    std::unique_ptr<ImageSequence> poSequence;
    //
    SourceType sourceType;
    //
    double widthRatio;
    double heightRatio;


private:
    //初始化
    void init(void);
    //設定影片檔案路徑
    void setFilePath(QString filePath);
    //開啟影片檔案
    bool loadVideo(void);
    //關閉影片檔案
    void closeVideo(void);

    void drawModifyPoint(QPainter *painter, QPoint center, QBrush brush);
    //於bufferFrame中繪製矩形區域
    void drawRectangleRegion(QPainter *painter, RectangularRegion *rectangularRegion);
    //於bufferFrame中繪製橢圓形區域
    void drawEllipticalRegion(QPainter *painter, EllipticalRegion* ellipticalRegion);
    //於bufferFrame中繪製圓形區域
    void drawCircularRegion(QPainter *painter, CircularRegion* circlularRegion);
    //
    int frameNumber;

    QImage transformMatToQImage(const cv::Mat& mat) const;
    QPixmap transformMatToQPixmap(const cv::Mat& mat) const;

protected:

};

#endif
