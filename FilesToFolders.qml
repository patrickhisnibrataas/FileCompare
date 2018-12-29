import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import io.brataas.filetools 1.0

Column {
    Label {
        id: title
        text: "Files to Folders"
        font.pointSize: Global.font.pointSize.h1
    }

    Column {
        spacing: Global.spacing
        width: parent.width

        Column {
            Item {
                implicitHeight: buttonBrowseInputFolders.implicitHeight
                width: 200

                Label {
                    anchors.left: parent.left
                    text: "Input folders"
                    font.pointSize: Global.font.pointSize.h3
                    anchors.verticalCenter: parent.verticalCenter
                }

                Button {
                    id: buttonBrowseInputFolders
                    anchors.right: parent.right
                    text: "Browse"
                    onClicked: inputFoldersDialog.open()
                }
            }

            ListView {
                id: inputFoldersListView
                implicitWidth: 100
                implicitHeight: Math.min(contentHeight, 200)
                model: filesToFolders.inputFolders
                delegate: Text {
                    text: modelData
                    color: Global.colors.white
                }
            }
        }

        Spacer {
            orientation: Spacer.Orientation.Horizontal
            width: parent.width
        }

        Column {
            Item {
                implicitHeight: buttonBrowseOutputFolders.implicitHeight
                width: 200

                Label {
                    anchors.left: parent.left
                    text: "Output folder"
                    font.pointSize: Global.font.pointSize.h3
                    anchors.verticalCenter: parent.verticalCenter
                }

                Button {
                    id: buttonBrowseOutputFolders
                    anchors.right: parent.right
                    text: "Browse"
                    onClicked: outputFolderDialog.open()
                }
            }

            Label {
                text: filesToFolders.outputFolder
            }
        }

        Spacer {
            orientation: Spacer.Orientation.Horizontal
            width: parent.width
        }

        Column {
            spacing: Global.spacing

            Label {
                text: "Sort by"
                font.pointSize: Global.font.pointSize.h3
            }

            ListView {
                id: sortByView

                height: Math.min(contentHeight, 200)
                width: 100
                model: filesToFolders.sortBy
                delegate: Rectangle {
                    id: sortByViewDelegate
                    height: childrenRect.height
                    width: childrenRect.width
                    color: ListView.isCurrentItem ? Global.material.accent : Global.colors.transparent

                    Text {
                        text: modelData
                        color: Global.colors.white
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: sortByViewDelegate.ListView.view.currentIndex = index
                    }
                }
            }
        }

        Spacer {
            orientation: Spacer.Orientation.Horizontal
            width: parent.width
        }

        Button {
            text: "Copy"
            onClicked: filesToFolders.sortToFolders(filesToFolders.sortBySelection)
        }
    }

    FileDialog {
        id: inputFoldersDialog
        visible: false
        title: "Please choose a file"
        folder: shortcuts.home
        selectFolder: true
        onAccepted: filesToFolders.addInputFolder(String(inputFoldersDialog.folder).replace("file:///", ""))
    }

    FileDialog {
        id: outputFolderDialog
        visible: false
        title: "Please choose a file"
        folder: shortcuts.home
        selectFolder: true
        onAccepted: filesToFolders.outputFolder = String(outputFolderDialog.folder).replace("file:///", "")
    }

    SortFilesToFolders {
        id: filesToFolders
        sortBySelection: sortByView.currentIndex
    }
}
