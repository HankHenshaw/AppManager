import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12
import ModuleTranslation 1.0

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
            _appPackageName.text = AppModel.sourceModel.getPackageName(modelIndex)
            _appPath.text = AppModel.sourceModel.getPackagePath(modelIndex)
            _appMinSdk.text = Translation.txtMinSdk + AppModel.sourceModel.getMinSdk(modelIndex)
            _appTargetSdk.text = Translation.txtTargetSdk + AppModel.sourceModel.getTargetSdk(modelIndex)
            _appVersion.text = Translation.txtVersion + AppModel.sourceModel.getVersion(modelIndex)
            _appCategory.text = Translation.txtCategory + AppModel.sourceModel.getCategory(modelIndex)
            _packSize.text = Translation.txtPckgSize + AppModel.sourceModel.getPackageSize(modelIndex) + Translation.txtPckgSize2
            _cacheSize.text = Translation.txtCacheSize + AppModel.sourceModel.getCacheSize(modelIndex) + Translation.txtCacheSize2
            _requestedPremissions.text = AppModel.sourceModel.getRequestedPermissions(modelIndex)
            _flick.contentHeight = _popUpAppInfo.height + 16 * AppModel.sourceModel.getPermissionsNumber()
            _firstInstallationTime.text = Translation.txtFirstInstlTime + AppModel.sourceModel.getFirstInstallationTime(modelIndex)
            if(AppModel.sourceModel.getFirstInstallationTime(modelIndex) === AppModel.sourceModel.getLastUpdateTime(modelIndex))
                _lastUpdatedTime.text = Translation.txtUpdateTime1
            else
                _lastUpdatedTime.text = Translation.txtUpdateTime2 + AppModel.sourceModel.getLastUpdateTime(modelIndex)
            _contextMenu.popup()
        }

        Menu {
            id: _contextMenu

            Action {
                id: _actionRunApp

                text: Translation.txtRunApp
                onTriggered: {
                    signalRunApp(modelIndex)
                }
            }
            Action {
                id: _actionRemoveApp

                text: Translation.txtRemoveApp
                onTriggered: {
                    dialogRemove.open()
                }
            }
            Action {
                id: _actionAppInfo

                text: Translation.txtInfoApp
                onTriggered: {
                    signalGetAppInfo(modelIndex)
                    popupInfo.open()
                }
            }

            MenuSeparator{}
            Action {
                id: _actionDeleteApp

                text: Translation.txtDeleteApp
                onTriggered: {
                    //dialogDelete.open()
                    //AppModel.sourceModel.deleteApp(modelIndex)
                    AppModel.sourceModel.clearCache(modelIndex) //TODO: Remove
                }
            }
        }
    }
}
