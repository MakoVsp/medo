import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: root

    orientationLock: CPageOrientation.LockPortrait
    statusBarHoldItemOpacity: 1.0
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "#ffffff"

    property string textContent: ""
    property string lastEditTime: ""
    property string openType: "OPEN_EDIT"
    property string recordId: ""
    property bool recording: false
    property string attPath: ""
    property var attList: null

    FontLoader { id: localFont; source: "qrc:///fonts/DroidSansFallback.ttf" }

    onStatusChanged: {
       if (status === CPageStatus.Show) {
           if (root.openType === "OPEN_NEW" && root.textContent == "") {
               editArea.focus = true
               root.recordId = new Date().toLocaleTimeString(Qt.locale(""));
               console.log("new id=======================",root.recordId)
           } else if(root.openType === "OPEN_EDIT" ) {

           }

       } else if (status === CPageStatus.WillShow) {
           // todo
           root.recording = false
       }
    }

    contentAreaItem: Item{

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
            rightItemEnabled: false
            leftItemText: "保存并返回"
            spacingBetweenLeftBorderAndLeftItem: 40

            Component.onCompleted: {
                leftItem.textColor = Qt.binding(
                            function() {
                                return leftItem.pressed ? gUiConst.getValue("color08") :
                                                          gUiConst.getValue("color05")
                            }
                            )
                leftItem.width = Qt.binding(function(){return 180})
            }

            onLeftItemTriggered: {
                editArea.focus = false

                if (editArea.text != root.textContent) {
                    console.log("Edit Done!!!")
                    if (root.openType === "OPEN_EDIT") {
                        console.log("medo === OPEN_EDIT")
                        medoRecordManager.updateRecord(root.recordId, editArea.text,root.attPath, medoRecordManager.currentTime())
                    } else if (root.openType === "OPEN_NEW"){
                        console.log("medo === OPEN_NEW")
                        if (editArea.text !== "") {
                            medoRecordManager.newRecord(root.recordId,editArea.text, root.attPath, medoRecordManager.currentTime())
                        }
                    }
                } else {
                    console.log("Not Edit !!!")
                    medoRecordManager.updateRecord(root.recordId, editArea.text,root.attPath, medoRecordManager.currentTime())//only update voice
                }

                pageStack.pop();
//                editArea.realText = ""
            }
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
            font.pixelSize : 30
            font.bold: true
            focus: false
            textColor : "#776E62"
            wrapMode: TextEdit.Wrap
            realText: root.textContent


            onTextChanged: {
                console.log("----------textedittext:-----------", text)
            }
        }

        Rectangle {
            id: voiceBtn

            width: parent.width
            height: 130
            anchors.bottom: attachementList.top //parent.bottom
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
                        attachementList.visible = !attachementList.visible
                    }
                }
            }
        }
        ListView{
            id:attachementList
            width: parent.width
            height:visible ? parent.height/3 : 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            visible: false
            model:root.attList
            clip: true
            Behavior on height {
                NumberAnimation{duration: 100}
            }

            delegate:Rectangle{
                width: attachementList.width
                height:100
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text:model.modelData.name
                    font.family: localFont.name
                    font.pixelSize :gUiConst.getValue("font11")
                    font.bold: true
                    color:"#8F7A66"
                }
                Rectangle{
                    width: parent.width
                    anchors.bottom: parent.bottom
                    height:1
                    color: "black"
                }
            }
        }
    }
}
