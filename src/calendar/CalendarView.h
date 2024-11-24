#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include "SingleDayCalendar.h"
#include "tasks.h"

#include <QList>
#include <QRect>
#include <QString>
#include <QWidget>

const auto COVER_MOVABLE_CURSOR = Qt::OpenHandCursor;
const auto COVER_NONE_CURSOR = Qt::ArrowCursor;
const auto COVER_RESIZABLE_CURSOR = Qt::SizeVerCursor;
const auto ACTION_MOVING_CURSOR = Qt::ClosedHandCursor;
const auto ACTION_RESIZING_CURSOR = Qt::SizeVerCursor;


class CalendarView : public QWidget {
    Q_OBJECT

public:
    explicit CalendarView(QWidget* parent = nullptr);
    bool addTask(Task t, int day);
    void initDaysViews(int daysCnt);
    // bool addZone(Task task);

protected:
    virtual QSize sizeHint() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void adjustCursor(QMouseEvent* event);
    // SingleDayCalendar &getDayBy(const QPoint &pos);
    void updateDay(int index);

    int getDayIndexBy(const QPoint& globalPos) const;
    QRect getDayRect(int index) const;

    QPoint m_lastMousePos;

    DragMode m_dragMode{DragMode::None};
    CalendarRect* m_draggedRect{nullptr};
    int m_draggedDay;

    QList<SingleDayCalendar> m_days;
    QList<Zone> m_zones;

    int m_daysCnt;
    QColor m_bgColor;

    int m_startHour;
    int m_endHour;

    int m_dayWidth;
    int m_hourHeight;

    int m_hourXPadding;
    int m_hourYPadding;

    // int m_eventWidth;
    int m_leftPadding;
    int m_rightPadding;
    int m_bottomPadding;

    // View consts
    int m_calendarWidth;
    int m_calendarHeight;
};

#endif // CALENDARVIEW_H
