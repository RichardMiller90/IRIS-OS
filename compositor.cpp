#include "compositor.h"
#include "appwindow.h"
#include <QWaylandXdgToplevel>
#include <QWaylandXdgPopup>
#include <QWaylandBufferRef>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QQuickItem>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QWaylandTextInput>

MobileCompositor::MobileCompositor(QObject *parent)
    : QWaylandCompositor(parent)
{
}

MobileCompositor::~MobileCompositor()
{
    qDeleteAll(m_windows);
}

void MobileCompositor::create()
{
    setSocketName("mobile-os");
    setupOutputs();
    setupShell();
    setupInput();

    m_engine = new QQmlApplicationEngine(this);
    m_engine->rootContext()->setContextProperty("compositor", this);

    m_keyboard = new VirtualKeyboard(this);
    m_engine->rootContext()->setContextProperty("keyboardController", m_keyboard);

    m_engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    QWaylandCompositor::create();
}

void MobileCompositor::setupOutputs()
{
    m_primaryOutput = new QWaylandOutput(this, this);
    m_primaryOutput->setGeometry(QRect(0, 0, 1080, 2400));
    m_primaryOutput->setPhysicalSize(QSize(68, 151));
    m_primaryOutput->setRefreshRate(60000);
    m_primaryOutput->setScaleFactor(3.0);
    addOutput(m_primaryOutput);
}

void MobileCompositor::setupShell()
{
    m_xdgShell = new QWaylandXdgShell(this);
    connect(m_xdgShell, &QWaylandXdgShell::xdgSurfaceCreated,
            this, &MobileCompositor::onXdgSurfaceCreated);

    m_wlShell = new QWaylandWlShell(this);
    connect(m_wlShell, &QWaylandWlShell::wlShellSurfaceCreated,
            this, &MobileCompositor::onWlShellSurfaceCreated);
}

void MobileCompositor::setupInput()
{
    m_seat = new QWaylandSeat(this, QWaylandSeat::Capability::Pointer
                                     | QWaylandSeat::Capability::Keyboard
                                     | QWaylandSeat::Capability::Touch);

    m_textInputManager = new QWaylandTextInputManager(this);
    connect(m_textInputManager, &QWaylandTextInputManager::textInputCreated,
            [this](QWaylandTextInput *textInput) {
        connect(textInput, &QWaylandTextInput::showInputPanel, [this, textInput]() {
            if (auto *surf = textInput->focusedSurface())
                m_keyboard->showForSurface(surf);
        });
        connect(textInput, &QWaylandTextInput::hideInputPanel, [this]() {
            m_keyboard->hide();
        });
    });
}

void MobileCompositor::onXdgSurfaceCreated(QWaylandXdgSurface *xdgSurface)
{
    auto *toplevel = xdgSurface->toplevel();
    if (!toplevel) return;

    QString appId = toplevel->appId();
    manageWindow(xdgSurface->surface(), appId);

    connect(toplevel, &QWaylandXdgToplevel::setFullscreenRequested,
            [this, xdgSurface](bool fullscreen) {
        xdgSurface->toplevel()->sendFullscreen(fullscreen);
    });

    connect(toplevel, &QWaylandXdgToplevel::setMaximizedRequested,
            [this, xdgSurface](bool maximized) {
        xdgSurface->toplevel()->sendMaximized();
    });
}

void MobileCompositor::onWlShellSurfaceCreated(QWaylandWlShellSurface *shellSurface)
{
    manageWindow(shellSurface->surface(), QString());
}

void MobileCompositor::manageWindow(QWaylandSurface *surface, const QString &appId)
{
    auto *window = new AppWindow(surface, appId, this);
    m_windows.append(window);
    setActiveWindow(window);
    emit windowAdded(window);
    emit windowsChanged();

    connect(surface, &QWaylandSurface::destroyed, this, &MobileCompositor::onSurfaceDestroyed);
}

void MobileCompositor::setActiveWindow(AppWindow* window)
{
    if (m_activeWindow == window) return;

    if (m_activeWindow) {
        m_activeWindow->setActive(false);
        if (auto *surf = m_activeWindow->surface())
            surf->raise();
    }

    m_activeWindow = window;

    if (m_activeWindow) {
        m_activeWindow->setActive(true);
        if (auto *surf = m_activeWindow->surface())
            surf->raise();
    }

    emit activeWindowChanged();
}

AppWindow* MobileCompositor::activeWindow() const { return m_activeWindow; }
bool MobileCompositor::locked() const { return m_locked; }
VirtualKeyboard* MobileCompositor::keyboard() const { return m_keyboard; }

QQmlListProperty<AppWindow> MobileCompositor::windows()
{
    return QQmlListProperty<AppWindow>(this, &m_windows);
}

void MobileCompositor::setLocked(bool locked)
{
    if (m_locked == locked) return;
    m_locked = locked;
    emit lockedChanged();
}

void MobileCompositor::showHomeScreen()
{
    if (m_activeWindow) {
        m_activeWindow->setActive(false);
        m_activeWindow = nullptr;
        emit activeWindowChanged();
    }
}

void MobileCompositor::handleGesture(Qt::GestureType type, const QPointF &pos, qreal value)
{
    switch (type) {
    case Qt::SwipeGesture:
        if (value > 0.5) {
            showHomeScreen();
        } else if (value < -0.5) {
            // Show control center
        }
        break;
    case Qt::PanGesture:
        if (pos.x() < 50) {
            // Back gesture
        }
        break;
    default:
        break;
    }
}

void MobileCompositor::onSurfaceDestroyed()
{
    auto *surface = qobject_cast<QWaylandSurface*>(sender());
    if (!surface) return;

    for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
        if ((*it)->surface() == surface) {
            auto *win = *it;
            if (m_activeWindow == win) {
                m_activeWindow = nullptr;
                emit activeWindowChanged();
            }
            m_windows.erase(it);
            emit windowRemoved(win);
            emit windowsChanged();
            win->deleteLater();
            break;
        }
    }
}

void MobileCompositor::render()
{
    // Frame callback handled by Qt Quick
}
