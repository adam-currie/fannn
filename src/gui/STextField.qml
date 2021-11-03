import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

TextField {
    id: control
    property real spaceAboveLine: 7
    color: Material.foreground
    bottomPadding: 8 + spaceAboveLine
    leftPadding: 5
    rightPadding: 5
    selectByMouse: true

    implicitWidth: contentWidth + rightPadding + leftPadding + 2

    onActiveFocusChanged: {
        if (!activeFocus) select(0,0)
    }

    background: Rectangle {
        y: control.height - height - control.bottomPadding + control.spaceAboveLine
        implicitWidth: 120
        height: control.activeFocus || control.hovered ? 2 : 1
        color: control.activeFocus ? control.Material.accentColor
                                   : (control.hovered ? control.Material.primaryTextColor : control.Material.hintTextColor)
    }
}
