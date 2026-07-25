#pragma once

#include <QGuiApplication>
#include <QWindow>
#include <QColor>

class MobileApplication : public QGuiApplication
{
    Q_OBJECT
public:
    explicit MobileApplication(int &argc, char **argv);

    void setStatusBarStyle(bool dark);
    void setScreenOrientation(Qt::ScreenOrientation orientation);
    void requestPermission(const QString &permission);
    void sendNotification(const QString &title, const QString &body);
    void setKeepScreenOn(bool on);

signals:
    void appResumed();
    void appSuspended();
    void backButtonPressed();

private:
    bool m_keepScreenOn = false;
};
