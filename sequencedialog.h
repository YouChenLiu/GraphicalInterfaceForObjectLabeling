#ifndef SEQUENCEDIALOG_H
#define SEQUENCEDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include "MainWindow.h"

namespace Ui {
    class SequenceDialog;
}

class SequenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SequenceDialog(QWidget *parent = 0);
    ~SequenceDialog();

    QString getPath(void) const;

    QString getPrefix(void) const;

    QString getExtension(void) const;

    int getPaddingLength(void) const;

    int getFirstNumber(void) const;

private slots:
    void on_browsingButton_clicked();

    void on_prefixEdit_textChanged(const QString &arg1);

    void on_extensionCombo_currentTextChanged(const QString &arg1);

    void on_paddingSpin_valueChanged(int arg1);

    void on_firstNumberSpin_valueChanged(int arg1);

    void on_pathEdit_textChanged(const QString &arg1);

private:
    Ui::SequenceDialog *ui;

    QString path;
    QString prefix;
    QString extension;
    int paddingLength;
    int firstNumber;

private:
    void updatePreviewLabel(void);

};

#endif // SEQUENCEDIALOG_H
