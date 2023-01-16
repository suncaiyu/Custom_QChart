import QtQuick 2.0
import QtQuick.Window 2.0
import Furrain.Control 1.0
import QtQuick.Controls 2.14

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    property SkinChooser chooser: SkinChooser{}

    Column {
        anchors.fill: parent
        focus: true
        Keys.enabled: true
        Keys.onPressed: {
            if (event.key === Qt.Key_2) {
                chooser.current = chooser.light
            }
            if (event.key === Qt.Key_1) {
                chooser.current = chooser.dark
            }
        }
        Timeline {
            id : timeline
            width:parent.width
            height: 25
            backgroundStart: chooser.current.timelineBackgroundStart
            backgroundEnd: chooser.current.timelineBackgroundEnd
            borderColor: chooser.current.borderColor
            labelBackgroundColor: chooser.current.labelTextBackgroundColor
            labelTextColor: chooser.current.labelTextColor
            Component.onCompleted: {
                mychart.initTimelineContext(timeline.getTimelineContext())
            }
        }
        MyChart {
            id : mychart
            width : parent.width
            height: parent.height - 25
            ScrollBar {
                id : scrollbar
                x : parent.width - 15
                y : 0
                width: 15
                height:parent.height
                onPositionChanged: {
                    mychart.updatePanValue(position)
                }
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.MidButton
                onWheel: {
                    if (wheel.angleDelta.y > 0) {
                        scrollbar.decrease()
                    }
                    if (wheel.angleDelta.y < 0 && scrollbar.size < 1) {
                        scrollbar.increase()
                    }
                }
            }

            onUpdateScrollbarValueSignal: {
                scrollbar.size = step / max //0.65 position最大值0.35
            }
        }
    }
}
