import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    id: root
    width: parent.width
    height: parent.height/10

    Button {
        id: _optionsButton
        width: parent.height
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top
        text: "O" //TODO: Change text
        onClicked: drawer.open()

        Drawer { //TODO: Move this into separate file
            id: drawer
            width: mainWindow.width * 0.66
            height: mainWindow.height

            Button {
                id: _buttonFilters
                text: "Filters"
                width: parent.width
                height: parent.height/5
                anchors.left: root.left
                anchors.top: root.top
            }
            Button {
                id: _buttonAdvOptions
                text: "AdvancedOptions"
                width: parent.width
                height: parent.height/5
                anchors.left: root.left
                anchors.top: _buttonFilters.bottom
            }
            Switch{
                id: _fullList
                text: "Show All Apps"
                width: parent.width
                height: parent.height/5
                anchors.left: root.left
                anchors.top: _buttonAdvOptions.bottom
                checked: false
            }
            Switch{
                id: _styleSwitch
                text: "Change Style"
                width: parent.width
                height: parent.height/5
                anchors.left: root.left
                anchors.top: _fullList.bottom
                checked: false
            }
//            MenuSeparator{}
            Button{
                id: _buttonQuit
                text: "Quit"
                width: parent.width
                height: parent.height/5
                anchors.left: root.left
                anchors.top: _styleSwitch.bottom
            }
        }


    }
    Text {
        id: _appName
        text: qsTr("App Manager")
        color: "red"
        anchors.centerIn: root
    }
}
