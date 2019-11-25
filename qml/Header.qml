import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root

    width: parent.width
    height: parent.height/10

//    color: "#505050" //Dark Theme
    color: "#D7D7D7" //Light Theme

    Button {
        id: _optionsButton

        width: parent.height
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top
        icon.source: "qrc:/icons/menu.png" //TODO: svg icon instead of png

        Material.background: "transparent"

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
        color: Material.color(Material.DeepOrange)
        anchors.centerIn: root
    }
    Button {
        id: _buttonSearch

        width: parent.height
        height: parent.height
        anchors.top: parent.top
        anchors.right: parent.right
        icon.source: "qrc:/icons/search.png"

        Material.background: "transparent"

        onClicked: {
            console.log("Search")
        }
    }
}
