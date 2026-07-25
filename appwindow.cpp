#include "appwindow.h"
#include <QWaylandSurface>
#include <QWaylandBufferRef>
#include <QSGTexture>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

AppWindow::AppWindow(QWaylandSurface *surface, const QString &appId, QObject *parent)
    : QQuickItem(qobject_cast<QQuickItem*>(parent))
    , m_surface(surface)
    , m_appId(appId)
{
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptTouchEvents(true);

    if (m_surface) {
        connect(m_surface, &QWaylandSurface::redraw, this, [this]() { update(); });
        connect(m_surface, &QWaylandSurface::titleChanged, this, &AppWindow::titleChanged);
        m_title = m_surface->title();
    }

    m_suspendTimer = new QTimer(this);
    m_suspendTimer->setInterval(300000);
    m_suspendTimer->setSingleShot(true);
    connect(m_suspendTimer, &QTimer::timeout, this, &AppWindow::suspend);
}

QWaylandSurface* AppWindow::surface() const { return m_surface; }
QString AppWindow::appId() const { return m_appId; }
QString AppWindow::title() const { return m_title; }
AppWindow::State AppWindow::state() const { return m_state; }
bool AppWindow::active() const { return m_active; }

void AppWindow::setState(State state)
{
    if (m_state == state) return;
    m_state = state;
    emit stateChanged();
    update();
}

void AppWindow::setActive(bool active)
{
    if (m_active == active) return;
    m_active = active;
    emit activeChanged();
    if (active) {
        setState(Active);
        m_suspendTimer->stop();
    }
}

void AppWindow::close()
{
    if (m_surface) {
        m_surface->close();
    }
    emit closed();
}

void AppWindow::minimize()
{
    setState(Minimized);
    setActive(false);
}

void AppWindow::maximize()
{
    if (parentItem()) {
        QRectF parentRect = parentItem()->boundingRect();
        qreal statusBarHeight = 40;
        setPosition(QPointF(0, statusBarHeight));
        setSize(QSizeF(parentRect.width(), parentRect.height() - statusBarHeight));
    }
    setState(Active);
    setActive(true);
}

void AppWindow::suspend()
{
    if (m_state == Suspended) return;
    setState(Suspended);
}

void AppWindow::resume()
{
    if (m_state != Suspended) return;
    setState(Active);
    m_suspendTimer->stop();
}

QSGNode* AppWindow::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (!m_surface || m_state == Suspended) {
        delete oldNode;
        return nullptr;
    }

    auto *node = static_cast<QSGSimpleTextureNode*>(oldNode);
    if (!node) {
        node = new QSGSimpleTextureNode();
        node->setOwnsTexture(false);
    }

    QWaylandBufferRef buffer = m_surface->currentBuffer();
    if (!buffer.hasContent()) return node;

    QSGTexture *texture = buffer.toOpenGLTexture();
    if (!texture) return node;

    node->setTexture(texture);
    node->setRect(boundingRect());

    return node;
}
