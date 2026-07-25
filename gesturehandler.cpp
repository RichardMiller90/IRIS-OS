#include "gesturehandler.h"
#include <QTouchEvent>
#include <QDateTime>
#include <QtMath>

GestureHandler::GestureHandler(QQuickItem *parent) : QQuickItem(parent)
{
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void GestureHandler::touchEvent(QTouchEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate: {
        const auto &points = event->points();
        for (const auto &point : points) {
            int id = point.id();

            if (point.state() == QEventPoint::Pressed) {
                TouchPoint tp;
                tp.startPos = point.position();
                tp.currentPos = point.position();
                tp.startTime = QDateTime::currentMSecsSinceEpoch();

                QRectF bounds = boundingRect();
                if (tp.startPos.x() < EDGE_THRESHOLD) {
                    tp.isEdge = true;
                    tp.edge = LeftEdge;
                } else if (tp.startPos.x() > bounds.width() - EDGE_THRESHOLD) {
                    tp.isEdge = true;
                    tp.edge = RightEdge;
                } else if (tp.startPos.y() < EDGE_THRESHOLD) {
                    tp.isEdge = true;
                    tp.edge = TopEdge;
                } else if (tp.startPos.y() > bounds.height() - EDGE_THRESHOLD) {
                    tp.isEdge = true;
                    tp.edge = BottomEdge;
                }

                m_activeTouches[id] = tp;

                if (m_activeTouches.size() == 1) {
                    m_longPressTimerId = startTimer(500);
                    m_longPressPos = tp.startPos;
                }
            } else if (point.state() == QEventPoint::Updated) {
                if (m_activeTouches.contains(id)) {
                    m_activeTouches[id].currentPos = point.position();

                    if (m_longPressTimerId &&
                        (m_activeTouches[id].currentPos - m_longPressPos).manhattanLength() > 20) {
                        killTimer(m_longPressTimerId);
                        m_longPressTimerId = 0;
                    }

                    if (m_activeTouches[id].isEdge) {
                        qreal progress = 0;
                        switch (m_activeTouches[id].edge) {
                        case LeftEdge:
                            progress = m_activeTouches[id].currentPos.x() / (width() * 0.4);
                            break;
                        case BottomEdge:
                            progress = (height() - m_activeTouches[id].currentPos.y()) / (height() * 0.4);
                            break;
                        default: break;
                        }
                        emit edgeSwipe(m_activeTouches[id].edge, qBound(0.0, progress, 1.0));
                    }
                }
            }
        }
        break;
    }
    case QEvent::TouchEnd: {
        const auto &points = event->points();
        for (const auto &point : points) {
            int id = point.id();
            if (!m_activeTouches.contains(id)) continue;

            auto &tp = m_activeTouches[id];
            QPointF delta = point.position() - tp.startPos;
            qint64 duration = QDateTime::currentMSecsSinceEpoch() - tp.startTime;
            qreal velocity = duration > 0 ? delta.manhattanLength() / (duration / 1000.0) : 0;

            if (m_longPressTimerId) {
                killTimer(m_longPressTimerId);
                m_longPressTimerId = 0;
            }

            if (velocity > SWIPE_VELOCITY_THRESHOLD) {
                if (qAbs(delta.x()) > qAbs(delta.y())) {
                    if (delta.x() > 0) emit swipeRight(velocity);
                    else emit swipeLeft(velocity);
                } else {
                    if (delta.y() > 0) emit swipeDown(velocity);
                    else emit swipeUp(velocity);
                }
            }

            m_activeTouches.remove(id);
        }
        break;
    }
    default:
        break;
    }
    event->accept();
}

void GestureHandler::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_longPressTimerId) {
        killTimer(m_longPressTimerId);
        m_longPressTimerId = 0;
        emit longPress(m_longPressPos);
    }
}
