import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

//TODO: Added Material Style

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    readonly property Window mainWindow: root
    property var themeStyle: Material.Dark

//    Material.theme: Material.Dark //TODO: Switch Between Light and Dark
    Material.theme: themeStyle
    Material.foreground: Material.DeepOrange
    Material.accent: Material.DeepPurple

    Header {
        id: _header

        anchors.left: parent.left
        anchors.top: parent.top
    }
    Rectangle {
        id: _restRect

        anchors.left: parent.left
        anchors.top: _header.bottom
        width: parent.width
        height: parent.height - _header.height
        z: -1
        AppView {
            anchors.fill: parent
        }
    }


}
