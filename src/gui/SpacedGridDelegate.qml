import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

Item {
    default property alias data: inner.data
    property real spacing: parent.parent.parent.cellSpacing
    width: parent.parent.cellWidth
    height: parent.parent.cellHeight
    Rectangle {
        id: inner
        anchors.fill: parent
        anchors.margins: spacing
        color: Material.shade(Material.background, Material.Shade100)
        radius: 4
    }
}
