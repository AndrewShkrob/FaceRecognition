#-------------------------------------------------
#
# Project created by QtCreator 2018-06-02T13:41:02
#
#-------------------------------------------------

LIBS     += -lboost_filesystem -lboost_system
LIBS     += -lm -llapack -lblas
LIBS     +=  -ldlib -lpng16 -lz -L/usr/local/cuda/lib64 -lcudnn -lpthread -lcuda -lcudart -lcublas -lcurand -lcusolver
LIBS     += `pkg-config --cflags --libs opencv`
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceRecognition
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    Worker.cpp \
    PersonBaseHelper.cpp \
    FaceDetector.cpp \
    FaceRecognizer.cpp \
    AddFaceDialog.cpp \
    AddFaceWorker.cpp \
    PersonBaseEntity.cpp \
    DetectedFace.cpp \
    Settings.cpp

HEADERS += \
        MainWindow.hpp \
    Worker.hpp \
    PersonBaseHelper.hpp \
    FaceDetector.hpp \
    FaceRecognizer.hpp \
    ResNetwork.hpp \
    AddFaceDialog.hpp \
    AddFaceWorker.hpp \
    PersonBaseEntity.hpp \
    DetectedFace.hpp \
    Settings.hpp

FORMS += \
        main_window.ui \
    add_face_dialog.ui
