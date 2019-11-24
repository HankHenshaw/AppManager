import QtQuick 2.0

Rectangle {
    id: root
    color: "gray" //TODO

    objectName: "delegateItem"

    property string appLabel: ""
    property string appLogo: ""
    property var index: -1


    border.color: "black"
    border.width: 1

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
}
