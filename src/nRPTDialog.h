#ifndef NRPTDIALOG_H
#define NRPTDIALOG_H

#include <QDialog>
class QAbstractButton;
class nRPTWorker;

namespace Ui {
    class nRPTDialog;
}

class nRPTDialog : public QDialog
{
    Q_OBJECT

public:
    explicit nRPTDialog(QString srcDir, QString dstDir, bool automatic, QWidget *parent = 0);
    ~nRPTDialog();


    Ui::nRPTDialog *ui;

public slots:
    void updateProgressBarValue(int value);
    void appendOutput(QString output);
    void updateCheckBox(int check, int state);

    void doStuff();
    void timerEvent(QTimerEvent *);

private slots:
    void on_buttonBox_clicked(QAbstractButton* button);

private:
    QString m_srcDir, m_dstDir;
    bool m_automatic;
    int m_autoTimerId;
    nRPTWorker * m_worker;
};

#endif // NRPTDIALOG_H
