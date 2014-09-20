import QtQuick 1.1

Flipable {
    id: flipable
    property int angle: 0

    width: 40; height: 40
    transform: Rotation{ origin.x: 20; origin.y: 20; axis.y: 1; axis.z: 0; angle: flipable.angle}

    front: Image{
        source:"pics/front.png"
        width: 40; height: 40

        Image{
            anchors.centerIn: parent
            source: "pics/flag.png";    opacity: modelData.hasFlag

            Behavior on opacity{ NumberAnimation{} }  //渐变  没有参数也行吗？？？
        }
    }

    back: Image{
        source: "pics/back.png"
        width: 40; height: 40

        Text {
            anchors.centerIn: parent
            text: modelData.hint; color:"white"; font.bold:true
            opacity: !modelData.hasMine && modelData.hint>0  //没有地雷，且线索数字大于0
        }
        //如果有地雷
        Image{
            anchors.centerIn: parent
            source: "pics/bomb.png"; opacity: modelData.hasMine
        }
        Explosion {id: expl}
    }

    states: State{             //状态描述
        name:"back"; when: modelData.flipped
        PropertyChanges{ target: flipable; angle:180 }
    }
                               //状态转换
    property real pauseDur: 250
    transitions: Transition{
        SequentialAnimation{  //连续转换: 脚本动作计算时间、停顿、旋转、脚本动作是否爆炸

            ScriptAction{     //脚本动作  ????????????????
                script: {
                    var ret = Math.abs(flipable.x - field.clickx)+Math.abs(flipable.y - field.clicky) //按距离延时
                    if(modelData.hasMine && modelData.flipped) //有雷，同时满足翻转
                        pauseDur = ret * 3
                    else pauseDur = ret
                }
            }
            PauseAnimation { duration: pauseDur }
            RotationAnimation{ easing.type: Easing.InOutQuad}
            ScriptAction {
                script: if(modelData.hasMine && modelData.flipped)
                            expl.explode = true
            }
        }
    }

    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            field.clickx = flipable.x  //鼠标点击时，把click坐标定为当前flipable的坐标
            field.clicky = flipable.y
            var row = Math.floor(index / 9)  //行= 坐标除以9 取整
            var col = index-(Math.floor(index / 9)*9) //列= 坐标减去每行9个    index从0开始，行列都从0开始
            if(mouse.button == undefined ||mouse.button == Qt.RightButton)
                flag(row,col)
            else
                flip(row,col)
        }
        onPressAndHold: {
            field.clickx = flipable.x
            field.clicky = flipable.y
            var row = Math.floor(index / 9)
            var col = index - ( Math.floor(index / 9) * 9 )
            flag(row,col)
        }
    }
}
