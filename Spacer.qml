import QtQuick 2.12

Rectangle {
    enum Orientation {
        Horizontal,
        Vertical
    }

    property var orientation: Orientation.Horizontal

    implicitHeight: orientation === Spacer.Orientation.Horizontal ? 1 : 100
    implicitWidth: orientation === Spacer.Orientation.Horizontal ? 100 : 1
    color: Global.colors.white
}
