import QtQuick 2.12
import QtQuick.Controls 2.12
import QtLocation 5.12
import QtPositioning 5.12

Rectangle {
    width: 800
    height: 600

    Map {
        anchors.fill: parent
        plugin: Plugin {
            name: "osm"  // OpenStreetMap plugin
        }
        zoomLevel: 5
        center {
            latitude: 0
            longitude: 0
        }
    }
}
