import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Scroll")

    ScrollView {
        anchors.fill: parent

        ListView {
            id: listView
            width: parent.width
            model: 20
            delegate: ItemDelegate {
                text: "Item " + (index + 1)
                width: listView.width
            }
        }
    }
}
