import QtQuick 2.12

Item {
    id: root

    property alias model: listView.model
    property alias delegate: listView.delegate

    Internal {
        id: internal
        property bool expanded: false
    }

    implicitHeight: internal.expanded ? listView.contentHeight + Global.margin : expander.implicitHeight + Global.margin

    Rectangle {
        id: expander

        color: Global.colors.white
        anchors {
            top: parent.top
            left: parent.left
        }

        implicitHeight: internal.expanded ? listView.contentHeight : Global.margin
        width: Global.margin

        MouseArea {
            anchors.fill: parent
            onClicked: internal.expanded = !internal.expanded
        }
    }

    ListView {
        id: listView
        visible: internal.expanded
        anchors {
            top: parent.top
            left: expander.right
            leftMargin: Global.margin
            right: parent.right
        }
        height: contentHeight
    }
}
