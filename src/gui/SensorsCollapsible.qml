import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

Collapsible {
    property alias model: grid.model
    property alias cellHeight: grid.cellHeight
    property real minCellWidth: 320
    title: "sensors"
    GridView {
        id: grid
        width: parent.width
        height: childrenRect.height
        cellWidth: width/Math.floor(width/minCellWidth)
        cellHeight: 150
        delegate: SensorDelegate {
            width: grid.cellWidth
            height: grid.cellHeight
        }
    }
}
