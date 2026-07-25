#pragma once

#include <QObject>
#include <QTouchEvent>
#include <QQuickItem>
#include <QMap>
#include <QDateTime>

class GestureHandler : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Edge {
        NoEdge,
        LeftEdge,
        RightEdge,
        TopEdge,
        BottomEdge
    };
    Q_ENUM(Edge)

    explicit GestureHandler(QQuickItem *parent = nullptr);

signals:
    void swipeUp(qreal velocity);
    void swipeDown(qreal velocity);
    void swipeLeft(qreal velocity);
    void swipeRight(qreal velocity);
    void edgeSwipe(Edge edge, qreal progress);
    void longPress(const QPointF &pos);
    void pinch(qreal scale);

protected:
    bool event(QEvent *event) override;
    void touchEvent(QTouchEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    struct TouchPoint {
        QPointF startPos;
        QPointF currentPos;
        qint64 startTime;
        bool isEdge = false;
        Edge edge = NoEdge;
    };

    QMap<int, TouchPoint> m_activeTouches;
    int m_longPressTimerId = 0;
    QPointF m_longPressPos;
    static constexpr int EDGE_THRESHOLD = 30;
    static constexpr int SWIPE_VELOCITY_THRESHOLD = 1000;
};
