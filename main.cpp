#include <QApplication>
#include "qmlapplicationviewer.h"

#include "tiledata.h"
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;
    ////////添加部分//////////////////
    qmlRegisterType<TileData>();             //注册TileData类,这个类不需要在QML中实例化
    MinehuntGame* game = new MinehuntGame(); //创建一个游戏，调用构造函数。

    viewer.engine()->rootContext()->setContextObject(game);
    //要将QML和C++结合起来,实现传统的开发方式,核心的代码就是
    //viewer.engine()->rootContext()->setContextObject(game);
    //这样就可以在QML中调用game实例的属性和函数了.

    ////////////////////////////
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/MyMineHunt/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
