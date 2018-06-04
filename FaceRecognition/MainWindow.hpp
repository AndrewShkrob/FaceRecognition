#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QFileDialog>

#include "Worker.hpp"
#include "AddFaceDialog.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QThread *_thread;

    void setup();
    void setupUI();

signals:
    void sendSetup(const StreamType streamType, const QString path = "");
    void sendChangeStreamStatus(StreamStatus status);

private slots:
    void receiveFrame(const QImage frame);
    void receiveChangeStreamStatus(const StreamStatus status);

    void receiveButtonOpenWebcam();
    void receiveButtonOpenFile();
    void receiveButtonAddFace();
};

#endif // MAINWINDOW_HPP
