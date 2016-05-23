import QtQuick 2.0
import QtQuick.Window 2.0
import QtGraphicalEffects 1.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    FontLoader {
        source: "qrc:/fonts/FontAwesome.ttf"
    }

    initialPage: CPage {

        FontLoader { id: localFont; source: "qrc:///fonts/DroidSansFallback.ttf" }

        color: "#72cfD7"
        statusBarHoldEnabled: true
        statusBarHoldItemColor: "#72cfD7"

        onActiveChanged: {
            //            if (active) {
            //                popMenu.show()
            //            }
        }

        contentAreaItem: Item {
            id: rootPage
            //        onStatusChanged: {
            //            if (status === CPageStatus.WillShow) {
            //                popMenu.show()
            //            }
            //        }

            function showEditPage(openType, text, lastEditTime, recordId, attPath,attList) {
//                var component = pageStack.getCachedPage(Qt.resolvedUrl("CMedoEditPage.qml"),"CMedoEditPage");

//                component.openType = openType
//                component.textContent = text
//                component.lastEditTime = lastEditTime
//                component.recordId = recordId
//                component.attPath = attPath
//                component.attList = attList

//                console.log("showEditPage :: openType = ", component.openType)
//                console.log("showEditPage :: textContent = ", component.textContent)
//                console.log("showEditPage :: lastEditTime = ", component.lastEditTime)
//                console.log("showEditPage :: recordId = ", component.recordId)
//                console.log("showEditPage :: attPath = ", component.attPath)
//                console.log("showEditPage :: attPathlist = ", component.attList)

//                pageStack.push(component);
                var  component = Qt.createComponent(Qt.resolvedUrl("CMedoEditPage.qml"));
                pageStack.push(component, {openType: openType,textContent:text,lastEditTime:lastEditTime,recordId:recordId,attPath:attPath,attList:attList});
            }

            Item {
                anchors.fill: parent

                CTitleBar {
                    id: titleBar

                    anchors.top: parent.top
                    implicitWidth: parent.width
                    implicitHeight: 94
                    z:10
                    leftItemEnabled: false
                    titleAreaCentered: true

                    backgroundComponent: Rectangle {
                        id: titleBarBackground
                        color: "#72cfD7"
                        z: parent.z - 1
                    }

                    titleItemComponent: Text {
                        text: "Medo"
                        color: "#FFFFFF"
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
                    anchors.fill: parent
                    color: "#F5F6F8"
                    z:parent.z-1
                }

                CMedoDropListView {
                    id: dropListView
                    anchors.top: titleBar.bottom
                    anchors.topMargin: 1
                    anchors.bottom: newBtnRoot.top
                    anchors.bottomMargin: 1
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
                    color: "white"

                    Item {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 10

                        LinearGradient {
                            anchors.fill: parent
                            start: Qt.point(0, 0)
                            end: Qt.point(0, 10)
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "#CCCCCC" }
                                GradientStop { position: 1.0; color: "#FFFFFF" }
                            }
                        }

                    }

                    Image {
                        id: newBtn
                        anchors.centerIn: parent
                        sourceSize: Qt.size(100, 100)
                        asynchronous : false
                        source: "qrc:/images/edit.png"
                        opacity: 0.75

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("onClicked newBtn !!!")
                                //                        popMenu.show()
                                var recordId = new Date().toLocaleTimeString(Qt.locale(""));
                                var dateTime = medoRecordManager.currentTime();
                                console.log("new id=======================",recordId)
                                medoRecordManager.newRecord(recordId,"", medoRecordManager.getNewAttPath(),dateTime)
                                rootPage.showEditPage("OPEN_NEW", "",
                                                      dateTime,
                                                      recordId,
                                                      medoRecordManager.getNewAttPath(),
                                                      medoRecordManager.getRecord(recordId).attachmentList)
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

                gScreenInfo.setStatusBarStyle("transwhite");
            }
        }
    }
}
