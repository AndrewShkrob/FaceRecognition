#include "MainWindow.hpp"
#include "ui_main_window.h"

// PUBLIC
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setupUI();
    setup();
}

MainWindow::~MainWindow()
{
    _thread->quit();

    while (!_thread->isFinished());

    delete _thread;
    delete ui;
}

// PRIVATE
void MainWindow::setup()
{
    _thread = new QThread();
    Worker *worker = new Worker();
    QTimer *workerTrigger = new QTimer();
    workerTrigger->setInterval(0);

    qRegisterMetaType<StreamType>("StreamType");
    qRegisterMetaType<StreamStatus>("StreamStatus");

    connect(workerTrigger, SIGNAL(timeout()), worker, SLOT(receiveGrabFrame()));
    connect(_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(_thread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));

    connect(this, SIGNAL(sendSetup(StreamType, QString)), worker, SLOT(receiveSetup(StreamType, QString)));
    connect(this, SIGNAL(sendChangeStreamStatus(StreamStatus)), worker, SLOT(receiveChangeStreamStatus(StreamStatus)));

    connect(ui->button_open_web_cam, SIGNAL(clicked(bool)), this, SLOT(receiveButtonOpenWebcam()));
    connect(ui->button_open_file, SIGNAL(clicked(bool)), this, SLOT(receiveButtonOpenFile()));
    connect(ui->button_add_face, SIGNAL(clicked(bool)), this, SLOT(receiveButtonAddFace()));

    connect(worker, SIGNAL(sendFrame(QImage)), this, SLOT(receiveFrame(QImage)));

    workerTrigger->start();
    worker->moveToThread(_thread);
    workerTrigger->moveToThread(_thread);

    _thread->start();

    emit sendSetup(WEBCAM, "/dev/video0");
}

void MainWindow::setupUI()
{
    ui->setupUi(this);
}

// SLOTS
void MainWindow::receiveFrame(const QImage frame)
{
    ui->frame->setPixmap(QPixmap::fromImage(frame));
}

void MainWindow::receiveChangeStreamStatus(const StreamStatus status)
{
    emit sendChangeStreamStatus(status);
}

void MainWindow::receiveButtonOpenWebcam()
{
    emit sendSetup(WEBCAM, "/dev/video0");
}

void MainWindow::receiveButtonOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("All supported files (*.bmp *.dib *.jpg *.jpeg *.jpe *.tiff *tif *.avi *.mp4);;"
                   "Windows bitmaps (*.bmp *.dib);;"
                   "JPEG files (*.jpg *.jpeg *.jpe);;"
                   "TIFF files (*.tiff *tif);;"
                   "AVI Files (*.avi);;"
                   "MP4 Files (*.mp4)")
    );

    if (fileName.isEmpty())
    {
        return;
    }

    emit sendSetup(FILe, fileName);
}

void MainWindow::receiveButtonAddFace()
{
    emit sendSetup(EMPTY);
    AddFaceDialog *fd = new AddFaceDialog(this);
    fd->setModal(true);
    fd->exec();
    delete fd;
    emit sendSetup(WEBCAM, "/dev/video0");
}
