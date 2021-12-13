import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

GridView {
    property real minCellWidth: 320
    property real maxCellWidth: 380
    property real cellPadding: 5
    height: contentHeight + cellPadding*2
    topMargin: cellPadding
    leftMargin: cellPadding
    rightMargin: cellPadding
    bottomMargin: cellPadding
    cellWidth:
        Math.min (
            maxCellWidth,
            contentItem.width / Math.min(
                                    count,
                                    Math.floor((contentItem.width)/minCellWidth)
                                )
        )
    cellHeight: 150
}

