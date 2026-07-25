#include "compositor.h"
#include <QGuiApplication>
#include <QQuickWindow>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("MobileOS");
    app.setOrganizationName("mobileos");

    MobileCompositor compositor;
    compositor.create();

    return app.exec();
}
