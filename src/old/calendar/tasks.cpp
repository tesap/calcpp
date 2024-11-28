#include "tasks.h"

CalendarRect::CalendarRect(float start, float duration, QString name)
    : start{start}
    , duration{duration}
    , m_name{name}
    , m_bgColor{QColor::fromHsv(200, 255, 200)} {}

bool CalendarRect::isOverlap(const CalendarRect& b) {
    return !(start + duration <= b.start || b.start + b.duration <= start);
}

void CalendarRect::draw(QPainter& painter, QPoint& offset) {
    painter.setBrush(m_bgColor);

    QRect absRect = m_rect.translated(offset);

    painter.setPen(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(absRect, 8, 8);
    painter.setPen(Qt::white);
    painter.drawText(absRect, Qt::AlignCenter, m_name);
}

QRect CalendarRect::getRect() const {
    return m_rect;
}

void CalendarRect::setRect(const QRect& newRect) {
    m_rect = newRect;
}

QColor CalendarRect::bgColor() const {
    return m_bgColor;
}

void CalendarRect::setBgColor(const QColor& newBgColor) {
    m_bgColor = newBgColor;
}

QString CalendarRect::name() const {
    return m_name;
}

void CalendarRect::setName(const QString& newName) {
    m_name = newName;
}

bool CalendarRect::operator<(const CalendarRect& b) const {
    return (start < b.start);
}

Task::Task(float start, float duration, QString name)
    : CalendarRect(start, duration, name) {}
