import QtQuick 2.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import "."

Window {
    id: rootWindow
    objectName: "LauncherWindow"
    visible: true
    width: Style.windowWidth
    height: Style.windowHeight

    FontLoader {
        id: mainFont
        source: "qrc:/fonts/fonts/OpenSans-Regular.ttf"
    }

    property Component activityMusic: ActivityMusic {}
    property Component activitySettings: ActivitySettings {}

    property var activityMap: {
        1: activityMusic,
        8: activitySettings
    }

    Rectangle {
        anchors.fill: parent
        anchors.left: parent.left
        width: rootWindow.width
        height: rootWindow.height

        gradient: Gradient {
            GradientStop {
                color: Style.backgroundColorEnd
                position: 0
            }
            GradientStop {
                color: Style.backgroundColorStart
                position: .5
            }
            GradientStop {
                color: Style.backgroundColorEnd
                position: 1
            }
        }

        StackView {
            id: stackView
            width: rootWindow.width
            height: rootWindow.height
            anchors.fill: parent
            anchors.left: parent.left

            initialItem: GridView {
                width: rootWindow.width - Style.mainMenu.marginH
                height: rootWindow.height - Style.mainMenu.marginV
                cellWidth: Style.mainMenu.buttonWidth + Style.mainMenu.marginH
                cellHeight: Style.mainMenu.buttonHeight + Style.mainMenu.marginV

                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: Style.mainMenu.marginH
                anchors.topMargin: Style.mainMenu.marginV

                model: ListModel {
                    ListElement {
                        index: 1
                        isEnabled: true
                        title: qsTr("Ma Musique")
                        icon: "qrc:/images/music"
                    }
                    ListElement {
                        index: 2
                        isEnabled: false
                        title: qsTr("Radio")
                        icon: "qrc:/images/radio"
                    }
                    ListElement {
                        index: 3
                        isEnabled: false
                        title: qsTr("Mes vidéos")
                        icon: "qrc:/images/video"
                    }
                    ListElement {
                        index: 4
                        isEnabled: false
                        title: qsTr("Informations")
                        icon: "qrc:/images/car"
                    }
                    ListElement {
                        index: 5
                        isEnabled: false
                        title: qsTr("GPS")
                        icon: "qrc:/images/map"
                    }
                    ListElement {
                        index: 6
                        isEnabled: false
                        title: qsTr("Appeler")
                        icon: "qrc:/images/phone"
                    }
                    ListElement {
                        index: 7
                        isEnabled: false
                        title: qsTr("Transférer")
                        icon: "qrc:/images/upload"
                    }
                    ListElement {
                        index: 8
                        isEnabled: true
                        title: qsTr("Paramètres")
                        icon: "qrc:/images/settings"
                    }
                }

                delegate: MainMenuButton {
                    width: Style.mainMenu.buttonWidth
                    height: Style.mainMenu.buttonHeight

                    enabled: isEnabled
                    text: title
                    iconSource: icon

                    onClicked: {
                        if(isEnabled && stackView.depth == 1) {
                            stackView.push({item: activityMap[index]});
                            stackView.currentItem.forceActiveFocus();
                        }
                    }
                }
            }
        }
    }
}


