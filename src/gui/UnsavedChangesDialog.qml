import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

Dialog {
    required property var profileModel
    modal: true
    title: "profile has unsaved changes"
    standardButtons: Dialog.Save | Dialog.Cancel | Dialog.Discard
    onAccepted: profileModel.save()
}
