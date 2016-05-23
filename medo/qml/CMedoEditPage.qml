import QtQuick 2.0
import QtGraphicalEffects 1.0
import com.syberos.basewidgets 2.0

CPage {
    id: root


    color: "#72cfD7"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "#72cfD7"

    // orientationLock: CPageOrientation.LockPortrait

    property string textContent: ""
    property string lastEditTime: ""
    property string openType: "OPEN_EDIT"
    property string recordId: ""
    property bool recording: false
    property string attPath: ""
    property alias attList: attachementList.model

    FontLoader { id: localFont; source: "qrc:///fonts/DroidSansFallback.ttf" }

    onStatusChanged: {
        if (status === CPageStatus.Show) {
            if (root.openType === "OPEN_NEW" && root.textContent == "") {
                editArea.focus = true
                //               root.recordId = new Date().toLocaleTimeString(Qt.locale(""));
                //               console.log("new id=======================",root.recordId)
            } else if(root.openType === "OPEN_EDIT" ) {

            }

        } else if (status === CPageStatus.WillShow) {
            // todo
            root.recording = false
        }
    }

    contentAreaItem: Item {

        Rectangle {
            anchors.fill: parent
            color: gUiConst.getValue("CB1")
        }

        CTitleBar {
            id: titleBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            titleItemEnabled: false
            leftItemEnabled: false
            rightItemEnabled: false
            MouseArea {
                id: newFileButton

                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }

                width: 320

                onClicked: {
                    editArea.focus = false
                    if(editArea.text === "") {

                        console.log("delete--record")
                        medoRecordManager.deleteRecord(root.recordId);
                        pageStack.pop();
                        return;
                    }
                    console.log("update--record")
                    medoRecordManager.updateRecord(root.recordId, editArea.text,root.attPath, medoRecordManager.currentTime())//only update voice
                    pageStack.pop();

                    //                if (editArea.text != root.textContent) {
                    //                    console.log("Edit Done!!!")
                    //                    if (root.openType === "OPEN_EDIT") {
                    //                        console.log("medo === OPEN_EDIT")
                    //                        medoRecordManager.updateRecord(root.recordId, editArea.text,root.attPath, medoRecordManager.currentTime())
                    //                    } else if (root.openType === "OPEN_NEW"){
                    //                        console.log("medo === OPEN_NEW")
                    //                        if (editArea.text !== "") {
                    //                            medoRecordManager.newRecord(root.recordId,editArea.text, root.attPath, medoRecordManager.currentTime())
                    //                        }

                    //                    }
                    //                } else {
                    //                    console.log("Not Edit !!!")
                    //                    medoRecordManager.updateRecord(root.recordId, editArea.text,root.attPath, medoRecordManager.currentTime())//only update voice
                    //                }

                    //                pageStack.pop();
                    //                editArea.realText = ""
                }

                //                Rectangle {
                //                    anchors.fill: parent
                //                    color: "yellow"
                //                }

                Text {
                    id: icon
                    anchors {
                        top: parent.top
                        left: parent.left
                        bottom: parent.bottom

                        leftMargin: 40
                        topMargin: 2
                    }

                    verticalAlignment: Text.AlignVCenter

                    font.family: "FontAwesome"
                    font.pixelSize: 40

                    color: "white"
                    text: "\uf053"
                }

                Text {
                    anchors {
                        top: parent.top
                        left: icon.right
                        bottom: parent.bottom

                        leftMargin: 20
                    }

                    verticalAlignment: Text.AlignVCenter

                    font.pixelSize: 36

                    color: "white"
                    text: "保存并返回"
                }
            }

            backgroundComponent: Rectangle {
                id: titleBarBackground
                color: "#72cfD7"
                z: parent.z - 1
            }

            //            Component.onCompleted: {
            //                leftItem.textColor = Qt.binding(
            //                            function() {
            //                                return leftItem.pressed ? gUiConst.getValue("color08") :
            //                                                          gUiConst.getValue("color05")
            //                            }
            //                            )
            //                leftItem.width = Qt.binding(function(){return 180})
            //            }
        }


        CLinkTextArea {
            id: editArea
            width: parent.width;
            anchors.top: titleBar.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.bottom: voiceBtn.top
            anchors.bottomMargin: 30
            font.family: localFont.name
            font.pixelSize : 36
            // font.bold: true
            focus: false
            textColor : "#1b1c27"
            opacity: 0.75
            wrapMode: TextEdit.Wrap
            realText: root.textContent


            onTextChanged: {
                console.log("----------textedittext:-----------", text)
            }
        }

        Connections {
            target: Qt.inputMethod
            onVisibleChanged: {
                if (Qt.inputMethod.visible) {
                    attachementList.visible = false;
                }
            }
        }

        Rectangle {
            id: voiceBtn

            //            Item {
            //                anchors.fill: parent

            //                LinearGradient {
            //                    anchors.fill: parent
            //                    start: Qt.point(0, 0)
            //                    end: Qt.point(0, parent.height)
            //                    gradient: Gradient {
            //                        GradientStop { position: 0.0; color: "#EEEEEE" }
            //                        GradientStop { position: 1.0; color: "#FFFFFF" }
            //                    }
            //                }

            //            }

            width: parent.width
            height: 130
            anchors.bottom: attachementList.top //parent.bottom
            anchors.bottomMargin: 4
            anchors.left: parent.left
            anchors.right: parent.right
            //            color: "salmon"
            opacity: 0.5

            Text {
                id: dateArea
                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                height: 30
                font.family: localFont.name
                font.pixelSize :gUiConst.getValue("font11")
                font.bold: true
                color:"#8F7A66"
                text: root.lastEditTime
            }

            Image {
                id: recordBtn
                anchors.centerIn: parent
                sourceSize: Qt.size(100, 100)
                asynchronous : false
                source: root.recording ? "qrc:/images/stop.png" : "qrc:/images/start.png"


                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("onClicked recordBtn !!!--text",editArea.text)
                        if(editArea.text === "")
                            return;

                        root.recording = !root.recording
                        console.log("onClicked recordBtn !!!",root.recording)
                        if (root.recording) {
                            attachementList.visible = true
                            medoRecordManager.startRecorder(root.recordId,root.attPath)
                            gScreenInfo.setWindowProperty("SCREEN_ALWAYS_ON",true)
                        } else {
                            medoAttManager.stopRecorder()
                            gScreenInfo.setWindowProperty("SCREEN_ALWAYS_ON",false)
                        }
                    }
                }
            }
            Image {
                id: listImage
                anchors.right: parent.right
                anchors.rightMargin:40
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/lists.png"
                sourceSize: Qt.size(100, 100)
                MouseArea{
                    anchors.fill: parent
                    onClicked: {

                        if (attachementList.visible) {
                            attachementList.visible = false;
                            if (editArea.focus) {
                                Qt.inputMethod.show();
                            }
                        } else {
                            Qt.inputMethod.hide();
                            attachementList.visible = true;
                        }
                    }
                }
            }
        }
        ListView{
            id:attachementList
            width: parent.width
            height:visible ? parent.height/2 : 0
            anchors.bottom: parent.bottom
            visible: false
            //            model:root.attList

            Rectangle {
                anchors.fill: parent
                color: "#F5F6F8"
                z: -1
            }

            clip: true
            Behavior on height {
                NumberAnimation{duration: 100}
            }
            delegate: SlideDelegate {
                id:slideDelegate
                width: attachementList.width
                height:attachementList.height / 6

                _rightMenuItem: Item {
                    width: 140
                    height: slideDelegate.height

                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 1

                        color: "#72cfD7"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            medoRecordManager.deleteAttachment(model.modelData.parentId, model.modelData.path, model.modelData.name);
                            root.attList.removeItem(index);
                        }
                    }

                    Text {
                        anchors.centerIn: parent

                        font.family: "FontAwesome"
                        font.pixelSize: 40

                        color: "white"
                        text: "\uf014"
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "white"

                    anchors.margins: 1

                    border {
                        color: "#F3F4F6"
                        width: 2
                    }
                    z:-1
                }

                property Item textItem: Text{
                    parent:slideDelegate.slideItem
                    anchors.verticalCenter: slideDelegate.slideItem.verticalCenter
                    anchors.left: slideDelegate.slideItem.left
                    anchors.leftMargin: 40
                    text:modelData.name
                    font.family: localFont.name
                    font.pixelSize: 32
                    // font.bold: true
                    color:"#1b1c27"
                    opacity: 0.75
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    z:100
                }

                onReleased: {
                    if(slideDelegate.realPressed) {
                        console.log("play=================",model.modelData.path,model.modelData.name)
                        medoAttManager.play(model.modelData.path + "/" + model.modelData.name)
                    }
                    slideDelegate.realPressed = false

                }
            }

        }
    }
}
