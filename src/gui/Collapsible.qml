import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

Page {
    id: collapsible

    default property alias content: contentView.data
    property bool collapsed: false

    height: collapsed? header.height : implicitHeight

    header: Button {
        topInset: 0
        bottomInset: 0
        onClicked: collapsed = !collapsed
        text: title
        font.pixelSize: 18
        font.capitalization: "MixedCase"
        background: Rectangle {
            color: Material.primary
            Ripple {
                clipRadius: 1
                width: parent.width
                height: parent.height
                pressed: header.pressed
                anchor: header
                active: header.down
                color: header.flat && header.highlighted ?  header.Material.highlightedRippleColor : header.Material.rippleColor
            }
        }
    }

    Item {
        id: wrapper
        width: collapsible.width
        visible: height > 0
        clip: true
        height: collapsed? 0 : contentView.implicitHeight

        Behavior on height {
            NumberAnimation {
                id: anim
                duration: 120
            }
        }

        Item {
            id: contentView
            implicitHeight: childrenRect.height
            width: parent.width
            anchors {
                bottom: collapsed? wrapper.bottom : undefined
                top: !collapsed? wrapper.top : undefined
            }
        }
    }
}
