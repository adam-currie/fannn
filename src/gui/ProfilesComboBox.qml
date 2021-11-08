import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

ComboBox {
    id: combo
    property real minWidth: 10
    property real bonusWidth: 50
    property Item modalDlgParent: this
    property int _currentLoadedIndex: 0

    Component.onCompleted: {
        if (count > 0) combo.model.loadProfile(currentValue)
        combo.width = Math.max(contentItem.contentWidth, minWidth) + bonusWidth
    }

    function _loadCurrent() {
        model.loadProfile(currentValue)
        _currentLoadedIndex = currentIndex
    }

    UnsavedChangesDialog {
        id: switchConfirmationDlg
        parent: modalDlgParent
        profileModel: model.currentProfile
        Connections {
            target: switchConfirmationDlg
            function onSaved() { _loadCurrent() }
            function onDiscarded() { _loadCurrent() }
            function onCanceled() {
                combo.currentIndex = _currentLoadedIndex
            }
        }
    }

    flat: true
    textRole: "name"

    onActivated: {
        var openedDlg = switchConfirmationDlg.openIfUnsaved()
        if (!openedDlg) _loadCurrent()
    }

    popup.onAboutToShow: {
        if (count === 0) return
        var widest = minWidth
        var originalCurrentIndex = currentIndex
        model.loadProfileNames()
        do {
          widest = Math.max(widest, contentItem.contentWidth)
          currentIndex = (currentIndex + 1) % count
        } while(currentIndex !== originalCurrentIndex)
        combo.width = widest + bonusWidth
    }

    popup.onAboutToHide: {
        combo.width = Math.max(contentItem.contentWidth, minWidth) + bonusWidth
    }

    Connections {
        target: combo.model
        function onCurrentProfileChanged(p) {
            combo.currentIndex = combo.model.indexOf(p.name)
        }
    }

}
