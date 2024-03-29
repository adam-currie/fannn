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

    ProfileListModel {
        id: profileListModel
    }

    SensorListModel {
        id: sensorsModel
        profile: profileListModel.currentProfile
    }

    GovernorListModel {
        id: governorsModel
        profile: profileListModel.currentProfile
        sensors: sensorsModel
    }

    CurveListModel {
        id: curvesModel
        profile: profileListModel.currentProfile
    }

    ControllerListModel {
        id: controllersModel
        profile: profileListModel.currentProfile
    }

    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: "#ff793b"

    UnsavedChangesDialog {
        id: closingWindowSaveDlg
        parent: mainPage
        profileModel: profileListModel.currentProfile
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

    Page {
        id: mainPage
        anchors.fill: parent

        header: ToolBar {
            focusPolicy: Qt.ClickFocus

            ProfilesComboBox {
                id: bongoBox
                modalDlgParent: mainPage
                model: window.profilesModel
                anchors {
                    left: parent.left
                    leftMargin: 5
                }
            }
            RoundButton {
                id: addButton

                function addProfile(){
                    profileListModel.createAndSwitchTo()
                    //todo: focus ComboBox textfield for editing new name
                }

                UnsavedChangesDialog {
                    id: confirmationDialog
                    parent: mainPage
                    profileModel: profileListModel.currentProfile
                    onSaved: addButton.addProfile()
                    onDiscarded: addButton.addProfile()
                }

                icon.name: "list-add-symbolic"
                anchors.left: bongoBox.right
                anchors.leftMargin: -6
                radius: 0
                flat: true
                onClicked: {
                    var openedDlg = confirmationDialog.openIfUnsaved()
                    if (!openedDlg) addProfile()
                }
            }

            RoundButton {
                id: favButton
                icon.name: (profileListModel?.activeProfileName && profileListModel.activeProfileName === profileListModel?.currentProfile?.name) ?
                               "starred-symbolic" :
                               "non-starred-symbolic"
                anchors.left: addButton.right
                anchors.leftMargin: -5
                radius: 0
                flat: true
                onClicked: profileListModel.activeProfileName = profileListModel.currentProfile.name
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
                        text: "update interval(ms)"
                    }
                    STextField {
                        id: updateIntervalField
                        anchors.bottom: parent.bottom
                        horizontalAlignment: TextInput.horizontalCenter
                        validator: RegularExpressionValidator {
                            //can't use IntValidator because we need to temporarily allow blank text
                            regularExpression: /[0-9]{0,9}/
                        }

                        property bool _isBlank: false

                        text: _isBlank || !profileListModel.currentProfile ?
                                  "" : profileListModel.currentProfile.updateIntervalMs

                        onEditingFinished: {
                            _isBlank = false
                        }

                        onTextChanged: {
                            var nextIsBlank = !text
                            if (!nextIsBlank) {
                                if (profileListModel.currentProfile)
                                    profileListModel.currentProfile.updateIntervalMs = text
                                }
                            _isBlank = nextIsBlank
                        }
                    }
                }
                Collapsible {
                    width: parent.width
                    title: "Sensors"
                    SpacedGridView {
                        interactive: false
                        width: parent.width
                        minCellWidth: 220
                        cellHeight: 140
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
                        interactive: false
                        width: parent.width
                        minCellWidth: 280
                        cellHeight: 240
                        delegate: CurveDelegate {
                            curves: curvesModel
                            everythingItem: mainPage
                        }
                        model: curvesModel
                    }
                    GridViewCellPredictor {
                        grid: curvesGrid
                        AddButton {
                            y: parent.height/2 - height/2
                            x: y
                            onClicked: curvesModel.add()
                        }
                    }
                }
                Collapsible {
                    width: parent.width
                    title: "Governors"

                    SpacedGridView {
                        id: govGrid
                        interactive: false
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
                Collapsible {
                    width: parent.width
                    title: "Controllers"
                    SpacedGridView {
                        id: controllerGrid
                        interactive: false
                        width: parent.width
                        minCellWidth: 250
                        cellHeight: 165
                        delegate: ControllerDelegate {
                            controllers: controllersModel
                        }
                        model: controllersModel
                    }
                }
            }
        }

        footer: ToolBar {
            focusPolicy: Qt.ClickFocus
            SaveAnywayDialog {
                id: saveAnywayDlg
                parent: mainPage
                profileModel: profileListModel.currentProfile
            }
            Button {
                text: "save profile"
                flat: true
                anchors.right: parent.right
                anchors.margins: 5
                enabled: profileListModel.currentProfile ?
                    profileListModel.currentProfile.unsavedChanges : false
                onClicked: profileListModel.currentProfile.hasIssues ?
                               saveAnywayDlg.open() :
                               profileListModel.currentProfile.save()
            }
        }
    }
}

