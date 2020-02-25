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
    readonly property Text pathInfo: _path

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
        height: parent.height*2 // 100 - height of header
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        Flickable
        {
            id: _flick
            anchors.fill: parent
            contentWidth: _popUpAppInfo.width
            contentHeight: _popUpAppInfo.height
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds

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
                    text: qsTr("Version: ")//???
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
                width: parent.width - 25
                height: 3
                color: Material.accent
            }

            Text {
                id: _packageName

                anchors.top: _sepatator.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
                text: qsTr("Package Name:")
                font.bold: true
                font.underline: true
            }

            Text {
                id: _appPackageName

                anchors.top: _packageName.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                fontSizeMode: Text.VerticalFit
                color: textColor
            }

            Text {
                id: _path

                anchors.top: _appPackageName.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
                font.bold: true
                font.underline: true
                text: qsTr("Path:")
            }

            Text {
                id: _appPath

                anchors.top: _path.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                fontSizeMode: Text.HorizontalFit
                color: textColor
            }

            Text {
                id: _appMinSdk

                anchors.top: _appPath.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
            }

            Text {
                id: _appTargetSdk

                anchors.top: _appMinSdk.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
            }

            Rectangle {
                id: _sepatator2

                anchors.top: _appTargetSdk.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                width: parent.width - 25
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

            Text {
                id: _cacheSize

                anchors.top: _packSize.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
            }

            Rectangle {
                id: _sepatator3

                anchors.top: _cacheSize.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                width: parent.width - 25
                height: 3
                color: Material.accent
            }

            Text {
                id: _firstInstallationTime

                anchors.top: _sepatator3.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
            }

            Text {
                id: _lastUpdatedTime

                anchors.top: _firstInstallationTime.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
            }

            Rectangle {
                id: _sepatator4

                anchors.top: _lastUpdatedTime.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                width: parent.width - 25
                height: 3
                color: Material.accent
            }
            Text {
                id: _premissions

                anchors.top: _sepatator4.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                text: qsTr("Permissions:")
                font.underline: true
                font.bold: true
                color: textColor
            }

            Text {
                id: _requestedPremissions

                anchors.top: _premissions.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                color: textColor
            }
        }
    }
}
