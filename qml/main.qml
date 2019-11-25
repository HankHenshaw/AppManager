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

    Material.theme: Material.Light
    Material.accent: Material.Orange

    AppView {
        Header {

        }
        anchors.fill: parent
    }
}
