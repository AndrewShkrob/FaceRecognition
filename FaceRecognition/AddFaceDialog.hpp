#ifndef ADDFACEDIALOG_HPP
#define ADDFACEDIALOG_HPP

#include <QDialog>
#include <QThread>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "AddFaceWorker.hpp"

namespace Ui {
class AddFaceDialog;
}

class AddFaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFaceDialog(QWidget *parent = 0);
    ~AddFaceDialog();

private:
    Ui::AddFaceDialog *ui;
    QThread *_thread;

    void setup();
    void setupUI();

signals:
    void sendSetup(const StreamType streamType, const QString path = "");
    void sendChangeStreamStatus(StreamStatus status);

    void sendAddFace(const QString name);

private slots:
    void receiveFrame(const QImage frame);

    void receiveButtonUseWebcam();
    void receiveButtonOpenImage();
    void receiveButtonCancel();
    void receiveButtonAccept();

    void receiveMoreThanOneFaceDetected();
    void receiveNoFaceDetected();
    void receiveNoFaceDetectedWithoutMessage();
    void receiveFaceDetected();

};

#endif // ADDFACEDIALOG_HPP
