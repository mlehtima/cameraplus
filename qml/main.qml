// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import CameraPlus 1.0

PageStackWindow {
        id: root

        property alias dimmer: camDimmer

        showStatusBar: false

        Component.onCompleted: {
                theme.inverted = true;
                if (settings.mode == 0) {
                        openFile("ImagePage.qml");
                }
                else {
                        openFile("VideoPage.qml");
                }
        }

        Settings {
                id: settings
        }

        // Stolen from https://qt.gitorious.org/qt-components/qt-components/blobs/master/examples/meego/QmlComponentGallery/qml/ListPage.qml
        function replacePage(file) {
                var component = Qt.createComponent(file)

                if (component.status == Component.Ready) {
                        pageStack.replace(component, {cam: cam}, true);
                }
                else {
                        console.log("Error loading component:", component.errorString());
                }
        }

        function openFile(file) {
                var component = Qt.createComponent(file)

                if (component.status == Component.Ready) {
                        pageStack.push(component, {cam: cam});
                }
                else {
                        console.log("Error loading component:", component.errorString());
                }
        }

        platformStyle: PageStackWindowStyle {
                // TODO: Hack
                background: " "
        }

        Camera {
                onDeviceIdChanged: cam.start();

                id: cam
                anchors.fill: parent

                // TODO: hardcoding
                Component.onCompleted: { cam.deviceId = 0; mode = settings.mode; }

                // TODO: Hack
                z: -1

                Rectangle {
                        id: camDimmer
                        z: 1
                        anchors.fill: parent
                        opacity: 0
                        color: "black"
                }
        }

        ModeController {
                id: cameraMode
                cam: cam
                dimmer: root.dimmer
        }

        Connections {
                target: cam
                onModeChanged: {
                        if (cam.mode == Camera.VideoMode) {
                                replacePage("VideoPage.qml");
                        }
                        else {
                                replacePage("ImagePage.qml");
                        }
                }
        }
}