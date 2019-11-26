import QtQuick 2.0

ListView {
    id: root

    model: AppModel

    objectName: "appView"

    signal signalRunApp(var i)

    delegate: Element {
        appLabel: display
        appLogo: "image://prov/" + model.index

        width: root.width
        height: root.height/6

        index: model.index

        MouseArea {
            id: _ma

            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width - parent.width * 0.1
            height: parent.height
            onClicked: {
                console.log("Test")
                root.signalRunApp(index)
            }
        }
    }
}
