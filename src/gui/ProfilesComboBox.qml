import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import Fannn

ComboBox {
    id: combo
    property Item modalDlgParent: this

    implicitContentWidthPolicy: ComboBox.WidestText

    UnsavedChangesDialog {
        id: switchConfirmationDlg
        parent: modalDlgParent
        profileModel: model.currentProfile
        Connections {
            target: switchConfirmationDlg
            function onSaved() { model.loadProfile(currentValue) }
            function onDiscarded() { model.loadProfile(currentValue) }
            function onCanceled() {
                //todo: check model combo.currentIndex = _currentLoadedIndex
            }
        }
    }

    flat: true
    textRole: "name"

    onActivated: {
        var openedDlg = switchConfirmationDlg.openIfUnsaved()
        if (!openedDlg) model.loadProfile(currentValue)
    }

    popup.onAboutToShow: model.loadProfileNames()

    Connections {
        target: combo.delegateModel
        function onModelUpdated() {
            combo.currentIndex = 0
        }
    }

    //todo: favorite button
    delegate: MenuItem {
        width: ListView.view.width
        text: combo.textRole ? (Array.isArray(combo.model) ? modelData[combo.textRole] : model[combo.textRole]) : modelData
        Material.foreground: combo.currentIndex === index ? ListView.view.contentItem.Material.accent : ListView.view.contentItem.Material.foreground
        highlighted: combo.highlightedIndex === index
        hoverEnabled: combo.hoverEnabled
    }

}
