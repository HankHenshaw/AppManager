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

    property Window mainWindow: root //readonly?
    property var themeStyle: Material.Dark
    readonly property Popup popupInfo: _popUpAppInfo
    readonly property Dialog dialogRemove: _dialogRemoveApp
    readonly property Dialog dialogDelete: _dialogDeleteApp

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

    Popup {
        id: _popUpAppInfo

        modal: true
        focus: true
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        contentItem: Text {
            text: "Hello"
            anchors.centerIn: parent
        }
    }

    Dialog {
        id: _dialogRemoveApp

        focus: true
        width: 300 //TODO: Replace with relative value
        height: 125 //TODO: Replace with relative value
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        Text {
            id: _upperRemoveText

            text: qsTr("Are you sure want to remove\nthis application from this list?") //TODO: Replace application with real app name
            anchors.centerIn: parent
        }
    }

    Dialog {
        id: _dialogDeleteApp

        focus: true
        width: 300 //TODO: Replace with relative value
        height: 125 //TODO: Replace with relative value
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        Text { //TODO: Text color, accept, reject slots
            id: _upperDeleteText

            text: qsTr("Are you sure want to erase\nthis application from your device?") //TODO: Replace application with real app name
            anchors.centerIn: parent
        }
    }
}
