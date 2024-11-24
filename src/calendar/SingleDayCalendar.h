#ifndef SINGLEDAYCALENDAR_H
#define SINGLEDAYCALENDAR_H

#include "tasks.h"

enum CoverType { None, TaskRect, TaskBorder, ZoneRect };
struct CoverData {
    CoverType type;
    CalendarRect* rect;
};

enum class DragMode { None, Move, ResizeBottom };

class SingleDayCalendar {
public:
    SingleDayCalendar(int startHour, int endHour, int hourHeight, int dayWidth);

    bool addTask(Task t);
    CoverData coverDataAt(const QPoint& pos);
    void draw(QPainter& painter, QPoint& offset);
    void updateRects();
    bool updateDraggingRect(DragMode mode, CalendarRect& cr, int deltaY);

    bool isRectDrawable(float startHour, float duration) const;

private:
    void updateRect(CalendarRect& cr, int colPos = 0, int colsWidth = 1, int cols = 1);
    void updateTasksRects();

    void ensureOwnership(CalendarRect& cr) const;

    QRect calcDrawRect(const CalendarRect& cr, int clusterIndex = 0, int widthInCluster = 1, int clusterSize = 1) const;

    bool isIntersectBorder(const CalendarRect& cr, const QPoint& pos) const;
    bool isIntersectBody(const CalendarRect& cr, const QPoint& pos) const;


    QList<Task> m_tasks;

    int m_startHour;
    int m_endHour;

    int m_hourHeight;

    int m_dayWidth;

    float m_minDeltaSegmentSize;
    int m_edgeEventMargin;

    // View consts
    int m_eventWidth;
};

#endif // SINGLEDAYCALENDAR_H
