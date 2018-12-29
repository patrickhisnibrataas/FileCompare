import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600

    property var model: [
        {
            'name': "File Compare",
            'view': compareView
        },
        {
            'name': "Files to Folders",
            'view': filesToFoldersView
        }

    ]

    ListView {
        id: sidebar
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: 100
        model: root.model
        delegate: Button {
            text: modelData.name
            onClicked: {
                if (stackView.currentItem !== modelData.view)
                    stackView.replace(modelData.view)
            }
        }
    }

    Spacer {
        id: spacer
        anchors {
            top: parent.top
            left: sidebar.right
        }
        orientation: Spacer.Orientation.Vertical
        height: parent.height
    }

    StackView {
        id: stackView
        anchors {
            top: parent.top
            left: spacer.right
            leftMargin: Global.margin
            right: parent.right
            bottom: parent.bottom
        }
        replaceEnter: Transition {}
        replaceExit: Transition {}

        CompareView {
            id: compareView
            visible: false
        }

        FilesToFolders {
            id: filesToFoldersView
            visible: false
        }
    }
}
