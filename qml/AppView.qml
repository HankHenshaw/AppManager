import QtQuick 2.0

ListView {
    id: root

    model: AppModel

    objectName: "appView"

    signal signalRunApp(var i)
    signal signalGetAppInfo(var i)

    delegate: Element {
        appLabel: display
        appLogo: "image://prov/" + modelIndex

        width: root.width
        height: root.height/6

        modelIndex: AppModel.sourceRow(model.index)

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
