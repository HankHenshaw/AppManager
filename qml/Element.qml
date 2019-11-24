import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    id: root
    color: "gray" //TODO

    objectName: "delegateItem"

    property string appLabel: ""
    property string appLogo: ""
    property var index: -1


    border.color: "black"
    border.width: 1

    //TODO: Place this in horizontalLayout (Row)
    Text {
        id: _appLabel

        anchors.centerIn: parent
        text: appLabel
    }

    Image {
        id: _appLogo

        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 10
        source: appLogo
    }
    Button {
        id: _contextMenu

        height: parent.height - 2
        width: parent.width/10
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/icons/button-of-three-vertical-squares.png"
    }
}
