import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

TextField {
    color: Material.foreground
    onActiveFocusChanged: {
        if (!activeFocus) select(0,0)
    }
}
