import QtQuick 2.0

ListView {
    id: root

    model: AppModel

    objectName: "appView"

    delegate: Element {
        appLabel: display
        appLogo: "image://prov/" + model.index

        width: root.width
        height: root.height/4

        index: model.index
    }
}
