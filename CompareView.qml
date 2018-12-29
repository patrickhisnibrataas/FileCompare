import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.4
import QtQuick.Dialogs 1.3
import io.brataas.filetools 1.0

Column {
    Label {
        id: title
        text: "File Compare"
        font.pointSize: Global.font.pointSize.h1
    }

    Column {
        spacing: Global.spacing
        height: parent.height - title.height

        Row {
            id: row
            spacing: Global.spacing

            Button {
                text: "Browse.."
                onClicked: fileDialog.open()
            }

            TextField {
                id: filter
                placeholderText: "filter"
            }

            Button {
                text: "Compare"
                onClicked: {
                    fileList.build(folders.inputFolders)
                    fileCompare.compare(fileList.files)
                }
            }

            Button {
                text: "Clear"
                onClicked: {
                    folders.clear()
                    fileList.clear()
                    fileCompare.clear()
                }
            }

            ProgressBar {
                from: 0
                to: 100
                value: fileCompare.progress
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            height: parent.height
            spacing: Global.spacing

            Column {
                spacing: Global.spacing

                Label {
                    text: "Extensions found"
                    font.pointSize: Global.font.pointSize.h3
                }

                ListView {
                    id: extensionsListView
                    implicitHeight: Math.max(Math.min(contentHeight, 400), 1)
                    implicitWidth: 150
                    model: fileList.extensions
                    delegate: Text {
                        text: modelData
                        color: Global.colors.white
                    }
                }
            }

            Spacer {
                orientation: Spacer.Orientation.Vertical
                height: parent.height
            }

            Column {
                spacing: Global.spacing

                Label {
                    text: "Folders to compare content"
                    font.pointSize: Global.font.pointSize.h3
                }
                ListView {
                    id: foldersListView
                    implicitHeight: Math.max(Math.min(contentHeight, 400), 1)
                    implicitWidth: 250
                    model: folders.inputFolders
                    delegate: Text {
                        text: modelData
                        color: Global.colors.white
                    }
                }
            }

            Spacer {
                orientation: Spacer.Orientation.Vertical
                height: parent.height
            }

            Column {
                spacing: Global.spacing

                Label {
                    text: "Duplicate content"
                    font.pointSize: Global.font.pointSize.h3
                }
                ListView {
                    id: duplicateFilesListView
                    implicitHeight: Math.max(Math.min(contentHeight, 400), 1)
                    implicitWidth: 250
                    model: fileCompare.duplicateFiles
                    delegate: TreeViewEntry {
                        width: parent.width
                        delegate: Text {
                            text: modelData
                            color: Global.colors.white
                        }

                        model: FileType
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        visible: false
        title: "Please choose a file"
        folder: shortcuts.home
        selectFolder: true
        onAccepted: folders.addInputFolder(String(fileDialog.folder).replace("file:///", ""))
    }

    Folders {
        id: folders
    }

    FileList {
        id: fileList
        filter: filter.text.replace(" ", "").split(",")
    }

    FileCompare {
        id: fileCompare
    }
}
