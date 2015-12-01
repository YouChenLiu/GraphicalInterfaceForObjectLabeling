#include "VideoProcessor.h"
#include <QPixmap>

VideoProcessor::VideoProcessor(QString filePath){
    init();
    setFilePath(filePath);
    sourceType = SourceType::VIDEO;
    loadVideo();
}

VideoProcessor::VideoProcessor(QString path, QString prefix, QString extension, int paddingLength, int firstNumber)
{
    init();
    poSequence = std::unique_ptr<ImageSequence>(new ImageSequence(path.toStdString(), prefix.toStdString(), extension.toStdString()));
    poSequence->setAttribute(ImageSequence::Attribute::FIRST_NUMBER, firstNumber);
    poSequence->setAttribute(ImageSequence::Attribute::PADDING_LENGTH, paddingLength);
    sourceType = SourceType::SEQUENCE;
    queryingFrameNumber = firstNumber - 1;
}

VideoProcessor::~VideoProcessor() {
    while (firstFrameContainer) {
        FrameContainer* tmp = firstFrameContainer;
        firstFrameContainer = firstFrameContainer->getNextFrameContainer();
        delete tmp;
    }
    closeVideo();
}

void VideoProcessor::init(void){
    this->video = NULL;
    this->queryingFrameNumber = -1;
    this->firstFrameContainer = NULL;
    this->currentFrameContainer = NULL;
}

void VideoProcessor::setFilePath(QString videoFilePath) {
    this->filePath = videoFilePath;
}

QString VideoProcessor::getFilePath(void){
    return filePath;
}

QString VideoProcessor::getFileName(void) {
    QString path = getFilePath();
    int slashIndex = path.lastIndexOf(tr("/"));
    return path.right(path.length() - slashIndex - 1);
}

double VideoProcessor::getWidthRatio() const
{
    return widthRatio;
}

double VideoProcessor::getHeightRatio() const
{
    return heightRatio;
}

bool VideoProcessor::loadVideo(void) {
    if (sourceType == SourceType::VIDEO) {
        if (filePath.isEmpty()) {
            return false;
        }
        video = new cv::VideoCapture(filePath.toStdString());
        if (video){
            return true;
        }else{
            qDebug("wrong file path");
            return false;
        }
    }
    return false;
}

void VideoProcessor::closeVideo(void){
    if ((video != NULL) && (video->isOpened())) {
        video->release();
    }
}

bool VideoProcessor::queryNextFrame(void) {
    if ((currentFrameContainer != NULL) && (currentFrameContainer->getFrameNumber()) < queryingFrameNumber) {
        currentFrameContainer = currentFrameContainer->getNextFrameContainer();
        bufferFrame = currentFrameContainer->getFrame();
        return true;
    }

    bool result = false;
    cv::Mat frame;
    if (sourceType == SourceType::VIDEO) {
        if (!video->isOpened()) {
            return false;
        }
        result = video->read(frame);
    } else if (sourceType == SourceType::SEQUENCE) {
        result = *poSequence >> frame;
    }
    if (result) {
        QSize imageSize(frame.cols, frame.rows);
        widthRatio = static_cast<double>(imageSize.width()) / static_cast<double>(VIDEO_VIEEWER_WIDTH);
        heightRatio = static_cast<double>(imageSize.height()) / static_cast<double>(VIDEO_VIEEWER_HEIGHT);
        cv::resize(frame, frame, cv::Size2i(VIDEO_VIEEWER_WIDTH, VIDEO_VIEEWER_HEIGHT), 0, 0, cv::INTER_NEAREST);
        bufferFrame = transformMatToQPixmap(frame);
        currentFrameContainer = new FrameContainer(bufferFrame, ++queryingFrameNumber);
        if (firstFrameContainer == NULL) {
            firstFrameContainer = currentFrameContainer;
//            cleanBuffer();
        } else {
            firstFrameContainer->push(currentFrameContainer);
        }
        return true;
    }
    return false;
}

QPixmap VideoProcessor::getFrame(void) {
//    QPainter painter(&bufferFrame);
//    painter.setPen(Qt::white);
//    painter.drawText(QPoint(10, 20), QString::number(getFrameNumber()));
    return bufferFrame;
}

int VideoProcessor::getFrameNumber(void) {
    return currentFrameContainer->getFrameNumber();
}

bool VideoProcessor::queryPreFrame(void) {
    if (!currentFrameContainer) {
        return false;
    }
    FrameContainer* preFrameContainer = currentFrameContainer->getPreFrameContainer();
    if (preFrameContainer) {
        currentFrameContainer = preFrameContainer;
        bufferFrame = currentFrameContainer->getFrame();
        return true;
    } else {
        return false;
    }
}

void VideoProcessor::jumpFrame(int targetFrameNumber) {
    FrameContainer* frameContainer = firstFrameContainer;
    while (frameContainer->getFrameNumber() <= targetFrameNumber) {
        if (frameContainer->getFrameNumber() == targetFrameNumber) {
            currentFrameContainer = frameContainer;
            break;
        }
        frameContainer = frameContainer->getNextFrameContainer();
    }
}

void VideoProcessor::drawRegion(RegionHeader* header){
    if (!header) {
        return;
    }
    if (header->getFrameNumber() != currentFrameContainer->getFrameNumber()) {
        return;
    }
    if (!currentFrameContainer) {
        return;
    }
    QPainter *painter = new QPainter(&bufferFrame);
    Region* region = header->getFirstRegion();
    while (region) {
        switch (region->getShape()) {
        case Shapes::Rectangle:
            drawRectangleRegion(painter, (RectangularRegion*)region);
            break;
        case Shapes::Circle:
            drawCircularRegion(painter, (CircularRegion*)region);
            break;
        case Shapes::Ellipse:
            drawEllipticalRegion(painter, (EllipticalRegion*)region);
            break;
        default:
            break;
        }
        region = region->getNextRegion();
    }
    delete painter;
}

void VideoProcessor::drawModifyPoint(QPainter *painter, QPoint center, QBrush brush)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);
    painter->drawEllipse(center, MODIFY_POINT_RADIUS, MODIFY_POINT_RADIUS);
    painter->setBrush(Qt::NoBrush);
}

void VideoProcessor::drawRectangleRegion(QPainter *painter, RectangularRegion *rectangularRegion){
    QPoint startPoint(rectangularRegion->getStartPoint().x() / widthRatio,
                      rectangularRegion->getStartPoint().y() / heightRatio);
    QPoint endPoint(rectangularRegion->getRightBottomPoint().x() / widthRatio,
                    rectangularRegion->getRightBottomPoint().y() / heightRatio);
    if (rectangularRegion->getIsSelected()) {
        QPoint center(rectangularRegion->getCenterPoint().x() / widthRatio,
                      rectangularRegion->getCenterPoint().y() / heightRatio);
        int width = rectangularRegion->getWidth() / widthRatio;
        int height = rectangularRegion->getHeight() / heightRatio;
        int halfWidth = width / 2;
        int halfHeight = height / 2;
        drawModifyPoint(painter, center, rectangularRegion->getIsAllSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);
        painter->setPen(rectangularRegion->getIsTopSelected() ? MODIFYING_REGION_COLOR : NOMAL_REGION_COLOR);
        painter->drawLine(startPoint, QPoint(endPoint.x(), startPoint.y()));
        drawModifyPoint(painter, QPoint(startPoint.x() + halfWidth, startPoint.y()), rectangularRegion->getIsTopSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);
        painter->setPen(rectangularRegion->getIsBottomSelected() ? MODIFYING_REGION_COLOR : NOMAL_REGION_COLOR);
        painter->drawLine(QPoint(startPoint.x(), endPoint.y()), endPoint);
        drawModifyPoint(painter, QPoint(startPoint.x() + halfWidth, endPoint.y()), rectangularRegion->getIsBottomSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);
        painter->setPen(rectangularRegion->getIsLeftSelected() ? MODIFYING_REGION_COLOR : NOMAL_REGION_COLOR);
        painter->drawLine(startPoint, QPoint(startPoint.x(), endPoint.y()));
        drawModifyPoint(painter, QPoint(startPoint.x(), startPoint.y() + halfHeight), rectangularRegion->getIsLeftSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);
        painter->setPen(rectangularRegion->getIsRightSelected() ? MODIFYING_REGION_COLOR : NOMAL_REGION_COLOR);
        painter->drawLine(QPoint(endPoint.x(), startPoint.y()), endPoint);
        drawModifyPoint(painter, QPoint(endPoint.x(), startPoint.y() + halfHeight), rectangularRegion->getIsRightSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);

    } else {
        painter->setPen(NOMAL_REGION_COLOR);
        painter->drawRect(QRect(startPoint, endPoint));
    }
}

void VideoProcessor::drawEllipticalRegion(QPainter *painter, EllipticalRegion* ellipticalRegion){
    QPoint center = QPoint(ellipticalRegion->getCenterPoint().x() / widthRatio,
                           ellipticalRegion->getCenterPoint().y() / heightRatio);
    QSize size = QSize(ellipticalRegion->getSize().width() / widthRatio,
                       ellipticalRegion->getSize().height() / heightRatio);
    QPoint leftTop(center.x() - size.width(), center.y() - size.height());
    QRect rect(leftTop, 2 * size);
    if (ellipticalRegion->getIsSelected()) {
        if (ellipticalRegion->getIsAllSelected()) {
            painter->setPen(MODIFYING_REGION_COLOR);
            painter->drawEllipse(rect);
        } else {
            if (ellipticalRegion->getIsRightSelected()) {
                painter->setPen(MODIFYING_REGION_COLOR);
                painter->drawArc(rect, 90 * 16, -180 * 16);
                painter->setPen(NOMAL_REGION_COLOR);
                painter->drawArc(rect, 90 * 16, 180 * 16);
            } else if (ellipticalRegion->getIsBottomSelected()) {
                painter->setPen(MODIFYING_REGION_COLOR);
                painter->drawArc(rect, 180 * 16, 180 * 16);
                painter->setPen(NOMAL_REGION_COLOR);
                painter->drawArc(rect, 180 * 16, -180 * 16);
            } else {
                painter->setPen(NOMAL_REGION_COLOR);
                painter->drawEllipse(rect);
            }
        }

        drawModifyPoint(painter, center, ellipticalRegion->getIsAllSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);
        QPoint rightModifyPoint(center.x() + size.width(), center.y());
        drawModifyPoint(painter, rightModifyPoint, ellipticalRegion->getIsRightSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);
        QPoint downModifyPoint(center.x(), center.y() + size.height());
        drawModifyPoint(painter, downModifyPoint, ellipticalRegion->getIsBottomSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR);
    } else {
        painter->setPen(NOMAL_REGION_COLOR);
        painter->drawEllipse(center, size.width(), size.height());
    }
}

void VideoProcessor::drawCircularRegion(QPainter *painter, CircularRegion *circularRegion){
    QPoint center = QPoint(circularRegion->getCenterPoint().x() / widthRatio,
                           circularRegion->getCenterPoint().y() / heightRatio);
    int radius = circularRegion->getRadius() / widthRatio;
    if (circularRegion->getIsSelected()) {
        QPoint modifyPoint(center.x() + (int)(radius * cos(45 * PI / 180)), center.y() + (int)(radius * sin(45 * PI / 180)));
        drawModifyPoint(painter, center, (circularRegion->getIsAllSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR));
        painter->setPen(circularRegion->getIsRightBottomSelected() ? MODIFYING_REGION_COLOR : NOMAL_REGION_COLOR);
        painter->drawEllipse(center, radius, radius);
        drawModifyPoint(painter, modifyPoint, (circularRegion->getIsRightBottomSelected() ? MODIFYING_MODIFY_POINT_COLOR : NORMAL_MODIFY_POINT_COLOR));
    } else {
        painter->setPen(NOMAL_REGION_COLOR);
        painter->drawEllipse(center, radius, radius);
    }
}

void VideoProcessor::cleanBuffer(void) {
    bufferFrame = currentFrameContainer->getFrame();
}

QImage VideoProcessor::transformMatToQImage(const cv::Mat& mat) const{
    static QVector<QRgb>  sColorTable;
    switch (mat.type()) {
    case CV_8UC4:    // 8-bit, 4 channel
        return QImage( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32 );
    case CV_8UC3:    // 8-bit, 3 channel
        return QImage( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 ).rgbSwapped();
    case CV_8UC1:    // 8-bit, 1 channel
        // only create our color table once
        if (sColorTable.isEmpty()) {
            for (int i = 0; i < 256; i++) {
                sColorTable.push_back( qRgb( i, i, i ) );
            }
        }
//        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );
//        image.setColorTable( sColorTable );
//        return QImage( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 ).setColorTable( sColorTable );
        break;
    default:
        qWarning("ASM::cvMatToQImage() - cv::Mat image type not handled in switch:");
        break;
    }

    return QImage();
}

QPixmap VideoProcessor::transformMatToQPixmap(const cv::Mat& mat) const {
    return QPixmap::fromImage(transformMatToQImage(mat));
}
