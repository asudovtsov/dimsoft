import QtQuick
import org.kde.kwin

SceneEffect {
    id: effect

    delegate: Rectangle {
        color: "blue"

        Text {
            anchors.centerIn: parent
            color: "white"
            text: "Hello world!"
        }
    }

    ScreenEdgeHandler {
        enabled: true
        edge: ScreenEdgeHandler.TopEdge
        onActivated: effect.visible = !effect.visible
    }

    ShortcutHandler {
        name: "Toggle Hello World Effect"
        text: "Toggle Hello World Effect"
        sequence: "Meta+H"
        onActivated: effect.visible = !effect.visible
    }

    PinchGestureHandler {
        direction: PinchGestureHandler.Direction.Contracting
        fingerCount: 3
        onActivated: effect.visible = !effect.visible
    }
}