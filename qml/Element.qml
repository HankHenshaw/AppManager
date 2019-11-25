import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Rectangle {
    id: root

    objectName: "delegateItem"

    property string appLabel: ""
    property string appLogo: ""
    property var index: -1


//    border.color: "black"
//    border.width: 1 //TODO: Replace border with horizontal line?

    //TODO: Place this in horizontalLayout (Row)
    Text {
        id: _appLabel

        anchors.centerIn: parent
        text: appLabel
    }

    Image {
        id: _appLogo

        width: parent.height * 0.7
        height: parent.height * 0.7
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: parent.width * 0.05
        source: appLogo
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: _mask
        }
    }
    Rectangle {
        id: _mask

        width: _appLogo.width
        height: _appLogo.height
        radius: width * 0.5
        visible: false
    }

    Button { //TODO: Слот элемента модели вызывается раньше слота кнопки
        id: _contextMenu

        height: parent.height - 2
        width: parent.width/10
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/icons/button-of-three-vertical-squares.png"

        onClicked: {
            console.log("Context Menu")
        }
    }
}
