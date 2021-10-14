import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

TextField {
    color: Material.foreground
    leftPadding: 5
    rightPadding: 5
    onActiveFocusChanged: {
        if (!activeFocus) select(0,0)
    }
}
