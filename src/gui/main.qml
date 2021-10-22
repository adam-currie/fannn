import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material
import Fannn

ApplicationWindow {
    id: window
    minimumHeight: 440
    minimumWidth: 440
    visibility: Window.Maximized
    width: 700
    height: 700

    visible: true
    title: qsTr("Fannn Profile Editor")
    property bool _closing: false

    required property var profilesModel

    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: "#ff793b"

    Item {
        id: windowAreaItem
        anchors.fill: parent
    }

    UnsavedChangesDialog {
        id: closingWindowSaveDlg
        parent: windowAreaItem
        profileModel: profilesModel.currentProfile
        onSaved: function (close) {
            window._closing = true
            window.close()
        }
        onDiscarded: function (close) {
            window._closing = true
            window.close()
        }
    }


    onClosing: function (close) {
        if (!_closing) {
            var openedDlg = closingWindowSaveDlg.openIfUnsaved()
            close.accepted = !openedDlg
        }
    }

    SensorListModel {
        id: sensorsModel
        profile: profilesModel.currentProfile
    }

    GovernorListModel {
        id: governorsModel
        profile: profilesModel.currentProfile
    }

    CurveListModel {
        id: curvesModel
        profile: profilesModel.currentProfile
    }

    Settings {
        id: settings
    }

    Dialog {
        id: settingsDialog
        x: Math.round((parent.width - width) / 2)
        y: Math.round(parent.height / 6)
        implicitWidth: 300
        width: Math.round(Math.min(parent.width, implicitWidth))
        modal: true
        focus: true
        title: "Settings"

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settingsDialog.close()
        }
        onRejected: {
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 8

            RowLayout {
                spacing: 8
            }

        }
    }

    header: ToolBar {
        focusPolicy: Qt.ClickFocus

        ProfilesComboBox {
            id: bongoBox
            modalDlgParent: windowAreaItem
            model: window.profilesModel
            anchors {
                left: parent.left
                leftMargin: 5
            }
        }
        RoundButton {
            id: addButton

            function addProfile(){
                profilesModel.createAndSwitchTo()
                //todo: focus ComboBox textfield for editing new name
            }

            UnsavedChangesDialog {
                id: confirmationDialog
                parent: windowAreaItem
                profileModel: profilesModel.currentProfile
                onSaved: addButton.addProfile()
                onDiscarded: addButton.addProfile()
            }

            icon.name: "list-add-symbolic"
            anchors {
                left: bongoBox.right
                leftMargin: -6
            }
            radius: 0
            flat: true
            onClicked: {
                var openedDlg = confirmationDialog.openIfUnsaved()
                if (!openedDlg) addProfile()
            }
        }

        ToolButton {
            icon.name: "view-more-symbolic"
            anchors.right: parent.right
            onClicked: optionsMenu.open();

            Menu {
                id: optionsMenu
                transformOrigin: Menu.TopRight

                Action {
                    text: "Settings"
                    onTriggered: settingsDialog.open()
                }
                Action {
                    text: "About"
                }
            }
        }
    }

    Flickable {
        width: parent.width
        height: parent.height
        contentHeight: column.height
        ScrollBar.vertical: ScrollBar { }


        Pane {
            anchors.fill: parent
            focusPolicy: Qt.ClickFocus
        }

        Column {
            id: column
            width: parent.width
            Row {
                anchors.left: parent.left
                anchors.leftMargin: 10
                height: 50
                width: parent.width
                spacing: 5
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -2
                    text: "update interval(ms): "
                }
                STextField {
                    padding: 4
                    id: updateIntervalField
                    anchors.bottom: parent.bottom
                    validator: RegularExpressionValidator {
                        //can't use IntValidator because we need to temporarily allow blank text
                        regularExpression: /[0-9]*/
                    }

                    property bool _isBlank: false

                    text: _isBlank || !profilesModel.currentProfile ?
                              "" : profilesModel.currentProfile.updateIntervalMs

                    onEditingFinished: {
                        _isBlank = false
                    }

                    onTextChanged: {
                        var nextIsBlank = !text
                        if (!nextIsBlank) {
                            if (profilesModel.currentProfile)
                                profilesModel.currentProfile.updateIntervalMs = text
                            }
                        _isBlank = nextIsBlank
                    }
                }
            }
            Collapsible {
                width: parent.width
                title: "Sensors"
                SpacedGridView {
                    width: parent.width
                    minCellWidth: 200
                    delegate: SensorDelegate {
                        sensors: sensorsModel
                    }
                    model: sensorsModel
                }
            }
            Collapsible {
                width: parent.width
                title: "Curves"

                SpacedGridView {
                    id: curvesGrid
                    width: parent.width
                    minCellWidth: 280
                    cellHeight: 240
                    delegate: CurveDelegate {
                        curves: curvesModel
                    }
                    model: curvesModel
                }
                GridViewCellPredictor {
                    grid: curvesGrid
                    AddButton {
                        y: parent.height/2 - height/2
                        x: y
                        onClicked: curvesGrid.add()
                    }
                }
            }
            Collapsible {
                width: parent.width
                title: "Governors"

                SpacedGridView {
                    id: govGrid
                    width: parent.width
                    minCellWidth: 250
                    cellHeight: 200
                    delegate: GovernorDelegate {
                        governors: governorsModel
                    }
                    model: governorsModel
                }
                GridViewCellPredictor {
                    grid: govGrid
                    AddButton {
                        y: parent.height/2 - height/2
                        x: y
                        onClicked: governorsModel.add()
                    }
                }
            }
        }
    }

    footer: ToolBar {
        focusPolicy: Qt.ClickFocus
        Button {
            text: "save profile"
            flat: true
            anchors.right: parent.right
            anchors.margins: 5
            enabled: profilesModel.currentProfile ?
                profilesModel.currentProfile.unsavedChanges : false
            onClicked: profilesModel.currentProfile.save()
        }
    }
}

