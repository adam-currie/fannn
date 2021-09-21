import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Page {
    default property alias content: contentView.data
    property alias delegate: contentView.delegate
    property alias model: contentView.model
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
        text: title
        font.pixelSize: 18
        font.capitalization: "MixedCase"
        flat: true
        background: Rectangle {
            color: Material.accent
        }
    }
    GridView {
        visible: !collapsed
        implicitHeight: childrenRect.height
        height: collapsed? 0 : implicitHeight
        id: contentView
    }
}

//Container {
//    id: container
//    property bool open: true
//    property int headerHeight: 50

//    Button {
//        width: container.width
//        height: headerHeight
//        text: container.text
//        font.capitalization: "MixedCase"
//        onClicked: container.open = !container.open
//    }
//    contentItem: ListView {
//        y: headerHeight
//        width: container.width
//        implicitHeight: container.height - headerHeight
//        visible: height > 0
//        clip: true
//        height: container.open ? implicitHeight : 0
//        Behavior on height {
//            NumberAnimation {
//                duration: 1000
//            }
//        }
//        model: container.contentModel
//        interactive: false
//    }
//}


