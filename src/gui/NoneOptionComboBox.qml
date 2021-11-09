import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Button {
    property Item everythingItem: this
    required property var model
    property string noneText: "none"
    property string textRole
    readonly property alias currentText: control._currentText
    property int currentIndex: -1
    property bool lostOurCurrentItemWaitingForChange: false

    property string _currentText:
        (currentIndex === -1) ?
            noneText :
            delegateModel.items.get(currentIndex).model[textRole]

    id: control
    flat: true
    topInset: 0
    bottomInset: 0
    font.capitalization: "MixedCase"
    font.pointSize: 13
    padding: 5
    leftPadding: 10

    contentItem: Label {
        text: control.text
        font: control.font
        color: (currentIndex === -1) ? control.Material.hintTextColor :
            control.flat && control.highlighted ? control.Material.accentColor :
            control.highlighted ? control.Material.primaryHighlightedTextColor : control.Material.foreground
    }

    Rectangle {
        y: control.height - height
        width: control.width
        height: control.activeFocus || control.hovered ? 2 : 1
        color: control.activeFocus ? control.Material.accentColor
                                   : (control.hovered ? control.Material.primaryTextColor : control.Material.hintTextColor)
    }

    onCurrentIndexChanged: {
        lostOurCurrentItemWaitingForChange = false
    }

    DelegateModel {
        // just here to give us easy access to data
        id: delegateModel
        model: control.model
        delegate: Item {}
    }

    ColorImage {
        x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        color: control.enabled ? control.Material.foreground : control.Material.hintTextColor
        source: "qrc:/qt-project.org/imports/QtQuick/Controls/Material/images/drop-indicator.png"
    }

    text: _currentText

    onClicked: {
        if (dropDown.closedForAtLeastOneUpdate)
            dropDown.open()
    }

    function invalidateCurrentIndex() {
        if (currentIndex == -1)
            return

        control.currentIndex = -1
        //todo: animate going red or something for a second and fading back to normal
    }

    Connections {
        target: control.model
        function onRowsRemoved(parent, start, end) {
            if (end < control.currentIndex){
                let span = 1 + start - end
                control.currentIndex -= span
            } else if (start <= control.currentIndex) {
                control.invalidateCurrentIndex()
            }
        }
        function onRowsMoved(parent, start, end, destParent, destStart) {
            console.error("can't handle moving rows yet"); //todo: https://doc.qt.io/qt-5/qabstractitemmodel.html#beginMoveRows
            control.invalidateCurrentIndex()
        }
    }

    Menu {
        id: dropDown

        property bool closedForAtLeastOneUpdate: true

        parent: everythingItem
        topPadding: 0
        bottomPadding: 0

        Timer {
            id: closeTimer
            interval: 0
            repeat: false
            onTriggered: dropDown.closedForAtLeastOneUpdate = !dropDown.opened
        }

        onClosed: {
            closeTimer.start()
        }

        onAboutToShow: {
            let pos = control.mapToItem(parent, 0, control.height)
            x = pos.x
            y = pos.y
        }

        onOpened: {
            closedForAtLeastOneUpdate = false
        }

        MenuItem {
            text:  control.noneText
            font.bold: true
            Material.foreground: (control.currentIndex === -1 || highlighted) ?
                                     control.Material.accent :
                                     control.Material.foreground
            onClicked: control.currentIndex = -1
        }

        Repeater {
            model: control.model
            MenuItem {
                required property string name
                required property int index
                text: name
                Material.foreground: (index === control.currentIndex || highlighted) ?
                                         control.Material.accent :
                                         control.Material.foreground
                onClicked: control.currentIndex = index
            }
        }
    }

}
