import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        id: rootPage
        width:parent.width
        height:parent.height
        FontLoader { id: localFont; source: "qrc:///fonts/DroidSansFallback.ttf" }

        onActiveChanged: {
            //            if (active) {
            //                popMenu.show()
            //            }
        }

        //        onStatusChanged: {
        //            if (status === CPageStatus.WillShow) {
        //                popMenu.show()
        //            }
        //        }

        function showEditPage(openType, text, lastEditTime, recordId, attPath) {
            var component = pageStack.getCachedPage(Qt.resolvedUrl("CMedoEditPage.qml"),"CMedoEditPage");

            component.openType = openType
            component.textContent = text
            component.lastEditTime = lastEditTime
            component.recordId = recordId
            component.attPath = attPath

            console.log("showEditPage :: openType = ", component.openType)
            console.log("showEditPage :: textContent = ", component.textContent)
            console.log("showEditPage :: lastEditTime = ", component.lastEditTime)
            console.log("showEditPage :: recordId = ", component.recordId)
            console.log("showEditPage :: attPath = ", component.attPath)

            pageStack.push(component);
        }

        contentAreaItem: Item{

            CTitleBar {
                id: titleBar

                anchors.top: parent.top
                implicitWidth: parent.width
                implicitHeight: 94
                z:10
                leftItemEnabled: false
                titleAreaCentered: true

//                Rectangle {
//                    id: titleBarBackground
//                    anchors.fill: parent
//                    color: "#8F7A66"
//                    z:parent.z-1
//                }

                titleItemComponent: Text {
                    text: "Medo"
                    color: "#72cfD7"
                    font.bold: true
                    font.pixelSize: 36
                    font.family: localFont.name
                    MouseArea {
                        anchors.fill: parent

                        onDoubleClicked:  {
                            dropListView.positionViewAtBeginning()
                        }
                    }
                }
            }

            Rectangle {
                id: background
                anchors.top: titleBar.bottom
                anchors.bottom: dropListView.bottom
                width: parent.width
                color: "#EEE4DA"
                z:parent.z-1
            }

            CMedoDropListView {
                id: dropListView
                anchors.top: titleBar.bottom
                anchors.bottom: newBtnRoot.top
                width: rootPage.width
                height: dropListView.contentHeight
                clip: true
                flickDeceleration: 3000
            }

            Rectangle {
                id: newBtnRoot

                width: dropListView.width
                height: 130
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
//                color: "salmon"
                opacity: 0.5

                Image {
                    id: newBtn
                    anchors.centerIn: parent
                    sourceSize: Qt.size(100, 100)
                    asynchronous : false
                    source: "qrc:/images/edit.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("onClicked newBtn !!!")
    //                        popMenu.show()
                            rootPage.showEditPage("OPEN_NEW", "", medoRecordManager.currentTime(), "", medoRecordManager.getNewAttPath())
                        }
                    }
                }
            }

            ListModel {
                id: popModel
                Component.onCompleted: {
                    popModel.append({
                                        "iconUrl": "qrc:/images/edit.png",
                                        "iconText": "文本记事",
                                        "iconBackgroundColor":"#ffb900"  //ca3
                                    });

                    popModel.append({
                                        "iconUrl": "qrc:/images/edit.png",
                                        "iconText": "语音记事",
                                        "iconBackgroundColor":"#ffb900"  //ca3
                                    });
                }
            }

            CMenuPopLayer
            {
                id: popMenu
                dataModel: popModel
                onTriggered: {
                    console.log("CMedoDropListView popModel.index:", index)
                    switch (index)
                    {
                    case 0:
                        rootPage.showEditPage("")
                        break;
//                    case 1:
//                        if (person)
//                            window.dial(group.remoteUids[0])
//                        else
//                            contactDBusService.addContact(group.remoteUids[0])
//                        break;
//                    case 2:
//                        if (person && utils.encryptedMessage)
//                            window.safeDial(group.remoteUids[0])
//                        else
//                            window.dial(group.remoteUids[0])
//                        break;
//                    case 3:
//                        window.safeDial(group.remoteUids[0])
//                        break;
                    default:
                        break;
                    }
                    hide();
                }

                onBackKeyReleased: {
                    hide();
                }
            }
        }

        Component.onCompleted:{
            var tmp = Qt.createComponent(Qt.resolvedUrl("CMedoDropListView.qml", Component.Asynchronous));
            tmp.destroy();
            tmp = Qt.createComponent(Qt.resolvedUrl("CMedoEditPage.qml", Component.Asynchronous));
            tmp.destroy();
        }
    }
}
