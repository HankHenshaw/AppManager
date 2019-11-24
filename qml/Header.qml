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
        icon.source: "qrc:/icons/menu.png" //TODO: svg icon instead of png

        onClicked: drawer.open()

        Drawer { //TODO: Move this into separate file
            id: drawer

            width: mainWindow.width * 0.66
            height: mainWindow.height

            Column {
                anchors.fill: parent
                Button {
                    id: _buttonFilters

                    text: "Filters"
                    width: parent.width
                    height: parent.height/5
                }
                Button {
                    id: _buttonAdvOptions

                    text: "AdvancedOptions"
                    width: parent.width
                    height: parent.height/5
                }
                Switch{
                    id: _fullList

                    text: "Show All Apps"
                    width: parent.width
                    height: parent.height/5
                    checked: false
                }
                Switch{
                    id: _styleSwitch

                    text: "Change Style"
                    width: parent.width
                    height: parent.height/5
                    checked: false
                }
                //TODO: Add Separator
                Button{
                    id: _buttonQuit

                    text: "Quit"
                    width: parent.width
                    height: parent.height/5
                }
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
