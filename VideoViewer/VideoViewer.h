#ifndef _VIDEO_VIEWER_H_
#define _VIDEO_VIEWER_H_

#include "../common.h"
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

class VideoViewer : public QLabel {
    Q_OBJECT

private:
    QPoint pressedPoint;
    QPoint currentPoint;
    bool isPressed;
    bool enabledDrawPreviewRegion;
    Shapes shape;

private:
    void setPressedPoint(int x, int y);
    void setCurrentPoint(int x, int y);

public :
    VideoViewer(QWidget* parent = NULL, Qt::WindowFlags flag = 0);
    VideoViewer(const QString& text, QWidget* parent = NULL, Qt::WindowFlags flag = 0);
    void setShape(Shapes shape);
    void setEnabledDrawPreviewRegion(bool enabled);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

    void drawNewRegion(QPoint leftTop, QPoint rightBottom);

    void clicked(QPoint point);

};

#endif
