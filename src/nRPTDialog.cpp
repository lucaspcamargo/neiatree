#include "nRPTDialog.h"
#include "nRPTWorker.h"
#include "ui_nRPTDialog.h"
#include <QScrollBar>

nRPTDialog::nRPTDialog(QString srcDir, QString dstDir, bool automatic, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::nRPTDialog)
{
    m_srcDir = srcDir;
    m_dstDir = dstDir;
    m_automatic = automatic;

    if(m_automatic) m_autoTimerId = startTimer(200);

    m_worker = 0;

    ui->setupUi(this);
    setWindowTitle(windowTitle()+" [auto]");

}

nRPTDialog::~nRPTDialog()
{
    delete ui;
}

void nRPTDialog::updateProgressBarValue(int value)
{
    ui->progressBar->setEnabled(true);
    ui->progressBar->setValue(value);
}

void nRPTDialog::updateCheckBox(int check, int state)
{
    QCheckBox * box = (check == 1 ? ui->progress1: check == 2 ? ui->progress2: check == 3 ? ui->progress3: check == 4 ? ui->progress4: 0 );
    if(box)
    {
        if(state==0)
        {
            box->setEnabled(true);
        }
        else if(state==1)
        {
            box->setCheckable(true);
            box->setChecked(true);
        }
        else
        {
            box->setCheckable(false);
            box->setEnabled(false);
        }
    }
}

void nRPTDialog::appendOutput(QString output)
{
    ui->output->appendPlainText(output);
    ui->output->verticalScrollBar()->setSliderPosition(ui->output->verticalScrollBar()->maximum());
}

#include <QMessageBox>
void nRPTDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if(ui->buttonBox->buttonRole(button)==QDialogButtonBox::ApplyRole)
        doStuff();
}

void nRPTDialog::doStuff()
{
    m_worker = new nRPTWorker(m_srcDir, m_dstDir, ui->threadsSpinBox->value(), this);
    ui->buttonBox->setEnabled(false);
    m_worker->start();
    while(m_worker->isRunning())
    {
        m_worker->wait(100);
        QApplication::processEvents();
    }
    ui->buttonBox->setEnabled(true);

    if(m_automatic) close();

}

void nRPTDialog::timerEvent(QTimerEvent * evt)
{
    Q_UNUSED(evt)

    killTimer(m_autoTimerId);
    doStuff();

}
