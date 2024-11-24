#include "SingleDayCalendar.h"
#include "calendar/CalendarView.h"
#include "utils/algorithms.h"

SingleDayCalendar::SingleDayCalendar(int startHour, int endHour, int hourHeight, int dayWidth) // , int edgeEventMargin)
    : m_startHour(startHour)
    , m_endHour(endHour)
    , m_hourHeight(hourHeight)
    , m_dayWidth(dayWidth)
    , m_minDeltaSegmentSize(0.5)
    , m_edgeEventMargin(12)
    , m_eventWidth(m_dayWidth)
{

}

bool SingleDayCalendar::addTask(Task t) {
    if (!isRectDrawable(t.start, t.duration)) {
        qDebug() << "Error: To be added task is not drawable";
        return false;
    }

    m_tasks.append(t);
    updateRects();
    return true;
}

bool SingleDayCalendar::isRectDrawable(float startHour, float duration) const {
    return startHour >= m_startHour && (startHour + duration) <= m_endHour;
}

void SingleDayCalendar::draw(QPainter& painter, QPoint& offset)
{
    for (Task& t : m_tasks) {
        t.draw(painter, offset);
    }
}

// TODO Update for each type (or for general?)
void SingleDayCalendar::updateRect(CalendarRect& cr, int colPos, int colsWidth, int cols) {
    // ensureOwnership(cr);

    QRect newRect = calcDrawRect(cr, colPos, cols);
    cr.setRect(newRect);
}

void SingleDayCalendar::updateRects()
{
    updateTasksRects();
    // updateZonesRects();
}

bool SingleDayCalendar::updateDraggingRect(DragMode mode, CalendarRect &cr, int deltaY) {
    ensureOwnership(cr);

    int deltaSegments = deltaY / (m_hourHeight * m_minDeltaSegmentSize);
    float deltaHours = deltaSegments * m_minDeltaSegmentSize;

    switch (mode) {
    case (DragMode::Move): {
        qDebug() << "Delta: " << deltaY << ", " << deltaSegments << ", " << deltaHours;

        float newStartHour = cr.start + deltaHours;
        if (deltaHours != 0 && isRectDrawable(newStartHour, cr.duration)) {
            cr.start = newStartHour;
            // m_lastMousePos = event->pos();

            updateRect(cr, 0, 1, 1);
            return true;
        }
        break;
    }
    case (DragMode::ResizeBottom): {
        float newDuration = cr.duration + deltaHours;
        // clang-format off
        if (deltaHours != 0
            && newDuration >= m_minDeltaSegmentSize
            && isRectDrawable(cr.start, newDuration)
            ) {
            // clang-format on
            cr.duration = newDuration;

            updateRect(cr, 0, 1, 1);
            return true;
        }
        break;
    }
    case (DragMode::None):
    default: {}
    };

    return false;
}


bool SingleDayCalendar::isIntersectBorder(const CalendarRect& cr, const QPoint& pos) const {
    QRect rect = cr.getRect();

    QRect topEdge = rect.adjusted(0, 0, 0, m_edgeEventMargin - rect.height());
    QRect bottomEdge = rect.adjusted(0, rect.height() - m_edgeEventMargin, 0, 0);

    // return topEdge.contains(pos) || bottomEdge.contains(pos);
    return bottomEdge.contains(pos);
}

bool SingleDayCalendar::isIntersectBody(const CalendarRect& cr, const QPoint& pos) const {
    return cr.getRect().contains(pos);
}

// Agnostic of global position
CoverData SingleDayCalendar::coverDataAt(const QPoint& pos) {
    CoverType result = CoverType::None;
    CalendarRect* rect{nullptr};

    // TODO Sort by max startHour
    std::any_of(m_tasks.begin(), m_tasks.end(), [this, &pos, &result, &rect](Task& t) {
        if (isIntersectBorder(t, pos)) {
            result = CoverType::TaskBorder;
            rect = &t;
            return true;
        } else if (isIntersectBody(t, pos)) {
            result = CoverType::TaskRect;
            rect = &t;
            return true;
        }
        return false;
    });

    return {result, rect};
}


void SingleDayCalendar::updateTasksRects() {
    // 1. Scanline
    // 2. Iterate over result, update each m_rect
    // 3. update()

    auto slInput = Algorithms::calRectsToSlEvents(m_tasks);
    auto slResult = Algorithms::scanlineAlgo(slInput);

    for (int i = 0; i < slResult.size(); i++) {
        // for (int i = 0; i < chunk.size(); i++) {
        //     auto taskIndex = chunk[i];
        //     auto chunkSize = chunk.size();
        //     Task& t = m_tasks[taskIndex];
        //     t.setRect(calcDrawRect(t, i, chunkSize));
        // }
        auto chunk = slResult[i];
        auto taskPos = chunk[0];
        auto taskWidth = chunk[1];
        auto chunkSize = chunk[2];

        Task& t = m_tasks[i];
        t.setRect(calcDrawRect(t, taskPos, taskWidth, chunkSize));
    }

    // update(); // Repainting full widget
}

void SingleDayCalendar::ensureOwnership(CalendarRect &cr) const
{
    if (Task* t = dynamic_cast<Task*>(&cr)) {
        assert(m_tasks.contains(*t));
    } else {
        // .. zones, etc
    }
}

// QRect SingleDayCalendar::calcDrawRect(const CalendarRect& cr, int clusterIndex, int clusterSize) const {
QRect SingleDayCalendar::calcDrawRect(const CalendarRect& cr, int clusterIndex, int widthInCluster, int clusterSize) const {
        float hourRelative = cr.start - m_startHour;


        int colWidth = m_eventWidth / clusterSize;

        int width = widthInCluster * colWidth;
        int height = cr.duration * m_hourHeight;

        int x = clusterIndex * colWidth;
        int y = hourRelative * m_hourHeight;
        return QRect(x, y, width, height);
    }

