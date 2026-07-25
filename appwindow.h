#pragma once

#include <QObject>
#include <QWaylandSurface>
#include <QQuickItem>
#include <QTimer>
#include <QSGSimpleTextureNode>

class AppWindow : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QWaylandSurface* surface READ surface CONSTANT)
    Q_PROPERTY(QString appId READ appId CONSTANT)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    QML_ELEMENT

public:
    enum State {
        Hidden,
        Minimized,
        Active,
        Suspended
    };
    Q_ENUM(State)

    explicit AppWindow(QWaylandSurface *surface = nullptr,
                       const QString &appId = QString(),
                       QObject *parent = nullptr);

    QWaylandSurface* surface() const;
    QString appId() const;
    QString title() const;
    State state() const;
    bool active() const;

    void setState(State state);
    void setActive(bool active);

signals:
    void titleChanged();
    void stateChanged();
    void activeChanged();
    void closed();

public slots:
    void close();
    void minimize();
    void maximize();
    void suspend();
    void resume();

protected:
    QSGNode* updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    QWaylandSurface *m_surface = nullptr;
    QString m_appId;
    QString m_title;
    State m_state = Hidden;
    bool m_active = false;
    QTimer *m_suspendTimer = nullptr;
};
