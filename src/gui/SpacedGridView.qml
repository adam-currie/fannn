import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Item {
    property alias model: inner.model
    property alias delegate: inner.delegate
    property alias cellHeight: inner.cellHeight
    property real minCellWidth: 320
    property real cellSpacing: 5

    height: inner.height

    GridView {
        id: inner
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: cellSpacing
        height: childrenRect.height
        cellWidth: width/Math.floor(width/minCellWidth)
        cellHeight: 150
    }
}
