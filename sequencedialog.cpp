#include "sequencedialog.h"
#include "ui_sequencedialog.h"
#include <QMimeData>

SequenceDialog::SequenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceDialog)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    extension = ui->extensionCombo->itemText(0);
    paddingLength = ui->paddingSpin->value();
    firstNumber = ui->firstNumberSpin->value();
    updatePreviewLabel();
}

SequenceDialog::~SequenceDialog()
{
    delete ui;
}

QString SequenceDialog::getPath() const
{
    return path;
}

QString SequenceDialog::getPrefix() const
{
    return prefix;
}

QString SequenceDialog::getExtension() const
{
    return extension;
}

int SequenceDialog::getPaddingLength() const
{
    return paddingLength;
}

int SequenceDialog::getFirstNumber() const
{
    return firstNumber;
}

void SequenceDialog::dragEnterEvent(QDragEnterEvent* event)
{
  // if some actions should not be usable, like move, this code must be adopted
  event->acceptProposedAction();
}

void SequenceDialog::dragMoveEvent(QDragMoveEvent* event)
{
  // if some actions should not be usable, like move, this code must be adopted
  event->acceptProposedAction();
}

void SequenceDialog::dragLeaveEvent(QDragLeaveEvent* event)
{
  event->accept();
}

void SequenceDialog::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QString filePath = urlList.at(0).toLocalFile();
        int lastSlah = filePath.lastIndexOf(QChar('/'));
        QString path = filePath.left(lastSlah);
        ui->pathEdit->setText(path);
        QString fileName = filePath.right(filePath.length() - lastSlah - 1);
        int number = fileName.left(fileName.lastIndexOf(QChar('.'))).toInt();
        ui->firstNumberSpin->setValue(number);
    }
}

void SequenceDialog::on_browsingButton_clicked()
{
    ui->pathEdit->setText(QFileDialog::getExistingDirectory(this, tr("Open Directory")));
    updatePreviewLabel();
}

void SequenceDialog::on_prefixEdit_textChanged(const QString &arg1)
{
    prefix = arg1;
    updatePreviewLabel();
}

void SequenceDialog::on_extensionCombo_currentTextChanged(const QString &arg1)
{
    extension = arg1;
    updatePreviewLabel();
}

void SequenceDialog::on_paddingSpin_valueChanged(int arg1)
{
    paddingLength = arg1;
    updatePreviewLabel();
}

void SequenceDialog::on_firstNumberSpin_valueChanged(int arg1)
{
    firstNumber = arg1;
    updatePreviewLabel();
}

void SequenceDialog::updatePreviewLabel(void)
{
    ui->previewLabel->setText(path + tr("/") + prefix + QString("%1").arg(firstNumber, paddingLength, 10, QChar('0')) + tr(".") + extension);
}

void SequenceDialog::on_pathEdit_textChanged(const QString &arg1)
{
    path = arg1;
    path.replace(QString("\\"), QString("/"));
    updatePreviewLabel();
}
