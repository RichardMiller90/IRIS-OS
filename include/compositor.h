#pragma once

#include <QWaylandCompositor>
#include <QWaylandXdgShell>
#include <QWaylandXdgSurface>
#include <QWaylandOutput>
#include <QWaylandSeat>
#include <QWaylandKeyboard>
#include <QWaylandPointer>
#include <QWaylandTouch>
#include <QWaylandWlShell>
#include <QWaylandTextInputManager>
#include <QTimer>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlListProperty>

#include "virtualkeyboard.h"

class AppWindow;

class MobileCompositor : public QWaylandCompositor
{
    Q_OBJECT
    Q_PROPERTY(AppWindow* activeWindow READ activeWindow WRITE setActiveWindow NOTIFY activeWindowChanged)
    Q_PROPERTY(bool locked READ locked WRITE setLocked NOTIFY lockedChanged)
    Q_PROPERTY(VirtualKeyboard* keyboard READ keyboard CONSTANT)
    Q_PROPERTY(QQmlListProperty<AppWindow> windows READ windows NOTIFY windowsChanged)

public:
    explicit MobileCompositor(QObject *parent = nullptr);
    ~MobileCompositor();

    void create() override;

    AppWindow* activeWindow() const;
    bool locked() const;
    VirtualKeyboard* keyboard() const;
    QQmlListProperty<AppWindow> windows();

    QWaylandSeat* defaultSeat() const { return m_seat; }

public slots:
    void setActiveWindow(AppWindow* window);
    void setLocked(bool locked);
    void showHomeScreen();
    void showAppDrawer();
    void handleGesture(Qt::GestureType type, const QPointF &pos, qreal value);

signals:
    void activeWindowChanged();
    void lockedChanged();
    void windowAdded(AppWindow* window);
    void windowRemoved(AppWindow* window);
    void windowsChanged();

private slots:
    void onXdgSurfaceCreated(QWaylandXdgSurface *xdgSurface);
    void onWlShellSurfaceCreated(QWaylandWlShellSurface *shellSurface);
    void onSurfaceDestroyed();
    void render();

private:
    void setupShell();
    void setupOutputs();
    void setupInput();
    void manageWindow(QWaylandSurface *surface, const QString &appId);

    QWaylandXdgShell *m_xdgShell = nullptr;
    QWaylandWlShell *m_wlShell = nullptr;
    QWaylandOutput *m_primaryOutput = nullptr;
    QWaylandSeat *m_seat = nullptr;
    QWaylandTextInputManager *m_textInputManager = nullptr;

    QList<AppWindow*> m_windows;
    AppWindow* m_activeWindow = nullptr;
    AppWindow* m_homeScreen = nullptr;
    VirtualKeyboard *m_keyboard = nullptr;

    bool m_locked = true;
    QQmlApplicationEngine *m_engine = nullptr;
};
