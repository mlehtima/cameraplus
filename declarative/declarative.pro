TEMPLATE = lib
TARGET = declarativeqtcamera
DEPENDPATH += . ../lib/
INCLUDEPATH += . ../lib/
include(../cameraplus.pri)

CONFIG += link_pkgconfig plugin

PKGCONFIG = sndfile libpulse

sailfish:PKGCONFIG += gstreamer-1.0
harmattan:PKGCONFIG += gstreamer-0.10

LIBS += -L../lib/ -lqtcamera

QT += dbus

qt4:QT += declarative
qt5:QT += qml quick

HEADERS += plugin.h previewprovider.h camera.h mode.h imagemode.h videomode.h \
           zoom.h flash.h scene.h evcomp.h videotorch.h whitebalance.h \
           colortone.h exposure.h aperture.h iso.h noisereduction.h \
           flickerreduction.h videomute.h metadata.h imagesettings.h \
           resolutionmodel.h videosettings.h \
           notificationscontainer.h sounds.h focus.h autofocus.h \
           roi.h cameraconfig.h videoplayer.h viewfinder.h capability.h \
           resolution.h viewfinderbufferhandler.h viewfinderframehandler.h \
           viewfinderhandler.h

SOURCES += plugin.cpp previewprovider.cpp camera.cpp mode.cpp imagemode.cpp videomode.cpp \
           zoom.cpp flash.cpp scene.cpp evcomp.cpp videotorch.cpp whitebalance.cpp \
           colortone.cpp exposure.cpp aperture.cpp iso.cpp noisereduction.cpp \
           flickerreduction.cpp videomute.cpp metadata.cpp imagesettings.cpp \
           resolutionmodel.cpp videosettings.cpp \
           notificationscontainer.cpp sounds.cpp focus.cpp autofocus.cpp \
           roi.cpp cameraconfig.cpp videoplayer.cpp viewfinder.cpp capability.cpp \
           resolution.cpp viewfinderbufferhandler.cpp viewfinderframehandler.cpp \
           viewfinderhandler.cpp

PLUGIN_IMPORT_PATH = QtCamera
target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH

qml.files = qmldir
qml.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
INSTALLS += target qml
