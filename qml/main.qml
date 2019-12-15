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
//    readonly property Popup popupInfo: _popUpAppInfo
    readonly property Dialog dialogRemove: _dialogRemoveApp
    readonly property Dialog dialogDelete: _dialogDeleteApp
    readonly property Popup popupAdvOptions: _popUpAdvOptions

    readonly property var textColor: Material.color(Material.DeepOrange)
    readonly property var elementColor: themeStyle === Material.Dark ? "#424242" : "#FFFFFF"
    readonly property var headerColor: themeStyle === Material.Dark ? "#505050" : "#D7D7D7"

    Material.theme: themeStyle
    Material.foreground: Material.DeepOrange
    Material.accent: Material.DeepPurple

    objectName: "mainWindow"
    signal signalSortingOrder(int order)

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
        id: _popUpFilters

        modal: true
        focus: true
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        Column {
            anchors.fill: parent

            Text {
                text: qsTr("Sorting order:")
                font.bold: true
                color: textColor
            }
            RadioButton { //TODO: Search for better slot
                text: qsTr("Ascending order")
                onPressed: {
                    signalSortingOrder(0)
                }
            }
            RadioButton {
                text: qsTr("Descending order")
                onPressed: {
                    signalSortingOrder(1)
                }
            }
        }
    }

    Dialog {
        id: _dialogRemoveApp

        title: qsTr("Remove Application")
        focus: true
        width: parent.width * 0.9
        height: parent.height * 0.33
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: console.log("Remove Dialog Ok")
        onRejected: console.log("Remove Dialog Cancel")

        Text {
            id: _upperRemoveText

            text: qsTr("Are you sure want to remove\nthis application from this list?") //TODO: Replace application with real app name
            color: textColor
            anchors.centerIn: parent
        }
    }

    Dialog {
        id: _dialogDeleteApp

        title: qsTr("Delete Application")
        focus: true
        width: parent.width * 0.9
        height: parent.height * 0.33
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: console.log("Delete Dialog Ok")
        onRejected: console.log("Delete Dialog Cancel")

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

        Column { //TODO: Edit elements width
            anchors.fill: parent

            spacing: 5 // TODO: Test value

            /*Language*/
            Text {
                text: qsTr("Language")
                color: textColor
            }

            ButtonGroup {
                id: _languageGroup

                exclusive: true
                buttons: _languageColumn.children
            }

            Column {
                id: _languageColumn

                CheckBox {
                    text: "English"
                    checked: true
                }
                CheckBox {
                    text: "Русский"
                    checked: false
                }
            }
            /*Language*/

            MenuSeparator {width: root.width - root.width*0.1}

            Button {
                id: _buttonAbout

                text: qsTr("About App")
                onClicked: _dialogAbout.open()

                Dialog { //TODO?: Separator between header and text
                    id: _dialogAbout

                    title: qsTr("About")
                    focus: true
                    width: root.width * 0.9
                    height: root.height * 0.33
                    standardButtons: Dialog.Ok

                    Text {
                        text: qsTr("Some Text")
                        color: textColor
                        anchors.centerIn: parent
                    }
                }
            }

        }
    }
}
