TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        FaceDetection/FaceDetection.cpp \
        InfoPrediction/InfoPrediction.cpp \
        RobotEyes.cpp \
        functions.cpp \
        main.cpp

INCLUDEPATH += $$PWD/../include
LIBS += -L$$PWD/../libs/ \
        -lpthread \
        -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs \
        -lopencv_dnn  -lopencv_features2d -lopencv_flann -lopencv_calib3d\
        -lfacedetection

HEADERS += \
    FaceDetection/FaceDetection.h \
    InfoPrediction/InfoPrediction.h \
    RobotEyes.h \
    TargetMatcher.h \
    functions.h
