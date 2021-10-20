import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

Item {
    required property var profileModel
    id: top
    anchors.fill: parent

    signal saved()
    signal discarded()
    signal canceled()

    function save() {
        profileModel.save()
        saved()
    }

    function openIfUnsaved() {
        var unsaved = profileModel && profileModel.unsavedChanges
        if (unsaved) unsavedChangesDlg.open()
        return unsaved
    }

    Dialog {
        id: unsavedChangesDlg
        x: (parent.width-width)/2
        y: (parent.height-height)/2
        modal: true
        standardButtons: Dialog.Save | Dialog.Cancel | Dialog.Discard

        onAboutToShow: {
            //setting here to avoid binding and changing while unshowing
            title = "save changes to '" + profileModel.name + "'?"
        }

        onAccepted: {
            if (profileModel.hasIssues) {
                saveAnywayDlg.open()
            } else {
                top.save()
            }
        }
        onDiscarded: {
            close()
            top.discarded()
        }
        onRejected: top.canceled()
    }

    SaveAnywayDialog {
        id: saveAnywayDlg
        profileModel: top.profileModel

        Component.onCompleted: {
            standardButtons = standardButtons | Dialog.Discard
        }

        onAccepted: saved()
        onDiscarded: {
            close()
            unsavedChangesDlg.discarded()
        }
        onRejected: top.canceled()
    }
}
