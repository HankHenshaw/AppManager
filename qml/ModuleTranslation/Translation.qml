pragma Singleton
import QtQuick 2.0

QtObject{
    property bool isRussianLanguage: false

    property var txtMinSdk: isRussianLanguage ? "Минимальная Версия СДК: " : "Minimum Sdk Version: "
    property var txtTargetSdk: isRussianLanguage ? "Нацеленая Версия СДК: " : "Target Sdk Version: "
    property var txtVersion: isRussianLanguage ? "Версия: " : "Version: "
    property var txtCategory: isRussianLanguage ? "Категория: " : "Category: "
    property var txtPckgSize: isRussianLanguage ? "Размер Пакета: ~" : "Package Size: ~"
    property var txtPckgSize2: isRussianLanguage ? " МБ" : " MB"
    property var txtCacheSize: isRussianLanguage ? "Размер Кэша: ~" : "Cache Size: ~"
    property var txtCacheSize2: isRussianLanguage ? " КБ" : " KB"
    property var txtFirstInstlTime: isRussianLanguage ? "Дата Установки: " : "First Installation Time: "
    property var txtUpdateTime1: isRussianLanguage ? "Дата Последнего Обновления: Ниразу" : "Last Updated Time: Never been updated"
    property var txtUpdateTime2: isRussianLanguage ? "Дата Последнего Обновления: " : "Last Updated Time: "

    property var txtRunApp: isRussianLanguage ? "Запустить" : "Run App"
    property var txtRemoveApp: isRussianLanguage ? "Убрать" : "Remove App"
    property var txtInfoApp: isRussianLanguage ? "Информиция" : "App Info"
    property var txtDeleteApp: isRussianLanguage ? "Удалить" : "Delete App"

    property var txtAboutTest: "About App"
    property var txtAbout: isRussianLanguage ? "О Приложении" : "About App"
    property var txtAboutTitle: isRussianLanguage ? "AppManager версия 0.1" : "AppManager ver. 0.1"
    property var txtAboutText: isRussianLanguage ? "Сделано с помощью Qt и JNI" : "Made with Qt and JNI"
    property var txtLng: isRussianLanguage ? "Язык" : "Language"
    property var txtDeleteTxt: isRussianLanguage ? "Вы действительно хотите удалить\nэто приложение?" : "Are you sure want to erase\nthis application from your device?"
    property var txtDeleteTitle: isRussianLanguage ? "Удалить Приложение" : "Delete Application"
    property var txtRemoveTxt: isRussianLanguage ? "Вы действительно хотите убрать\nэто приложение из списка?" : "Are you sure want to remove\nthis application from this list?"
    property var txtRemoveTitle: isRussianLanguage ? "Убрать приложение из списка" : "Remove Application"
    property var txtSortOrder: isRussianLanguage ? "Порядок сортировки:" : "Sorting order:"
    property var txtSortAsc: isRussianLanguage ? "По Возрастанию" : "Ascending order"
    property var txtSortDesc: isRussianLanguage ? "По Убыванию" : "Descending order"

    property var txtFilters: isRussianLanguage ? "Фильтры" : "Filters"
    property var txtOptions: isRussianLanguage ? "Опции" : "Options"
    property var txtShowSwitch: isRussianLanguage ? "Все Приложения" : "Show All Apps" // ТОДО
    property var txtStyle: isRussianLanguage ? "Изменить Стиль" : "Change Style"
    property var txtQuit: isRussianLanguage ? "Выход" : "Quit"
}
