import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Item {
    default property alias data: inner.data
    width: (parent && parent.parent)? parent.parent.cellWidth : 0
    height: (parent && parent.parent)? parent.parent.cellHeight : 0
    Rectangle {
        id: inner
        anchors.fill: parent
        anchors.margins: (parent && parent.parent && parent.parent.parent)? parent.parent.parent.cellPadding : 0
        color: Material.shade(Material.background, Material.Shade100)
        radius: 4
    }
}
