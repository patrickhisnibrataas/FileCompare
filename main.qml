import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 720

    property var model: [
        {
            'name': "Compare",
            'view': compareView
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

    StackView {
        id: stackView
        anchors {
            top: parent.top
            left: sidebar.right
            right: parent.right
            bottom: parent.bottom
        }

        CompareView {
            id: compareView
            visible: false
        }
    }
}
