#include "VideoViewer.h"

VideoViewer::VideoViewer(const QString &text, QWidget *parent, Qt::WindowFlags flag) : QLabel(text, parent, flag)
{
    setCurrentPoint(-1, -1);
    setPressedPoint(-1, -1);
    isPressed = false;
}

VideoViewer::VideoViewer(QWidget *parent, Qt::WindowFlags flag) : QLabel(parent, flag)
{
    setCurrentPoint(-1, -1);
    setPressedPoint(-1, -1);
    isPressed = false;
}

void VideoViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (!isPressed) {
        return;
    }
    setCurrentPoint(event->pos().x(), event->pos().y());
    repaint();
}

void VideoViewer::mousePressEvent(QMouseEvent *event)
{
    setPressedPoint(event->pos().x(), event->pos().y());
    isPressed = true;
}

void VideoViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (currentPoint == QPoint(-1, -1)) {
        emit clicked(event->pos());
    } else {
        QPoint leftTop(pressedPoint.x() > currentPoint.x() ? currentPoint.x() : pressedPoint.x(),
                       pressedPoint.y() > currentPoint.y() ? currentPoint.y() : pressedPoint.y());
        QPoint rightBottom(pressedPoint.x() > currentPoint.x() ? pressedPoint.x() : currentPoint.x(),
                           pressedPoint.y() > currentPoint.y() ? pressedPoint.y() : currentPoint.y());
        emit drawNewRegion(leftTop, rightBottom);
    }
    setCurrentPoint(-1, -1);
    setPressedPoint(-1, -1);
    isPressed = false;
}

void VideoViewer::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if (!enabledDrawPreviewRegion) {
        return;
    }
    if (isPressed && (currentPoint != QPoint(-1, -1))) {
        QPainter painter(this);
        painter.setPen(NOMAL_REGION_COLOR);
        QPoint leftTop(pressedPoint.x() > currentPoint.x() ? currentPoint.x() : pressedPoint.x(),
                       pressedPoint.y() > currentPoint.y() ? currentPoint.y() : pressedPoint.y());
        QPoint rightBottom(pressedPoint.x() > currentPoint.x() ? pressedPoint.x() : currentPoint.x(),
                           pressedPoint.y() > currentPoint.y() ? pressedPoint.y() : currentPoint.y());
        QPoint center((leftTop.x() + rightBottom.x()) / 2,
                      (leftTop.y() + rightBottom.y()) / 2);
        int width = rightBottom.x() - leftTop.x();
        int height = rightBottom.y() - leftTop.y();
        int radius = 0;
        switch (shape) {
        case Shapes::Circle:
            radius = (int)(sqrt(SQUARE(width) + SQUARE(height)) / 2);
            painter.drawEllipse(center, radius, radius);
            break;
        case Shapes::Ellipse:
            painter.drawEllipse(center, width / 2, height / 2);
            break;
        case Shapes::Rectangle:
            painter.drawRect(leftTop.x(), leftTop.y(), width, height);
            break;
        default:
            break;
        }
    }
}

void VideoViewer::setPressedPoint(int x, int y)
{
    pressedPoint.setX(x);
    pressedPoint.setY(y);
}

void VideoViewer::setCurrentPoint(int x, int y)
{
    currentPoint.setX(x);
    currentPoint.setY(y);
}

void VideoViewer::setShape(Shapes shape)
{
    this->shape = shape;
}

void VideoViewer::setEnabledDrawPreviewRegion(bool enabled)
{
    enabledDrawPreviewRegion = enabled;
}
