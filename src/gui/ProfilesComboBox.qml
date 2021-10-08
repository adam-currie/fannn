import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

ComboBox {
    id: combo
    property var bonusWidth: 60
    property var minWidth: 10

    popup.onAboutToHide: { minWidth = 10 }
    width: Math.max(minWidth, contentItem.contentWidth + bonusWidth)

    Component.onCompleted: {
        if (count > 0) combo.model.loadProfile(currentValue)
    }

    UnsavedChangesDialog {
        id: switchConfirmationDlg
        profileModel: model.currentProfile
        Connections {
            target: switchConfirmationDlg
            function onAccepted() { combo.model.loadProfile(currentValue) }
            function onDiscarded() { combo.model.loadProfile(currentValue) }
            //onRejected do nothing
        }
    }

    flat: true
    textRole: "name"

    onActivated: (model.currentProfile && model.currentProfile.unsavedChanges) ?
                     switchConfirmationDlg.open() :
                     model.loadProfile(currentValue)

    popup.onAboutToShow: {
        if (count === 0) return
        var widest = 0
        var originalCurrentIndex = currentIndex
        model.loadProfileNames()
        do {
          widest = Math.max(widest, contentItem.contentWidth)
          currentIndex = (currentIndex + 1) % count
        } while(currentIndex !== originalCurrentIndex)
        minWidth = widest + bonusWidth
    }

    Connections {
        target: combo.model
        function onCurrentProfileChanged(p) {
            combo.currentIndex = combo.model.indexOf(p.name)
        }
    }

}
