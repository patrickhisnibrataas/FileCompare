pragma Singleton
import QtQuick 2.12

QtObject {
    readonly property QtObject colors: QtObject {
        readonly property color white: "#FFFFFF"
        readonly property color black: "#000000"
        readonly property string transparent: "transparent"
    }

    readonly property QtObject font: QtObject {
        readonly property QtObject pointSize: QtObject {
            readonly property real h1: 24
            readonly property real h2: 18
            readonly property real h3: 12
        }
    }

    readonly property real margin: 10

    //Add more material colors when needed from here: https://doc.qt.io/qt-5.12/qtquickcontrols2-material.html#pre-defined-material-colors
    readonly property QtObject material: QtObject {
        readonly property color accent: theme.dark.color.purple

        readonly property QtObject theme: QtObject {
            readonly property QtObject dark: QtObject {
                readonly property QtObject color: QtObject {
                    readonly property color purple: "#CE93D8"
                }
            }
        }
    }

    readonly property real spacing: 10
}
