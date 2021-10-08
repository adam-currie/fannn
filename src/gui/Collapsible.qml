import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: collapsible

    default property alias content: contentView.data
    property bool collapsed: false

    height: collapsed? header.height : implicitHeight

    background: Rectangle {
        color: Material.accent
        opacity: .5
    }

    header: Button {
        topInset: 0
        bottomInset: 0
        onClicked: collapsed = !collapsed
        text: title //debug collapsible.width + "," + collapsible.height
        font.pixelSize: 18
        font.capitalization: "MixedCase"
        flat: true
        background: Rectangle {
            color: Material.accent
        }
    }

    Item {
        id: contentView
        visible: !collapsed
        implicitHeight: childrenRect.height
        height: collapsed? 0 : implicitHeight
        width: collapsible.width
    }
}
