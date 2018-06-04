#include "AddFaceDialog.hpp"
#include "ui_add_face_dialog.h"

// PUBLIC
AddFaceDialog::AddFaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFaceDialog)
{
    setupUI();
    setup();
}

AddFaceDialog::~AddFaceDialog()
{
    _thread->quit();

    while (!_thread->isFinished());

    delete _thread;
    delete ui;
}

// PRIVATE
void AddFaceDialog::setup()
{
    _thread = new QThread();
    AddFaceWorker *worker = new AddFaceWorker();
    QTimer *workerTrigger = new QTimer();
    workerTrigger->setInterval(0);

    connect(workerTrigger, SIGNAL(timeout()), worker, SLOT(receiveGrabFrame()));
    connect(_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(_thread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));

    connect(this, SIGNAL(sendSetup(StreamType, QString)), worker, SLOT(receiveSetup(StreamType, QString)));
    connect(this, SIGNAL(sendChangeStreamStatus(StreamStatus)), worker, SLOT(receiveChangeStreamStatus(StreamStatus)));
    connect(this, SIGNAL(sendAddFace(QString)), worker, SLOT(receiveAddFace(QString)));

    connect(ui->button_use_webcam, SIGNAL(clicked(bool)), this, SLOT(receiveButtonUseWebcam()));
    connect(ui->button_open_image, SIGNAL(clicked(bool)), this, SLOT(receiveButtonOpenImage()));
    connect(ui->button_cancel, SIGNAL(clicked(bool)), this, SLOT(receiveButtonCancel()));
    connect(ui->button_accept, SIGNAL(clicked(bool)), this, SLOT(receiveButtonAccept()));

    connect(worker, SIGNAL(sendFrame(QImage)), this, SLOT(receiveFrame(QImage)));
    connect(worker, SIGNAL(sendMoreThanOneFaceDetected()), this, SLOT(receiveMoreThanOneFaceDetected()));
    connect(worker, SIGNAL(sendNoFaceDetected()), this, SLOT(receiveNoFaceDetected()));
    connect(worker, SIGNAL(sendNoFaceDetectedWithoutMessage()), this, SLOT(receiveNoFaceDetectedWithoutMessage()));
    connect(worker, SIGNAL(sendFaceDetected()), this, SLOT(receiveFaceDetected()));

    workerTrigger->start();
    worker->moveToThread(_thread);
    workerTrigger->moveToThread(_thread);

    _thread->start();

    emit sendSetup(EMPTY);
}

void AddFaceDialog::setupUI()
{
    ui->setupUi(this);
}

// PRIVATE SLOTS
void AddFaceDialog::receiveFrame(const QImage frame)
{
    ui->frame->setPixmap(QPixmap::fromImage(frame));
}

void AddFaceDialog::receiveButtonUseWebcam()
{
    emit sendSetup(WEBCAM, "/dev/video0");
}

void AddFaceDialog::receiveButtonOpenImage()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("All supported files (*.bmp *.dib *.jpg *.jpeg *.jpe *.tiff *tif);;"
                   "Windows bitmaps (*.bmp *.dib);;"
                   "JPEG files (*.jpg *.jpeg *.jpe);;"
                   "TIFF files (*.tiff *tif)")
    );

    if (fileName.isEmpty())
    {
        return;
    }

    emit sendSetup(FILe, fileName);
}

void AddFaceDialog::receiveButtonCancel()
{
    destroy();
}

void AddFaceDialog::receiveButtonAccept()
{
    if (ui->edit_name->text().size() == 0)
    {
        QMessageBox *emptyText = new QMessageBox(
                    QMessageBox::Critical,
                    "Ошибка",
                    "Не задано имя. ",
                    QMessageBox::Ok,
                    this);
        emptyText->show();
        return;
    }

    emit sendAddFace(ui->edit_name->text());
    emit sendSetup(EMPTY);
    destroy();
}

void AddFaceDialog::receiveMoreThanOneFaceDetected()
{
    QMessageBox *moreThenOneFaceDetectedMessage = new QMessageBox(
                QMessageBox::Critical,
                "Ошибка",
                "На изображении найдено больше одного лица. "
                "Пожалуйста, загрузите изображение с одним лицом.",
                QMessageBox::Ok,
                this);
    moreThenOneFaceDetectedMessage->show();

    ui->button_accept->setEnabled(false);
}

void AddFaceDialog::receiveNoFaceDetectedWithoutMessage()
{
    ui->button_accept->setEnabled(false);
}

void AddFaceDialog::receiveNoFaceDetected()
{
    QMessageBox *noFaceDetected = new QMessageBox(
                QMessageBox::Critical,
                "Ошибка",
                "На изображении не найдено ни одного лица. "
                "Пожалуйста, загрузите изображение с одним лицом.",
                QMessageBox::Ok,
                this);
    noFaceDetected->show();

    ui->button_accept->setEnabled(false);
}

void AddFaceDialog::receiveFaceDetected()
{
    ui->button_accept->setEnabled(true);
}
