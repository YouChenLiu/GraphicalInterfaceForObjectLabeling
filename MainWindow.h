#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QDomDocument>
#include <QTextStream>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QWidget>
#include <QTimer>
#include <QDate>
#include "common.h"
#include "VideoProcessor/VideoProcessor.h"
#include "RegionHeader/RegionHeader.h"
#include "Region/Region.h"
#include "VideoViewer/VideoViewer.h"
#include "sequencedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionVideo_triggered();

    void on_rectangleButton_clicked();

    void on_ellipseButton_clicked();

    void on_circleButton_clicked();

    void on_nextFrameButton_clicked();

    void on_preFrameButton_clicked();

    void on_resultTable_itemClicked(QTableWidgetItem *item);

    void on_readonlyCheckBox_stateChanged(int arg1);

    void on_playPauseButton_clicked();

    void on_replayButton_clicked();

    void on_positionSlider_valueChanged(int value);

    void on_positionSlider_rangeChanged(int min, int max);

    void playVideo();

    void on_actionClose_triggered();

    void on_actionSave_triggered();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_deleteButton_clicked();

    void on_cloneButton_clicked();

    void on_videoViewer_clicked(const QPoint point);

    void on_videoViewer_drawNewRegion(const QPoint &leftTop, const QPoint &rightBottom);

    void on_actionSequence_triggered();

private:
    Ui::MainWindow *ui;
    QString videoFilePath;
    QString xmlFilePath;
    Shapes shape;
    QDomDocument xmlDocument;
    bool isReadonly;
    VideoProcessor *video;
    QTimer *timer = new QTimer(this);
    bool isPlaying;
    RegionHeader *header;
    bool hasModify;

private:
    void setShape(Shapes shape);
    bool openXmlFile(void);
    bool createXmlFile(void);
    bool readXmlFile(void);
    bool writeXmlFile(void);
    void updateRegionHeader(int frameNumber);
    void updateResultTable(int frameNumber);
    void closeEvent(QCloseEvent *event);
    void updateVideoViewer(void);
    void disableAllButtons(void);
    void enableAllButtons(void);
    void setPlayButtonsEnable(bool enabled);
    void setDirectionButtonsEnable(bool enabled);
    void setNodeOperationButtinsEnable(bool enabled);
    void setModifyButtonsEnable(bool enabled);
    void updateShapeButtons(void);
    void setInitState(void);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
