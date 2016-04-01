import QtQuick 2.0
import QtQml.Models 2.1

ListView {
    id: root

    displaced: Transition {
        NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
    }

    Behavior on contentY {
        NumberAnimation { duration: 200 }
    }

    FontLoader { id: localFont; source: "qrc:///fonts/DroidSansFallback.ttf" }

    model: DelegateModel {
        id: visualModel
        model: medoRecordManager
        delegate: Item {
            id: rootItem
            width: gScreenInfo.platformWidth
            height: 100
            property int visualIndex: DelegateModel.itemsIndex

            MouseArea {
                id: delegateRoot
                anchors.fill: parent

                //                drag.target: cellItem
                drag.axis: Drag.YAxis

                onPressAndHold: {
                    delegateRoot.drag.target = cellItem
                }

                onClicked: {
                    medoAttManager.updateAttModel(model.modelData.attachment)
                    rootPage.showEditPage("OPEN_EDIT", model.modelData.content,
                                          model.modelData.date, model.modelData.id,
                                          model.modelData.attachment,model.modelData.attachmentList)
                }

                onReleased: {
                    delegateRoot.drag.target = undefined
                }

                onCanceled: {
                    delegateRoot.drag.target = undefined
                }

                Rectangle {
                    id: cellItem
                    anchors.fill: parent

                    color: "#EEE4DA"
//                    radius: 30
//                    opacity: 0.5

                    Drag.active: delegateRoot.drag.active
                    Drag.source: rootItem
                    Drag.hotSpot.x: cellItem.width/2
                    Drag.hotSpot.y: cellItem.height/2

                    Drag.onActiveChanged: {
                        if (Drag.active) {
                            cellItem.anchors.fill = undefined
                            cellItem.parent = root
                        } else {
                            cellItem.parent = delegateRoot
                            cellItem.anchors.fill = delegateRoot
                        }
                    }

                    Text {
                        id: contentText
                        anchors.left: parent.left
                        anchors.leftMargin: 40
                        anchors.top: parent.top
                        anchors.topMargin: 36
                        anchors.right: timeText.left
                        anchors.rightMargin: 40
                        font.pixelSize: 24
                        height: 28
                        clip: true
                        color: "#777777"
//                        color: "#F9F6F2"
                        elide: Text.ElideRight
                        font.family: localFont.name
                        font.bold: true
                        text: model.modelData.content
                    }

                    Text {
                        id: timeText
                        anchors.right: parent.right
                        anchors.rightMargin: 40
                        anchors.top: contentText.top
                        height: contentText.height
                        verticalAlignment: Text.AlignVCenter

                        font.pixelSize: 24
                        color: "#777777"

                        text: model.modelData.date
                    }

                    Rectangle {
                        id: bottomLine
                        color: "#1291A9"
                        height: 1
                        width: parent.width
                        anchors.left: parent.left
                        anchors.leftMargin: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 40
                        anchors.bottom: parent.bottom
                        z: parent.z+2
                    }

//                    Text {
//                        id: timeText
//                        anchors.right: parent.right
//                        anchors.rightMargin: sessionHistoryListView.editing ? 100 : 40
//                        anchors.top: nameText.top
//                        height: nameText.height
//                        verticalAlignment: Text.AlignVCenter

//                        font.pixelSize: 24
//                        color: "#777777"

//                        text: Qt.formatDateTime(imSessionListPage.listwallClockCurrentTime, "yyyy") !== Qt.formatDateTime(model.modelData.time, "yyyy") ?
//                                  Qt.formatDateTime(model.modelData.time, "yyyy-MM-dd hh:mm") :
//                                  Qt.formatDateTime(imSessionListPage.listwallClockCurrentTime, "MM-dd") !== Qt.formatDateTime(model.modelData.time, "MM-dd") ?
//                                      Qt.formatDateTime(model.modelData.time, "MM-dd hh:mm") : Qt.formatDateTime(model.modelData.time, "hh:mm")
//                    }

//                    onYChanged: {
//                        console.log("cellItem.y = ", cellItem.y)
//                        console.log("xxx = ", root.height - (root.visibleArea.yPosition * root.height + cellItem.height))
//                        if (cellItem.y > root.height - (root.visibleArea.yPosition * root.height + cellItem.height)) {
//                            if (!viewMoveTimer.running) {
//                                viewMoveTimer.restart()
//                            }
//                        }
//                    }
                }

                Timer {
                    id: viewMoveTimer
                    interval: 500; running: false; repeat: false
                    onTriggered: {
                        root.contentY += cellItem.height
                    }
                }

                DropArea {
                    id: dropArea
                    anchors { fill: parent; margins: 15 }

                    onEntered: visualModel.items.move(drag.source.visualIndex, rootItem.visualIndex)
                }
            }

            Component.onCompleted: {
                medoRecordManager.startQueryAttachments(model.modelData.id);
            }
        }
    }


    ListModel {
        id: colorModel
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "red" }
        ListElement { color: "yellow" }
        ListElement { color: "orange" }
        ListElement { color: "purple" }
        ListElement { color: "cyan" }
        ListElement { color: "magenta" }
        ListElement { color: "chartreuse" }
        ListElement { color: "aquamarine" }
        ListElement { color: "indigo" }
        ListElement { color: "black" }
        ListElement { color: "lightsteelblue" }
        ListElement { color: "violet" }
        ListElement { color: "grey" }
        ListElement { color: "springgreen" }
        ListElement { color: "salmon" }
        ListElement { color: "blanchedalmond" }
        ListElement { color: "forestgreen" }
        ListElement { color: "pink" }
        ListElement { color: "navy" }
        ListElement { color: "goldenrod" }
        ListElement { color: "crimson" }
        ListElement { color: "tan" }
        ListElement { color: "teal" }
        ListElement { color: "plum" }
        ListElement { color: "seagreen" }
        ListElement { color: "brown" }
    }
}
