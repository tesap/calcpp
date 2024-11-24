#include "CalendarView.h"

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QString>

namespace {

constexpr auto WHITE = Qt::white;
constexpr auto BLACK = Qt::black;
constexpr auto LIGHT_GRAY = Qt::lightGray;

// Draws text below base position, not above
void drawTextBelow(QPainter& painter, const QString& text, const QColor& color, QPoint&& pos) {
    painter.setPen(color);

    QFontMetrics fm{painter.font()};
    int textHeight = fm.height();
    pos.setY(pos.y() + textHeight);

    painter.drawText(pos, text);
}

void drawDashedLine(QPainter& painter, const QColor& color, const QLine& line) {
    QPen pen = painter.pen();
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.drawLine(line);
}

} // namespace

CalendarView::CalendarView(QWidget* parent)
    : QWidget(parent)
    , m_daysCnt(3)
    , m_bgColor(WHITE)
    , m_startHour(5)
    , m_endHour(23)
    , m_dayWidth(350)
    , m_hourHeight(50)
    , m_hourXPadding(5)
    , m_hourYPadding(0)
    , m_leftPadding(50)
    , m_rightPadding(50)
    , m_bottomPadding(25)
    , m_calendarWidth(m_dayWidth * m_daysCnt + m_leftPadding + m_rightPadding)
    , m_calendarHeight(m_hourHeight * (m_endHour - m_startHour) + m_bottomPadding)
    // , m_hourHeight(50)

    // , m_eventLeftPadding(50)
    // , m_eventRightPadding(50)
    // , m_edgeEventMargin(12)
{
    setMinimumSize(m_calendarWidth, m_calendarHeight);
    setMouseTracking(true);

    initDaysViews(m_daysCnt);

    addTask({10, 2.5, "QStr"}, 0);
    addTask({10, 2.5, "QStr"}, 1);
    addTask({10, 2.5, "QStr"}, 2);
    // addTask({10, 2.5, "QStr"}, 3);
    // addTask({10, 2.5, "QStr"}, 4);
}

// // TODO update on this level or at SingleDayCalendar? (Or both?)
// void CalendarView::updateDraggingTaskRect() {
//     // Update task's rect indefferently to other tasks, as it
//     // is being in process of dragging

//     update();
// }

bool CalendarView::addTask(Task t, int dayInd) {
    SingleDayCalendar& day = m_days[dayInd];
    auto result = day.addTask(t);
    updateDay(dayInd);

    return result;
}

void CalendarView::initDaysViews(int daysCnt)
{
    m_days.clear();
    for (int i = 0; i < daysCnt; i++) {
        // clang-format on
        SingleDayCalendar day{
            m_startHour,
            m_endHour,
            m_hourHeight,
            m_dayWidth
        };
        // clang-format off
        m_days.append(day);
    }
    update();
}

void CalendarView::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(rect(), m_bgColor);

    int hoursCnt = m_endHour - m_startHour;
    for (int i = 0; i <= hoursCnt; ++i) { // [start;end]
        int y = i * m_hourHeight;
        drawDashedLine(painter, LIGHT_GRAY, {0, y, m_calendarWidth, y});

        int curHour = i + m_startHour;
        QString text = QString("%1:00").arg(curHour, 2, 10, QChar('0'));
        drawTextBelow(painter, text, BLACK, {m_hourXPadding, y + m_hourYPadding});
    }

    for (int i = 0; i < m_daysCnt; i++) {
        auto& day = m_days[i];
        QRect dayRect = getDayRect(i);
        // if (event->rect().intersects(dayRect)) {
            // TODO Redraw partially
            QPoint offset = dayRect.topLeft();
            day.draw(painter, offset);
        // }
    }

    // Dragging rect is to be drawn at top
    if (m_draggedRect) {
        QPoint offset = getDayRect(m_draggedDay).topLeft();
        m_draggedRect->draw(painter, offset);
    }
}

void CalendarView::mousePressEvent(QMouseEvent* event) {
    m_dragMode = DragMode::None;
    m_draggedRect = nullptr;

    QPoint globalPos = event->pos();
    int dayIndex = getDayIndexBy(globalPos);
    SingleDayCalendar& day = m_days[dayIndex];
    QPoint relativePos = globalPos - getDayRect(dayIndex).topLeft();
    CoverData cd = day.coverDataAt(relativePos);

    switch (event->button()) {
    case (Qt::RightButton): {
        // Add context menu in corresponding day
        QMenu menu(this);

        switch (cd.type) {
        case (TaskBorder):
        case (TaskRect): {
            menu.addAction("Edit Task", []() {});   // editTask(t); });
            menu.addAction("Delete Task", []() {}); //  { deleteTask(t); });
            break;
        }
        case (ZoneRect): {}
        case (CoverType::None):
        default: {
            menu.addAction("Add Task", [this, &globalPos, &dayIndex]() {
                float startHour = m_startHour + static_cast<float>(globalPos.y()) / m_hourHeight;
                Task newTask{startHour, 1.0, "New Task"}; // Default task
                addTask(newTask, dayIndex);
            });
        }
        }

        menu.exec(event->globalPos());
        break;
    }
    case (Qt::LeftButton): {
        // Start of action with Task in corresponding day
        // Make Rect full-wide
        switch (cd.type) {
        case (TaskBorder):
            m_dragMode = DragMode::ResizeBottom;
            m_draggedRect = cd.rect;
            m_lastMousePos = event->pos();
            m_draggedDay = dayIndex;
            // Change rect view
            day.updateDraggingRect(m_dragMode, *cd.rect, 0);
            qDebug() << "Started resizing: " << cd.rect->name();
            break;
        case (TaskRect): {
            m_dragMode = DragMode::Move;
            m_draggedRect = cd.rect;
            m_lastMousePos = event->pos();
            m_draggedDay = dayIndex;
            day.updateDraggingRect(m_dragMode, *cd.rect, 0);
            qDebug() << "Started dragging:" << cd.rect->name();
            break;
        }
        case (ZoneRect): {}
        case (CoverType::None):
        default: {
        }
        }
    }
    default: {
    }
    }
}

void CalendarView::mouseMoveEvent(QMouseEvent* event) {
    adjustCursor(event);

    // TODO Delta by x (Day1 -> Day2)
    // Update Task in action : udpate m_rect, Redraw
    // - Either partial old + new
    // - Now full day / 2days

    // QPoint globalPos = event->pos();
    // int dayIndex = getDayIndexBy(globalPos);
    // SingleDayCalendar& day = m_days[dayIndex];
    // QPoint relativePos = globalPos - getDayRect(dayIndex).topLeft();
    // CoverData cd = day.coverDataAt(relativePos);


    if (true) { // Same day
        // TODO Some lost delta on fast movement?
        int deltaY = event->pos().y() - m_lastMousePos.y();

        SingleDayCalendar& day = m_days[m_draggedDay];
        bool moved = day.updateDraggingRect(m_dragMode, *m_draggedRect, deltaY);

        if (moved) {
            m_lastMousePos = event->pos();
            updateDay(m_draggedDay);
        }
    } else {

    }
}

void CalendarView::mouseReleaseEvent(QMouseEvent* event) {
    // Adjust tasks
    // TODO Adjust partially, i.e. only affected by move (needed?)

    QPoint globalPos = event->pos();
    int dayIndex = getDayIndexBy(globalPos);
    SingleDayCalendar& day = m_days[dayIndex];

    m_dragMode = DragMode::None;
    m_draggedRect = nullptr;
    m_draggedDay = -1;

    adjustCursor(event);

    // Update day rects
    // day.updateDraggingRect(DragMode::None, *m_draggedRect, 0);
    day.updateRects();

    // TODO parital
    update();
}

QSize CalendarView::sizeHint() const {
    return QSize(m_calendarWidth, m_calendarHeight);
}

void CalendarView::adjustCursor(QMouseEvent* event) {
    QPoint globalPos = event->pos();

    switch (m_dragMode) {
    case (DragMode::Move): {
        setCursor(ACTION_MOVING_CURSOR);
        return;
    }
    case (DragMode::ResizeBottom): {
        setCursor(ACTION_RESIZING_CURSOR);
        return;
    }
    case (DragMode::None): {
        int dayIndex = getDayIndexBy(globalPos);
        SingleDayCalendar& day = m_days[dayIndex];
        QPoint relativePos = globalPos - getDayRect(dayIndex).topLeft();

        qDebug() << "IND: " << dayIndex;
        qDebug() << "   TOP_LEFT: " << getDayRect(dayIndex).topLeft();
        qDebug() << "   REL_POS: " << relativePos;

        CoverData cd = day.coverDataAt(relativePos);

        switch (cd.type) {
        case (CoverType::TaskBorder): {
            setCursor(COVER_RESIZABLE_CURSOR);
            break;
        }
        case (CoverType::TaskRect): {
            setCursor(COVER_MOVABLE_CURSOR);
            break;
        }
        case (CoverType::ZoneRect): {
            // TODO ?
            // setCursor(COVER_MOVABLE_CURSOR);
            // break;
        }
        case (CoverType::None):
        default: {
            setCursor(Qt::ArrowCursor);
            break;
        }
        }
        break;
    }
    default: {
        qDebug() << "Unknown drag state?: " << (int) m_dragMode;
    }
    }
}

void CalendarView::updateDay(int index)
{
    QRect rect = getDayRect(index);
    update(rect);
}

int CalendarView::getDayIndexBy(const QPoint& globalPos) const {
    // TODO? return (static_cast<float>(pos.x()) / m_dayWidth);
    return (globalPos.x() - m_leftPadding) / m_dayWidth;
    // return m_days[index];
}

QRect CalendarView::getDayRect(int index) const {
    return {
        m_leftPadding + m_dayWidth * index,
        0,
        m_dayWidth,
        m_calendarHeight
    };
}
