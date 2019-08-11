TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        FaceDetection/FaceDetection.cpp \
        InfoPrediction/InfoPrediction.cpp \
        functions.cpp \
        main.cpp

INCLUDEPATH += $$PWD/../include
LIBS += -L$$PWD/../libs/ \
        -lpthread \
        -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs \
        -lopencv_dnn \
        -lfacedetection

HEADERS += \
    FaceDetection/FaceDetection.h \
    InfoPrediction/InfoPrediction.h \
    functions.h
