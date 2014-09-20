import QtQuick 1.1
import "core"

Item {
    id: field
    property int clickx: 0
    property int clicky: 0

    width: 450; height: 450

    Image {source: "core/pics/background.png"; anchors.fill: parent; fillMode: Image.Tile}

    Grid {
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 9; spacing: 1

        Repeater {
            id:repeater
            model: tiles           //？？？？
            delegate: Tile{}     //代理   格式  ？
        }
    }

    //底部左边 游戏数据显示  退出、炸弹、旗子数量等
    Row {
        id: gamedata
        x: 20; spacing: 20
        anchors.bottom: field.bottom; anchors.bottomMargin: 15

        //退出
        Image {
            source: "core/pics/quit.png"
            scale: quitMouse.pressed ? 0.8:1.0
            smooth: quitMouse.pressed    //使在转换时画质顺畅，但在硬件上会变慢，在图大小与原大小一致时不起作用，默认true
            y:10
            MouseArea{
                id:quitMouse
                anchors.fill: parent
                anchors.margins: -20     //当scale大小改变时，MouseArea也改变，以防不能产生click信号??????????????
                onClicked: Qt.quit()
            }
        }

        //地雷数量
        Column{
            spacing: 2
            Image {source: "core/pics/bomb-color.png"}
            Text {anchors.horizontalCenter: parent.horizontalCenter; color:"white"; text: numMines}
        }

        //旗子数量
        Column{
            spacing: 2
            Image {source: "core/pics/flag-color.png"}
            Text {anchors.horizontalCenter: parent.horizontalCenter; color:"white"; text: numFlags}
        }
    }

    //底部右边  小脸
    Image {
        anchors.bottom: field.bottom; anchors.bottomMargin: 15
        anchors.right: field.right;   anchors.rightMargin: 20
        source: isPlaying ? "core/pics/face-smile.png" :
                            hasWon ? "core/pics/face-smile-big.png":"core/pics/face-sad.png"
        MouseArea{anchors.fill: parent; onPressed: reset()}
    }

    //错误时，显示文字  C++未编译时
    Text {
        anchors.centerIn: parent; width: parent.width-20
        horizontalAlignment: Text.AlignHCenter //????????对齐的是哪里？
        wrapMode: Text.WordWrap
        text: "MineHunt Game error,please compile c++."
        color: "white"; font.bold: true; font.pixelSize: 14
        visible: tiles == undefined  //在tiles没有定义时，说明C还没有编译
    }
}
