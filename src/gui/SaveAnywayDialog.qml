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
    onAboutToShow: {
        //setting here to avoid binding and changing while unshowing
        title = "profile: '" + profileModel.name + "' has issues,\nare you sure you want to save it?"
    }
    standardButtons: Dialog.Save | Dialog.Cancel
    onAccepted: profileModel.save()
}

