import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Item {
    required property GridView grid
    property int columnCount: grid.contentItem.width/grid.cellWidth
    x: grid.contentItem.x + grid.cellWidth * (grid.count%columnCount)
    y: grid.contentItem.y + grid.cellHeight * Math.floor(grid.count/columnCount)
    width: grid.cellWidth
    height: grid.cellHeight
}
