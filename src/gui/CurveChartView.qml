import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts
import Fannn

ChartView {
    required property var curve
    property int ticks: 11

    margins.top: 0
    margins.left: 0
    margins.right: 0
    margins.bottom: 0

    antialiasing: true
    backgroundColor: "transparent"
    legend.visible: false


    HorzExtendedXYModelWrapper {
        id: extendedCurve
        model: curve
        //todo: min and max
    }

    ValueAxis {
        id: axisX
        labelsColor: "white"//todo: not sure why Material.foreground doesn't work
        min: curve.minX
        max: curve.maxX
        tickCount: ticks
        labelsFont.bold: true
    }

    ValueAxis {
        id: axisY
        labelsColor: "white"//todo: not sure why Material.foreground doesn't work
        min: curve.minY
        max: curve.maxY
        tickCount: ticks
        labelsFont.bold: true
    }

    LineSeries {
        id: series
        axisX: axisX
        axisY: axisY
    }

    VXYModelMapper {
        xColumn: 0
        yColumn: 1
        series: series
        model: extendedCurve
    }

}
