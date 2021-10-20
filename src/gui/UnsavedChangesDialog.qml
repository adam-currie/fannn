import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

Dialog {
    required property var profileModel
    x: (parent.width-width)/2
    y: (parent.height-height)/2
    modal: true
    title: "profile has unsaved changes"
    standardButtons: Dialog.Save | Dialog.Cancel | Dialog.Discard
    onAccepted: profileModel.save()
    onDiscarded: close()
}
