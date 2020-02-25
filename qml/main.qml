import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import ModuleTranslation 1.0

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Applications Manager")

    property var themeStyle: Material.Dark
    readonly property Window mainWindow: root
//    readonly property Popup popupInfo: _popUpAppInfo
    readonly property Dialog dialogRemove: _dialogRemoveApp
    readonly property Dialog dialogDelete: _dialogDeleteApp
    readonly property Popup popupAdvOptions: _popUpAdvOptions

    readonly property var textColor: Material.color(Material.DeepOrange)
    readonly property var elementColor: themeStyle === Material.Dark ? "#424242" : "#FFFFFF"
    readonly property var headerColor: themeStyle === Material.Dark ? "#505050" : "#D7D7D7"

    Material.theme: themeStyle
    Material.foreground: Material.DeepOrange
    Material.accent: Material.DeepPurple

    objectName: "mainWindow"
    signal signalSortingOrder(int order)

    Header {
        id: _header

        anchors.left: parent.left
        anchors.top: parent.top
    }
    Rectangle {
        id: _restRect

        anchors.left: parent.left
        anchors.top: _header.bottom
        width: parent.width
        height: parent.height - _header.height
        z: -1
        AppView {
            id: _aView

            anchors.fill: parent
        }
    }

    Popup {
        id: _popUpFilters

        modal: true
        focus: true
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        Column {
            anchors.fill: parent

            Text {
                id: _sortOrderText

                text: qsTr("Sorting order:")
                font.bold: true
                color: textColor
            }
            RadioButton { //TODO: Search for better slot
                id: _ascRadioButton

                text: qsTr("Ascending order")
                onPressed: {
                    signalSortingOrder(0)
                }
            }
            RadioButton {
                id: _desRadioButton
                text: qsTr("Descending order")
                onPressed: {
                    signalSortingOrder(1)
                }
            }
        }
    }

    Dialog {
        id: _dialogRemoveApp

        title: qsTr("Remove Application")
        focus: true
        width: parent.width * 0.9
        height: parent.height * 0.33
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: console.log("Remove Dialog Ok")
        onRejected: console.log("Remove Dialog Cancel")

        Text {
            id: _upperRemoveText

            text: qsTr("Are you sure want to remove\nthis application from this list?") //TODO: Replace application with real app name
            color: textColor
            anchors.centerIn: parent
        }
    }

    Dialog {
        id: _dialogDeleteApp

        title: qsTr("Delete Application")
        focus: true
        width: parent.width * 0.9
        height: parent.height * 0.33
        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: console.log("Delete Dialog Ok")
        onRejected: console.log("Delete Dialog Cancel")

        Text { //TODO: accept, reject slots
            id: _upperDeleteText


            text: qsTr("Are you sure want to erase\nthis application from your device?") //TODO: Replace application with real app name
            color: textColor
            anchors.centerIn: parent
        }
    }

    Popup { //TODO: Fill this widget
        id: _popUpAdvOptions

        modal: true
        focus: true
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape

        Column { //TODO: Edit elements width
            anchors.fill: parent

            spacing: 5 // TODO: Test value

            /*Language*/
            Text {
                id: _lngText

                text: qsTr("Language")
                color: textColor
            }

            ButtonGroup {
                id: _languageGroup

                exclusive: true
                buttons: _languageColumn.children
            }

            Column {
                id: _languageColumn

                CheckBox {
                    id: _enCheckBox

                    text: "English"
                    checked: true
                }
                CheckBox {
                    id: _ruCheckBox

                    objectName: "langChange"

                    text: "Русский"
                    checked: false
                    onCheckedChanged:
                    {
                        //console.log("Checkbox state:", _ruCheckBox.checked)
                        console.log("isRussianLanguage state:", Translation.isRussianLanguage)
                        Translation.isRussianLanguage = !Translation.isRussianLanguage
                        console.log("isRussianLanguage state:", Translation.isRussianLanguage)
//                        _buttonAbout.text = Locale.testText
//                        if(_ruCheckBox.checked)
//                        {
//                            _buttonAbout.text = "О Приложении"
//                            _dialogAbout.title = "AppManager версия 0.1"
//                            _dialogAboutText.text = "Сделано с помощью Qt и JNI"
//                            _lngText.text = "Язык"
//                            _upperDeleteText.text = "Вы действительно хотите удалить\nэто приложение?"
//                            _dialogDeleteApp.title = "Удалить Приложение"
//                            _upperRemoveText.text = "Вы действительно хотите убрать\nэто приложение из списка?"
//                            _dialogRemoveApp.title = "Убрать приложение из списка"
//                            _sortOrderText.text = "Порядок сортировки:"
//                            _ascRadioButton.text = "По Возрастанию"
//                            _desRadioButton.text = "По Убыванию"
//                        } else {
//                            _buttonAbout.text = "About App"
//                            _dialogAbout.title = "AppManager ver. 0.1"
//                            _dialogAboutText.text = "Made with Qt and JNI"
//                            _lngText.text = "Language"
//                            _upperDeleteText.text = "Are you sure want to erase\nthis application from your device?"
//                            _dialogDeleteApp.title = "Delete Application"
//                            _upperRemoveText.text = "Are you sure want to remove\nthis application from this list?"
//                            _dialogRemoveApp.title = "Remove Application"
//                            _sortOrderText.text = "Sorting order:"
//                            _ascRadioButton.text = "Ascending order"
//                            _desRadioButton.text = "Descending order"
//                        }

                    }
                }
            }
            /*Language*/

            MenuSeparator {width: root.width - root.width*0.1}

            Button {
                id: _buttonAbout

                text: qsTr("About App")
                onClicked: _dialogAbout.open()

                Dialog { //TODO?: Separator between header and text
                    id: _dialogAbout

                    title: qsTr("AppManager ver. 0.1")
                    focus: true
                    width: root.width * 0.9
                    height: root.height * 0.33
                    standardButtons: Dialog.Ok

                    Text {
                        id: _dialogAboutText
                        text: qsTr("Made with Qt and JNI")
                        color: textColor
                        anchors.centerIn: parent
                    }
                }
            }

        }
    }
    Component.onCompleted: {
        console.log("Russian val state: ", Translation.isRussianLanguage)
//        console.log("Language: ")
//        console.log(_aView._path.text)
//        if(!Locale.isRussianLanguage) console.log("English")
//        else console.log("Russian")
    }
}
