import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root

    objectName: "delegateItem"

    property string appLabel: ""
    property string appLogo: ""
    property var modelIndex: -1
    property Rectangle elemRect: root

    color: elementColor

    //TODO: Place this in horizontalLayout (Row)
    Text {
        id: _appLabel

        anchors.centerIn: parent
        text: appLabel
        color: textColor
    }

    Image {
        id: _appLogo

        width: parent.height * 0.7
        height: parent.height * 0.7
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: parent.width * 0.02
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

    Button {
        id: _buttonContextMenu

        height: parent.height
        width: parent.width*0.1
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/icons/button-of-three-vertical-squares.png"
        Material.background: "transparent"

        onClicked: {
            _infoImg.source = "image://prov/" + modelIndex
            _appName.text = appLabel
            _appPackageName.text = "Package: " + AppModel.sourceModel.getPackageName(modelIndex)
            _appPath.text = AppModel.sourceModel.getPackagePath(modelIndex)
            _appMinSdk.text = "Minimum Sdk Version: " + AppModel.sourceModel.getMinSdk(modelIndex)
            _appTargetSdk.text = "Target Sdk Version: " + AppModel.sourceModel.getTargetSdk(modelIndex)
            _appVersion.text = "Version: " + AppModel.sourceModel.getVersion(modelIndex)
            _appCategory.text = "Category: " + AppModel.sourceModel.getCategory(modelIndex)
            _packSize.text = "Package Size: ~" + AppModel.sourceModel.getPackageSize(modelIndex) + " MB"
            _contextMenu.popup()
            console.log("Context Menu")
        }

        Menu {
            id: _contextMenu

            Action {
                id: _actionRunApp

                text: "Run App"
                onTriggered: {
                    signalRunApp(modelIndex)
                }
            }
            Action {
                id: _actionRemoveApp

                text: "Remove App"
                onTriggered: {
                    dialogRemove.open()
                }
            }
            Action {
                id: _actionAppInfo

                text: "App Info"
                onTriggered: {
                    signalGetAppInfo(modelIndex)
                    popupInfo.open()
                }
            }

            MenuSeparator{}
            Action {
                id: _actionDeleteApp

                text: "Delete App"
                onTriggered: {
                    dialogDelete.open()
                }
            }
        }
    }
}
