import QtQuick 2.12

Item {
    id: root

    property bool expanded: false
    property real margin: 10
    property alias model: listView.model
    property alias delegate: listView.delegate

    implicitHeight: expanded ? listView.contentHeight + root.margin : expander.implicitHeight + root.margin

    Rectangle {
        id: expander

        color: "white"
        anchors {
            top: parent.top
            left: parent.left
        }

        implicitHeight: expanded ? listView.contentHeight : root.margin
        width: root.margin

        MouseArea {
            anchors.fill: parent
            onClicked: root.expanded = !root.expanded
        }
    }

    ListView {
        id: listView
        visible: expanded
        anchors {
            top: parent.top
            left: expander.right
            leftMargin: root.margin
            right: parent.right
        }
        height: contentHeight
    }
}
