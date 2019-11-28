import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Applications Manager")

    property var themeStyle: Material.Dark
    readonly property Window mainWindow: root
    readonly property Popup popupInfo: _popUpAppInfo
    readonly property Dialog dialogRemove: _dialogRemoveApp
    readonly property Dialog dialogDelete: _dialogDeleteApp
    readonly property Popup popupAdvOptions: _popUpAdvOptions

    readonly property var textColor: Material.color(Material.DeepOrange) //TODO: Replace all colors with this, ?place all theme and color in separate file?
    readonly property var elementColor: themeStyle === Material.Dark ? "#424242" : "#FFFFFF"
    readonly property var headerColor: themeStyle === Material.Dark ? "#505050" : "#D7D7D7"

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

    Popup { //TODO: Fill this widget
        id: _popUpAppInfo

        modal: true
        focus: true
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        contentItem: Text {
            text: "Hello"
            color: textColor
            anchors.centerIn: parent
        }
    }

    Dialog {
        id: _dialogRemoveApp

        focus: true
        width: parent.width * 0.9
        height: parent.height * 0.33
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        Text {
            id: _upperRemoveText

            text: qsTr("Are you sure want to remove\nthis application from this list?") //TODO: Replace application with real app name
            color: textColor
            anchors.centerIn: parent
        }
    }

    Dialog {
        id: _dialogDeleteApp

        focus: true
        width: parent.width * 0.9
        height: parent.height * 0.33
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        Text { //TODO: accept, reject slots
            id: _upperDeleteText

            text: qsTr("Are you sure want to erase\nthis application from your device?") //TODO: Replace application with real app name
            color: textColor
            anchors.centerIn: parent
        }
    }

    Popup { //TODO: Fill this widget
        id: _popUpAdvOptions

        modal: true
        focus: true
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        contentItem: Text {
            text: "Hello"
            color: textColor
            anchors.centerIn: parent
        }
    }
}
