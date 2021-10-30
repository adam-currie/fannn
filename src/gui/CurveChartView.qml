import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts
import Fannn

ChartView {
    required property var curve

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
        tickCount: 5
    }

    ValueAxis {
        id: axisY
        labelsColor: "white"//todo: not sure why Material.foreground doesn't work
        min: curve.minY
        max: curve.maxY
        tickCount: 5
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
