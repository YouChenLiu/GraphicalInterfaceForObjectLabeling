#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setInitState();
    connect(timer, SIGNAL(timeout()), this, SLOT(playVideo()));

    //let table layout beautiful
    ui->resultTable->setColumnWidth(0, 30);
    ui->resultTable->setColumnWidth(1, 40);
    ui->resultTable->setColumnWidth(2, 40);
    ui->resultTable->setColumnWidth(3, 50);
    ui->resultTable->setColumnWidth(4, 50);
    ui->resultTable->setColumnWidth(5, 70);

    isPlaying = false;
    isReadonly = true;
    hasModify = false;
    ui->resultTable->setFocusPolicy(Qt::NoFocus);
    ui->readonlyCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->positionSlider->setFocusPolicy(Qt::NoFocus);
    ui->videoViewer->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton check = QMessageBox::question(this, tr("Quit ?"), tr("Do You Want To Quit This Program?"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, QMessageBox::StandardButton::No);
    if (check == QMessageBox::StandardButton::No) {
        event->ignore();
    }
    on_actionClose_triggered();
}

void MainWindow::on_actionVideo_triggered()
{
    QString filterStringSeperateSymbol = tr(";;");
    QString videoFileFilterString = tr("Video Files (*.avi *.mp4)") + filterStringSeperateSymbol;
//    QString imageFileFilterString = tr("Image Files (*.jpg *.bmp)") + filterStringSeperateSymbol;
//    QString xmlFileFilterString = tr("XML Files (*.xml)") + filterStringSeperateSymbol;
//    QString anyFileFilterString = tr("Any Files (*.*)");
    QString filterString = videoFileFilterString;
    QString dialogTitle = tr("Open Video");
    QString dialogDefaultPath = QDir::currentPath();
    QString path = QFileDialog::getOpenFileName(this, dialogTitle, dialogDefaultPath, filterString);

    if (path.isEmpty()) {
        ui->statusBar->showMessage(tr("Canceled Open File"));
    } else {
        on_actionClose_triggered();
        video = new VideoProcessor(videoFilePath = path);
        path.resize(path.lastIndexOf(tr(".")) + 1);
        xmlFilePath = path + QString(tr("xml"));
        openXmlFile();
        setPlayButtonsEnable(true);
//        updateRegionHeader(0);
        on_nextFrameButton_clicked();
//        updateVideoViewer();
        ui->actionClose->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->videoViewer->setEnabled(true);
    }
}

void MainWindow::on_actionSequence_triggered()
{
    SequenceDialog sequenceDialog(this);
    if (sequenceDialog.exec() == QDialog::Accepted) {
//        on_actionClose_triggered();
        QString path = sequenceDialog.getPath();
        QString prefix = sequenceDialog.getPrefix();
        QString extension = sequenceDialog.getExtension();
        int paddingLength = sequenceDialog.getPaddingLength();
        int firstNumber = sequenceDialog.getFirstNumber();
        video = new VideoProcessor(path  + tr("/"), prefix, extension, paddingLength, firstNumber);
        QStringList list = path.split("/");
        QString folderName = list.at(list.size() - 1);
        xmlFilePath = path + "/" + (folderName.isEmpty() ? tr("Untitled") : folderName) + tr(".xml");
        openXmlFile();
        setPlayButtonsEnable(true);
        updateRegionHeader(firstNumber);
        on_nextFrameButton_clicked();
//        updateVideoViewer();
        ui->actionClose->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->videoViewer->setEnabled(true);
        ui->zeroPositionLabel->setText(QString::number(firstNumber));
    } else {
        ui->statusBar->showMessage(tr("Open Sequence Canceled"));
    }

}

void MainWindow::on_actionClose_triggered()
{
    if (video) {
        delete video;
        video = NULL;
    }
    if (header) {
        delete header;
        header = NULL;
    }
    writeXmlFile();
    setInitState();
    ui->actionClose->setEnabled(false);
    ui->actionVideo->setEnabled(true);
    ui->actionSave->setEnabled(false);
}

void MainWindow::setShape(Shapes shape)
{
    this->shape = shape;
    QString usingShapeString;
    switch (shape) {
    case Shapes::Undefine:
        usingShapeString = tr("Change Undefine");
        break;
    case Shapes::Rectangle:
        usingShapeString = tr("Change Rectangle");
        break;
    case Shapes::Circle:
        usingShapeString = tr("Change Circle");
        break;
    case Shapes::Ellipse:
        usingShapeString = tr("Change Ellipse");
        break;
    default:
        usingShapeString = tr("Change Shape ERROR!!");
        break;
    }
    updateShapeButtons();
    ui->videoViewer->setShape(shape);
    ui->statusBar->showMessage(usingShapeString);
}

void MainWindow::on_rectangleButton_clicked()
{
    setShape(Shapes::Rectangle);
}

void MainWindow::on_ellipseButton_clicked()
{
    setShape(Shapes::Ellipse);
}

void MainWindow::on_circleButton_clicked()
{
    setShape(Shapes::Circle);
}

bool MainWindow::openXmlFile(void)
{
    if (xmlFilePath.isEmpty()) {
        ui->statusBar->showMessage(tr("XML File Path is empty"));
        return false;
    }

    if (QFile::exists(xmlFilePath)) {
        if (readXmlFile()) {
            ui->readonlyCheckBox->setCheckState(Qt::Checked);
        } else {
            ui->statusBar->showMessage(tr("Reading XML File ERROR!!"));
        }
    } else {
        if (!createXmlFile()) {
            ui->statusBar->showMessage(tr("Create XML File ERROR!!"));
            return false;
        }
        readXmlFile();
        ui->readonlyCheckBox->setCheckState(Qt::Unchecked);
    }

    return true;
}

bool MainWindow::createXmlFile(void)
{
    //setting XML declaration
    QDomProcessingInstruction xmlDeclaration = xmlDocument.createProcessingInstruction(tr("xml"), tr("version=\"1.0\" encoding=\"UTF-8\""));
    xmlDocument.appendChild(xmlDeclaration);
    //setting video name tag
    QDomText videoNameText = xmlDocument.createTextNode(video->getFileName());
    QDomElement videoNameTag = xmlDocument.createElement(tr(VIDEO_FILE_NAME_TAG));
    videoNameTag.appendChild(videoNameText);
    //setting create date tag
    QDomElement createDateTag = xmlDocument.createElement(tr(CREATE_DATE_TAG));
    QDate currentDate = QDate::currentDate();
    QDomText createDateText = xmlDocument.createTextNode(currentDate.toString("yyyy/MM/dd"));
    createDateTag.appendChild(createDateText);
    //setting lastest modify date tag
    QDomElement lastModifyDateTag = xmlDocument.createElement(tr(LATEST_MODIFY_DATE_TAG));
    //setting file description tag
    QDomElement fileDescriptionTag = xmlDocument.createElement(tr(FILE_DESCRIPTION_TAG));
    fileDescriptionTag.appendChild(videoNameTag);
    fileDescriptionTag.appendChild(createDateTag);
    fileDescriptionTag.appendChild(lastModifyDateTag);
    //setting header tag with frame 0
    QDomElement headerTag = xmlDocument.createElement(tr(HEADER_TAG));
    headerTag.setAttribute(tr(FRAME_NUMBER_ATTRIBUTE), 0);
    headerTag.setAttribute(tr(TOTAL_RECORD_ATTRIBUTE), 0);
    //setting date set tag
    QDomElement dataSetTag = xmlDocument.createElement(tr(DATA_SET_TAG));
    dataSetTag.appendChild(headerTag);
    //setting root tag
    QDomElement rootTag = xmlDocument.createElement(tr(ROOT_TAG));
    rootTag.appendChild(fileDescriptionTag);
    rootTag.appendChild(dataSetTag);
    xmlDocument.appendChild(rootTag);
    hasModify = true;
    if (!writeXmlFile()) {
        return false;
    }
    return true;
}

bool MainWindow::readXmlFile(void)
{
    xmlDocument = QDomDocument();
    QFile file(xmlFilePath);
    file.open(QIODevice::ReadOnly);
    if (!xmlDocument.setContent(&file)) {
        file.close();
        ui->statusBar->showMessage(tr("Open XML File ERROR!!"));
        return false;
    }
    QDomElement fileDescriptionTag = xmlDocument.elementsByTagName(tr(FILE_DESCRIPTION_TAG)).at(0).toElement();
    QDomNodeList children = fileDescriptionTag.childNodes();
    ui->videoNameLabel->setText(children.at(0).toElement().text());
    ui->createDateLabel->setText(children.at(1).toElement().text());
    QString modifyDate = children.at(2).toElement().text();
    ui->modifyDateLabel->setText(modifyDate.isEmpty() ? tr("None") : modifyDate);
    return true;
}

bool MainWindow::writeXmlFile()
{
//    if (!hasModify) {
//        return false;
//    }
    if (xmlFilePath.isEmpty()) {
        return false;
    }
    //edit latest modify date text
    QDomText latestModifyDateText = xmlDocument.createTextNode(QDate::currentDate().toString(tr(DATE_FORMAT)));
    QDomElement latestModifyDateTag = xmlDocument.elementsByTagName(tr(LATEST_MODIFY_DATE_TAG)).at(0).toElement();
    latestModifyDateTag.removeChild(latestModifyDateTag.childNodes().at(0));
    latestModifyDateTag.appendChild(latestModifyDateText);
    if (header) {
        //transform region header to xml dom tree
        int frameNumber = header->getFrameNumber();
        int totalRegion = header->getTotalRegion();
        QDomElement newHeaderTag = xmlDocument.createElement(tr(HEADER_TAG));
        newHeaderTag.setAttribute(tr(FRAME_NUMBER_ATTRIBUTE), frameNumber);
        newHeaderTag.setAttribute(tr(TOTAL_RECORD_ATTRIBUTE), totalRegion);
        Region *region = header->getFirstRegion();
        while (region) {
            //setting shape tag
            QString shapeString;
            switch (region->getShape()) {
            case Shapes::Rectangle:
                shapeString = tr("rectangle");
                break;
            case Shapes::Ellipse:
                shapeString = tr("ellipse");
                break;
            case Shapes::Circle:
                shapeString = tr("circle");
                break;
            default:
                shapeString = tr("unknow");
                break;
            }
            QDomElement shapeTag = xmlDocument.createElement(tr(SHAPE_TAG));
            shapeTag.appendChild(xmlDocument.createTextNode(shapeString));
            //setting start point tag
            QDomElement startPointTag = xmlDocument.createElement(START_POINT_TAG);
            startPointTag.appendChild(xmlDocument.createTextNode(QString::number(region->getStartPoint().x()) + ", " +
                                                                 QString::number(region->getStartPoint().y()))
                                      );
            //setting height tag
            QDomElement heightTag = xmlDocument.createElement(tr(HEIGHT_TAG));
            heightTag.appendChild(xmlDocument.createTextNode(QString::number(region->getHeight())));
            //setting width tag
            QDomElement widthTag = xmlDocument.createElement(tr(WIDTH_TAG));
            widthTag.appendChild(xmlDocument.createTextNode(QString::number(region->getWidth())));
            //setting record tag
            QDomElement recordTag = xmlDocument.createElement(tr(RECORD_TAG));
            recordTag.setAttribute(tr(SERIAL_NUMBER_ATTRIBUTE), region->getSerialNumber());
            recordTag.appendChild(shapeTag);
            recordTag.appendChild(startPointTag);
            recordTag.appendChild(heightTag);
            recordTag.appendChild(widthTag);
            newHeaderTag.appendChild(recordTag);
            region = region->getNextRegion();
        }
        //insert header tag to suitable position
        QDomElement dataSetTag = xmlDocument.documentElement().firstChildElement(tr(DATA_SET_TAG));
        QDomNodeList headerTagList = xmlDocument.elementsByTagName(tr(HEADER_TAG));
        QDomElement originalHeaderTag;
        for (int i = 0; i < headerTagList.length(); i++){
            QDomElement headerTag = headerTagList.at(i).toElement();
            if (headerTag.attribute(tr(FRAME_NUMBER_ATTRIBUTE)).toInt() >= frameNumber) {
                originalHeaderTag = headerTag;
                break;
            }
        }
        if (originalHeaderTag.isNull()) {
            dataSetTag.appendChild(newHeaderTag);
        } else {
            if (originalHeaderTag.attribute(tr(FRAME_NUMBER_ATTRIBUTE)).toInt() == frameNumber) {
                dataSetTag.insertAfter(newHeaderTag, originalHeaderTag);
                dataSetTag.removeChild(originalHeaderTag);
            } else {
                dataSetTag.insertBefore(newHeaderTag, originalHeaderTag);
            }
        }
    }

    //open text file
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.close();
        return false;
    }
    //write down xml document
    QTextStream fileStream(&file);
    fileStream.setCodec("UTF-8");
    fileStream.setGenerateByteOrderMark(true);
    fileStream << xmlDocument.toString();
//    fileStream.flush();
    file.close();
    hasModify = false;
    return true;
}

void MainWindow::updateRegionHeader(int frameNumber)
{
    if (header) {
        delete header;
    }
    header = new RegionHeader(frameNumber);

    QDomElement headerTag = xmlDocument.documentElement().firstChildElement(tr(DATA_SET_TAG)).firstChildElement(tr(HEADER_TAG));
    while (!headerTag.isNull()) {
        int frameNumberAttribute = headerTag.attribute(tr(FRAME_NUMBER_ATTRIBUTE)).toInt();
        if (frameNumberAttribute == frameNumber) {
            QDomNodeList recordList = headerTag.elementsByTagName(tr(RECORD_TAG));
            for (int i = 0; i < recordList.length(); i++) {
                QDomElement recordTag = recordList.at(i).toElement();
                int serialNumberAttrute = recordTag.attribute(tr(SERIAL_NUMBER_ATTRIBUTE)).toInt();
                QString shapeText = recordTag.firstChildElement(tr(SHAPE_TAG)).text();
                QString startPointText = recordTag.firstChildElement(tr(START_POINT_TAG)).text();
                QString heightText = recordTag.firstChildElement(tr(HEIGHT_TAG)).text();
                QString widthText = recordTag.firstChildElement(tr(WIDTH_TAG)).text();
                QStringList position =  startPointText.split(tr(", "));
                QString startPointXString = position[0];
                QString startPointYString = position[1];
                QPoint startPoint(startPointXString.toInt(), startPointYString.toInt());
                if (shapeText == tr("rectangle")) {
                    header->addRectangularRegion(serialNumberAttrute,
                                                 startPoint,
                                                 widthText.toInt(),
                                                 heightText.toInt()
                                                 );
                } else if (shapeText == tr("ellipse")) {
                    header->addEllipticalRegion(serialNumberAttrute,
                                                startPoint,
                                                widthText.toInt(),
                                                heightText.toInt()
                                                );
                } else if (shapeText == tr("circle")) {
                    header->addCircularRegion(serialNumberAttrute,
                                              startPoint,
                                              widthText.toInt()
                                              );
                }
            }
            break;
        } else if (frameNumberAttribute > frameNumber) {
            break;
        }
        headerTag = headerTag.nextSiblingElement();
    }
}


void MainWindow::updateResultTable(int frameNumber)
{
    if (frameNumber <= -1) {
        ui->statusBar->showMessage(tr("Wrong Frame Number!!"));
        return;
    }
    if (!header && header->getFrameNumber() != frameNumber) {
        return;
    }

    ui->resultTable->clearContents();
    ui->resultTable->setRowCount(header->getTotalRegion());
    Region *region = header->getFirstRegion();
    int row = 0;
    while (region) {
        QTableWidgetItem *serialNumberItem = new QTableWidgetItem(QString::number(region->getSerialNumber()));
        QTableWidgetItem *startPointXItem = new QTableWidgetItem(QString::number(region->getStartPoint().x()));
        QTableWidgetItem *startPointYItem = new QTableWidgetItem(QString::number(region->getStartPoint().y()));
        QTableWidgetItem *heightItem = new QTableWidgetItem(QString::number(region->getHeight()));
        QTableWidgetItem *widthItem = new QTableWidgetItem(QString::number(region->getWidth()));
        QString shapeString;
        switch (region->getShape()) {
        case Shapes::Rectangle:
            shapeString = tr("rectangle");
            break;
        case Shapes::Ellipse:
            shapeString = tr("ellipse");
            break;
        case Shapes::Circle:
            shapeString = tr("circle");
            break;
        default:
            shapeString = tr("unknow");
            break;
        }
        QTableWidgetItem *shapeItem = new QTableWidgetItem(shapeString);
        ui->resultTable->setItem(row, 0, serialNumberItem);
        ui->resultTable->setItem(row, 1, startPointXItem);
        ui->resultTable->setItem(row, 2, startPointYItem);
        ui->resultTable->setItem(row, 3, widthItem);
        ui->resultTable->setItem(row, 4, heightItem);
        ui->resultTable->setItem(row, 5, shapeItem);
        row++;
        region = region->getNextRegion();
    }
}

void MainWindow::on_nextFrameButton_clicked()
{
    writeXmlFile();
    if (video->queryNextFrame()) {
        int frameNumber = video->getFrameNumber();
//        if (header) {
//            delete header;
//        }
//        header = new RegionHeader(frameNumber);
        updateRegionHeader(frameNumber);
        updateVideoViewer();
        if (ui->positionSlider->maximum() <= frameNumber) {
            ui->positionSlider->setRange(ui->zeroPositionLabel->text().toInt(), frameNumber);
        }
        hasModify = false;
        ui->positionSlider->setValue(frameNumber);
    } else {
        isPlaying = false;
        ui->statusBar->showMessage(tr("Querying Next Frame ERROR!"));
    }
}

void MainWindow::on_preFrameButton_clicked()
{
    writeXmlFile();
    if (video->queryPreFrame()) {
        int frameNumber = video->getFrameNumber();
//        if (header) {
//            delete header;
//        }
//        header = new RegionHeader(frameNumber);
        updateRegionHeader(frameNumber);
        ui->positionSlider->setValue(frameNumber);
        hasModify = false;
    } else {
        ui->statusBar->showMessage(tr("Querying Previous Frame ERROR!"));
    }
}

void MainWindow::on_resultTable_itemClicked(QTableWidgetItem *item)
{
    //show the selected region
    int selectedRow = item->row();
    int columnCount = ui->resultTable->columnCount();
    QStringList rowDatas;
    for (int i = 0; i < columnCount; i++) {
        rowDatas << ui->resultTable->item(selectedRow, i)->text();
    }
    /*  Sn    x     y   width   height  shape
     *  0     1     2     3       4       5
     */

    if (!isReadonly) {
        header->sendCleanMessage();
        header->hitRegion(rowDatas[0].toInt());
        updateVideoViewer();
    }
}

void MainWindow::on_readonlyCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        isReadonly = true;
    } else if (arg1 == Qt::Unchecked) {
        isReadonly = false;
    }

    setModifyButtonsEnable(!isReadonly);
    ui->videoViewer->setEnabledDrawPreviewRegion(!isReadonly);
}

void MainWindow::on_playPauseButton_clicked()
{
    isPlaying = !isPlaying;
    if (isPlaying) {
        timer->start(defaultWaitingTime);
    }
}

void MainWindow::playVideo()
{
    on_nextFrameButton_clicked();
    if (!isPlaying) {
        timer->stop();
    }
}

void MainWindow::on_replayButton_clicked()
{
    ui->statusBar->showMessage(tr("Replay"));
    ui->positionSlider->setValue(0);
    isPlaying = false;
    on_playPauseButton_clicked();
}

void MainWindow::disableAllButtons(void)
{
    setPlayButtonsEnable(false);
    setDirectionButtonsEnable(false);
    setNodeOperationButtinsEnable(false);
}

void MainWindow::enableAllButtons(void)
{
    setPlayButtonsEnable(true);
    setDirectionButtonsEnable(true);
    setNodeOperationButtinsEnable(true);
}

void MainWindow::setPlayButtonsEnable(bool enabled)
{
    ui->playPauseButton->setEnabled(enabled);
    ui->preFrameButton->setEnabled(enabled);
    ui->nextFrameButton->setEnabled(enabled);
    ui->replayButton->setEnabled(enabled);
}

void MainWindow::setDirectionButtonsEnable(bool enabled)
{
    ui->upButton->setEnabled(enabled);
    ui->downButton->setEnabled(enabled);
    ui->leftButton->setEnabled(enabled);
    ui->rightButton->setEnabled(enabled);
}

void MainWindow::setNodeOperationButtinsEnable(bool enabled)
{
    ui->deleteButton->setEnabled(enabled);
    ui->cloneButton->setEnabled(enabled);
}

void MainWindow::setModifyButtonsEnable(bool enabled)
{
    setDirectionButtonsEnable(enabled);
    setNodeOperationButtinsEnable(enabled);
}

void MainWindow::updateShapeButtons(void)
{
    ui->rectangleButton->setEnabled(this->shape == Shapes::Rectangle ? false : true);
    ui->ellipseButton->setEnabled(this->shape == Shapes::Ellipse ? false : true);
    ui->circleButton->setEnabled(this->shape == Shapes::Circle ? false : true);
}

void MainWindow::setInitState()
{
    setShape(Shapes::Rectangle);
    ui->videoViewer->setEnabled(false);
    disableAllButtons();
    ui->positionSlider->setRange(0, 0);
    isPlaying = false;
    hasModify = false;
    video = NULL;
    header = NULL;
    ui->resultTable->setRowCount(0);
    ui->videoViewer->clear();
    ui->videoViewer->setText(tr("Video Viewer"));
    ui->videoNameLabel->setText(tr("None"));
    ui->createDateLabel->setText(tr("None"));
    ui->modifyDateLabel->setText(tr("None"));
    ui->zeroPositionLabel->setText(QString::number(0));

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //ui->statusBar->showMessage(QString("%1 Pressed").arg(event->text()));
    int index = -1;
    Region* region = nullptr;
    if (header != nullptr) {
        index = header->getAllSelecedRegionID();
        region = header->getRegion(index);
    }
    switch (event->key()) {
    case KEY_CANCLE:
        ui->deleteButton->click();
        break;
    case KEY_NEXT_FRAME:

        ui->nextFrameButton->click();
        break;
    case KEY_PREVIOUS_FRAME:
        ui->preFrameButton->click();
        break;
    case Qt::Key_A:
        if (index == -1) {
            break;
        }
        switch (region->getShape()) {
        case Shapes::Rectangle:
            ui->videoViewer->clicked(region->getStartPoint() + QPoint(0, region->getHeight() / 2));
            break;
        default:
            break;
        }
        break;
    case Qt::Key_D:
        if (index == -1) {
            break;
        }
        switch (region->getShape()) {
        case Shapes::Rectangle:
            ui->videoViewer->clicked(region->getStartPoint() + QPoint(region->getWidth(), region->getHeight() / 2));
            break;
        default:
            break;
        }
        break;
    case Qt::Key_W:
        if (index == -1) {
            break;
        }
        switch (region->getShape()) {
        case Shapes::Rectangle:
            ui->videoViewer->clicked(region->getStartPoint() + QPoint(region->getWidth() / 2, 0));
            break;
        default:
            break;
        }
        break;
    case Qt::Key_S:
        if (index == -1) {
            break;
        }
        switch (region->getShape()) {
        case Shapes::Rectangle:
            ui->videoViewer->clicked(region->getStartPoint() + QPoint(region->getWidth() / 2, region->getHeight()));
            break;
        default:
            break;
        }
        break;
    case KEY_LEFT:
        ui->leftButton->click();
        break;
    case KEY_RIGHT:
        ui->rightButton->click();
        break;
    case KEY_UP:
        ui->upButton->click();
        break;
    case KEY_DOWN:
        ui->downButton->click();
        break;
    case KEY_PLAY_PAUSE:
        ui->playPauseButton->click();
        break;
    case KEY_SHAPE_1:
        ui->rectangleButton->click();
        break;
    case KEY_SHAPE_2:
        ui->ellipseButton->click();
        break;
    case KEY_SHAPE_3:
        ui->circleButton->click();
    default:
        break;
    }
}

void MainWindow::on_positionSlider_valueChanged(int value)
{
    if (video == nullptr) {
        return;
    }
    video->jumpFrame(value);
    updateRegionHeader(value);
    updateVideoViewer();
}

void MainWindow::updateVideoViewer(void)
{
    video->cleanBuffer();
    int frameNumber = video->getFrameNumber();
    ui->frameNumberLabel->setText(tr("Frame Number : ") + QString::number(frameNumber));
    video->drawRegion(header);
    ui->videoViewer->setPixmap(video->getFrame());
    updateResultTable(frameNumber);
}

void MainWindow::on_positionSlider_rangeChanged(int, int max)
{
    ui->endFrameLabel->setText(QString::number(max));
}

void MainWindow::on_actionSave_triggered()
{
    if (isReadonly) {
        QMessageBox::warning(this, tr("Can Not Save XML File!!"), tr("This Program Is In Readonly Mode!\nPlease Uncheck The Readonly Checkbox First."));
        return;
    }
    if (!hasModify) {
        ui->statusBar->showMessage(tr("There Is No Change To Save"));
        return;
    }
    if (!writeXmlFile()) {
        QMessageBox::StandardButton check = QMessageBox::question(this, tr("ERROR when saving XML File"), tr("Do You Want To Save As New XML File?"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, QMessageBox::StandardButton::No);
        if (check == QMessageBox::StandardButton::Yes) {
            //save new file
        }
    }
}

void MainWindow::on_upButton_clicked()
{
    hasModify = true;
    header->sendModifyMessage(0, -1);
    updateVideoViewer();
}

void MainWindow::on_downButton_clicked()
{
    hasModify = true;
    header->sendModifyMessage(0, 1);
    updateVideoViewer();
}

void MainWindow::on_leftButton_clicked()
{
    hasModify = true;
    header->sendModifyMessage(-1, 0);
    updateVideoViewer();
}

void MainWindow::on_rightButton_clicked()
{
    hasModify = true;
    header->sendModifyMessage(1, 0);
    updateVideoViewer();
}

void MainWindow::on_deleteButton_clicked()
{
    header->sendRemoveMessage();
    updateVideoViewer();
    hasModify = true;
}

void MainWindow::on_cloneButton_clicked()
{
    writeXmlFile();
    if (video->queryNextFrame()) {
        int frameNumber = video->getFrameNumber();
        RegionHeader *tmp = header;
        header = new RegionHeader(frameNumber, tmp);
        delete tmp;
        if (ui->positionSlider->maximum() <= frameNumber) {
            ui->positionSlider->setRange(0, frameNumber);
        }
        hasModify = true;
        writeXmlFile();
        ui->positionSlider->setValue(frameNumber);
        ui->statusBar->showMessage(tr("Querying Next Frame Success"));
    } else {
        ui->statusBar->showMessage(tr("Querying Next Frame ERROR!"));
    }
}

void MainWindow::on_videoViewer_clicked(const QPoint point)
{
    if (isReadonly || !header) {
        return;
    }

    header->sendHitMessage(QPoint(point.x() * video->getWidthRatio(), point.y() * video->getHeightRatio()));
    updateVideoViewer();
    ui->statusBar->showMessage(tr("Clicked At (") + QString::number(point.x()) + "," + QString::number(point.y()) + tr(")"));
}

void MainWindow::on_videoViewer_drawNewRegion(const QPoint& leftTop, const QPoint& rightBottom)
{
    if (isReadonly) {
        ui->statusBar->showMessage(tr("Please Unlock The Readonly Mode First"));
        return;
    }
    double widthRatio = video->getWidthRatio();
    double heightRatio = video->getHeightRatio();
    QPoint lt = QPoint(leftTop.x() * widthRatio, leftTop.y() * heightRatio);
    QPoint rb = QPoint(rightBottom.x() * widthRatio, rightBottom.y() * heightRatio);
    QPoint center((lt.x() + rb.x()) / 2,
                  (lt.y() + rb.y()) / 2);
    int width = (rb.x() - lt.x());
    int height = (rb.y() - lt.y());
    int radius = 0;
    int serialNumber = header->getUnusedSerialNumber();
    switch (shape) {
    case Shapes::Circle:
        radius = (int)(sqrt(SQUARE(width) + SQUARE(height)) / 2);
        header->addCircularRegion(serialNumber, center, radius);
        break;
    case Shapes::Ellipse:
        header->addEllipticalRegion(serialNumber, center, width / 2, height / 2);
        break;
    case Shapes::Rectangle:
        header->addRectangularRegion(serialNumber, lt, width, height);
        break;
    default:
        break;
    }
    header->sendCleanMessage();
    header->hitRegion(serialNumber);

    hasModify = true;
    updateVideoViewer();
}
