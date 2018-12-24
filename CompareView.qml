import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.4
import QtQuick.Dialogs 1.3
import io.brataas.filetools 1.0

Item {
    Label {
        id: title
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        text: "File Compare"
        font.pointSize: 24
    }

    Row {
        id: row

        property var folders: []

        anchors {
            top: title.bottom
            left: parent.left
        }
        spacing: 10
        width: parent.width * 0.5

        Button {
            text: "Browse.."
            onClicked: fileDialog.open()
        }

        Button {
            text: "Clear"
            onClicked: {
                folders.clear()
                fileList.clear()
                fileCompare.clear()
            }
        }

        Button {
            text: "Compare"
            onClicked: {
                fileList.build(folders.inputFolders)
                fileCompare.compare(fileList.files)
            }
        }

        TextField {
            id: filter
            placeholderText: "filter"
        }

        ProgressBar {
            from: 0
            to: 100
            value: fileCompare.progress
            anchors.verticalCenter: parent.verticalCenter
            Material.accent: Material.Purple
        }
    }

    ListView {
        id: extensionsListView
        anchors {
            top: row.bottom
            left: parent.left
            bottom: parent.bottom
        }
        model: fileList.extensions
        delegate: Text {
            text: modelData
            color: "white"
        }
        width: 100
    }

    ListView {
        id: foldersListView
        anchors {
            top: row.bottom
            left: extensionsListView.right
            bottom: parent.bottom
        }
        width: (parent.width - extensionsListView.width) * 0.5
        model: folders.inputFolders
        delegate: Text {
            text: modelData
            color: "white"
        }
    }

    ListView {
        id: duplicateFilesListView
        anchors {
            top: row.bottom
            left: foldersListView.right
            right: parent.right
            bottom: parent.bottom
        }
        model: fileCompare.duplicateFiles
        delegate: TreeViewEntry {
            width: parent.width
            delegate: Text {
                text: modelData
                color: "white"
            }

            model: FileType
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
