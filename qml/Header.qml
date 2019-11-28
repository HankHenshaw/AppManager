import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root

    width: parent.width
    height: parent.height/10

    color: "#505050" //Dark Theme

    TextField {
        id: _searchFilter

        anchors.top: _optionsButton.top
        anchors.left: _optionsButton.right
        anchors.topMargin: 5 //TODO: Replace with relative value or anchors
        width: parent.width - _optionsButton.width - _buttonSearch.width
        height: parent.height
        visible: false
    }

    Button {
        id: _optionsButton

        width: parent.height
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 6 //TODO: Replace with relative value or anchors
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

                    onCheckedChanged: {
                        if(checked) {
                            themeStyle = Material.Light
                            root.color = "#D7D7D7"
                            elemColor = "#FFFFFF" //TODO
                        } else {
                            themeStyle = Material.Dark
                            root.color =  "#505050"
                            elemColor = "#424242" //TODO
                        }
                    }
                }
                //TODO: Add Separator
                Button{
                    id: _buttonQuit

                    text: "Quit"
                    width: parent.width
                    height: parent.height/5

                    onClicked: {
                        Qt.quit()
                    }
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
            if(_appName.visible) _appName.visible = false
            else _appName.visible = true

            if(_searchFilter.visible) _searchFilter.visible = false
            else _searchFilter.visible = true
            console.log("Search")
        }
    }
}
