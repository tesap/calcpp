#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include "tasks.h"

#include <QList>
#include <QRect>
#include <QString>
#include <QWidget>

class CalendarView : public QWidget {
    Q_OBJECT

public:
    explicit CalendarView(QWidget* parent = nullptr);
    bool addTask(Task task);
    // bool addZone(Task task);
    void clearTasks();

protected:
    virtual QSize sizeHint() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void adjustCursor(QMouseEvent* event);
    // void adjustTasksRects();
    void updateTasksRects();
    void updateDraggingTaskRect();

    bool taskDrawable(float startHour, float duration) const;
    QRect calcDrawRect(const CalendarRect& cr, int clusterIndex, int widthInCluster, int clusterSize) const;
    bool isIntersectBorder(const CalendarRect& cr, const QPoint& pos) const;
    bool isIntersectBody(const CalendarRect& cr, const QPoint& pos) const;

    QPoint m_lastMousePos;

    enum DragMode { None, Move, ResizeBottom };
    DragMode m_dragMode{None};
    CalendarRect* m_draggedRect{nullptr};

    QList<Task> m_tasks;
    QList<Zone> m_zones;

    QColor m_bgColor;
    int m_startHour;
    int m_endHour;

    int m_hourHeight;
    int m_hourXPadding;
    int m_hourYPadding;

    int m_eventWidth;
    int m_eventLeftPadding;
    int m_eventRightPadding;

    int m_calendarWidth;
    int m_calendarHeight;

    float minDeltaSegmentSize;
    int m_edgeEventMargin;
};

#endif // CALENDARVIEW_H
