import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ListView {
    id: root

    model: AppModel

    objectName: "appView"

    signal signalRunApp(var i)
    signal signalGetAppInfo(var i)

    readonly property Popup popupInfo: _popUpAppInfo

    delegate: Element {
        id: _elem

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

    Popup { //TODO: Fill this widget
        id: _popUpAppInfo

        modal: true
        focus: true
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        Image { //TODO: Opacity Mask, Remove magic numbers
            id: _infoImg

            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10

            Text {
                id: _appName

                anchors.top: _popUpAppInfo.top
                anchors.left: parent.right
                anchors.leftMargin: 10
                font.bold: true
                color: textColor
            }

            Text {
                id: _appVersion

                anchors.top: _appName.bottom
                anchors.topMargin: 5
                anchors.left: parent.right
                anchors.leftMargin: 10
                color: textColor
                text: qsTr("Version: " + "14.95.13.21")
            }

            Text {
                id: _appCategory

                anchors.top: _appVersion.bottom
                anchors.topMargin: 5
                anchors.left: parent.right
                anchors.leftMargin: 10
                color: textColor
            }
        }

        Rectangle {
            id: _sepatator

            anchors.top: _infoImg.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            width: parent.width
            height: 3
            color: Material.accent
        }

        Text {
            id: _appPackageName

            anchors.top: _sepatator.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            color: textColor
        }

        Text {
            id: _appMinSdk

            anchors.top: _appPackageName.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            color: textColor
        }

        Text {
            id: _appTargetSdk

            anchors.top: _appMinSdk.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            color: textColor
        }

        Text { //TODO: Add "Path:" string before real path, fix problem with long string
            id: _appPath

            anchors.top: _appTargetSdk.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            color: textColor
        }

        Rectangle {
            id: _sepatator2

            anchors.top: _appPath.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            width: parent.width
            height: 3
            color: Material.accent
        }

        Text {
            id: _packSize

            anchors.top: _sepatator2.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            color: textColor
        }
    }
}
