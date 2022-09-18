import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Window {
    width: 950
    height: 525
    visible: true
    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    title: qsTr("Whiteboard Stream")

    Image {
        id: streamImage

        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        fillMode: Image.PreserveAspectFit
        property bool counter: false
        visible: true
        source: "image://stream/image"
        asynchronous: false
        cache: false

        function reload()
        {
            counter = !counter
            source = "image://stream/image?id=" + counter
        }
    }


    Connections{
        target: liveImageProvider

        function onUpdated()
        {
            streamImage.reload()
        }

    }
}
