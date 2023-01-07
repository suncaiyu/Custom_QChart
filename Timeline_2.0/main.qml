import QtQuick 2.0
import QtQuick.Window 2.0
import Furrain.Control 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Column {
        anchors.fill: parent
        Timeline {
            width:parent.width
            height: 50
        }
        MyChart {
            width : parent.width
            height: parent.height - 50
        }
    }
}
