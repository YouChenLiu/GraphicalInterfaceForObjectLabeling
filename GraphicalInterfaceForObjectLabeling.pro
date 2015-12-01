#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T23:08:05
#
#-------------------------------------------------

QT       += core gui xml widgets

TARGET = GraphicalInterfaceForObjectLabeling
TEMPLATE = app

SOURCES +=  main.cpp \
            MainWindow.cpp \
            LinkedListNode/LinkedListNode.cpp \
            Region/Region.cpp \
            VideoProcessor/VideoProcessor.cpp \
            FrameContainer/FrameContainer.cpp \
            RectangularRegion/RectangularRegion.cpp \
            EllipticalRegion/EllipticalRegion.cpp \
            CircularRegion/CircularRegion.cpp \
            RegionHeader/RegionHeader.cpp \
            VideoViewer/VideoViewer.cpp \
            sequencedialog.cpp \
            ImageSequence/ImageSequence.cpp

HEADERS +=  MainWindow.h \
            LinkedListNode/LinkedListNode.h \
            Region/Region.h \
            common.h \
            VideoProcessor/VideoProcessor.h \
            FrameContainer/FrameContainer.h \
            RectangularRegion/RectangularRegion.h \
            EllipticalRegion/EllipticalRegion.h \
            CircularRegion/CircularRegion.h \
            RegionHeader/RegionHeader.h \
            Shapes/Shapes.h \
            VideoViewer/VideoViewer.h \
            sequencedialog.h \
            ImageSequence/ImageSequence.h

FORMS    += MainWindow.ui \
    sequencedialog.ui

CONFIG -= debug_and_release

gcc: {
    CONFIG += c++11
    INCLUDEPATH += C:/opencv-mingw/include

    LIBS += -LC:/opencv-mingw/lib \
                -lopencv_core2411 \
                -lopencv_highgui2411 \
                -llibopencv_imgproc2411
}

msvc: {
    INCLUDEPATH += C:/opencv/build/include

    LIBS += -LC:/opencv/build/x64/vc12/lib

    CONFIG(debug, release|debug) {
        LIBS += -lopencv_core2411d \
                -lopencv_highgui2411d \
                -lopencv_imgproc2411d
    } else {
        LIBS += -lopencv_core2411 \
                -lopencv_highgui2411 \
                -lopencv_imgproc2411
    }
}
