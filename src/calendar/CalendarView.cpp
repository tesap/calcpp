#include "CalendarView.h"
#include "utils/algorithms.h"

#include <QPainter>
#include <QMouseEvent>
#include <QString>
#include <QMenu>

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

}

CalendarView::CalendarView(QWidget *parent)
    : QWidget(parent)
    , m_bgColor(WHITE)
    , m_startHour(5)
    , m_endHour(23)
    , m_hourHeight(50)
    , m_hourXPadding(5)
    , m_hourYPadding(0)
    , m_eventWidth(350)
    , m_eventLeftPadding(50)
    , m_eventRightPadding(50)
    , m_calendarWidth(m_eventWidth + m_eventLeftPadding + m_eventRightPadding)
    , m_calendarHeight(m_hourHeight * (m_endHour - m_startHour) + 25)
    , minDeltaSegmentSize(0.5)
    , m_edgeEventMargin(12)
{

    setMinimumSize(
        m_calendarWidth,
        m_calendarHeight);
    setMouseTracking(true);

    addTask({10, 2.5, "QStr"});
}

void CalendarView::updateTasksRects() {
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
        auto chunkSize = chunk[1];

        Task& t = m_tasks[i];
        t.setRect(calcDrawRect(t, taskPos, chunkSize));
    }

    update(); // Repainting full widget
}

void CalendarView::updateDraggingTaskRect()
{
    // Update task's rect indefferently to other tasks, as it
    // is being in process of dragging
    m_draggedRect->setRect(calcDrawRect(*m_draggedRect));
    update();
}

bool CalendarView::addTask(Task t) {
    if (!taskDrawable(t.start, t.duration)) {
        qDebug() << "Error: To be added task is not drawable";
        return false;
    }

    m_tasks.append(t);

    updateTasksRects();
    return true;
}

void CalendarView::clearTasks() {
    m_tasks.clear();
    update();
}

void CalendarView::paintEvent(QPaintEvent *event) {

    qDebug() << "RECT: " << event->rect();

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

    for (Task& t: m_tasks) {
        t.draw(painter);
    }

    // Dragging rect is to be drawn at top
    if (m_draggedRect) {
        m_draggedRect->draw(painter);
    }
}

void CalendarView::mousePressEvent(QMouseEvent *event) {
    m_dragMode = None;
    m_draggedRect = nullptr;

    switch (event->button()) {
    case (Qt::RightButton): {
        std::any_of(m_tasks.begin(), m_tasks.end(), [event, this](const Task& t){
            if (isIntersectBody(t, event->pos())) {
                QMenu menu(this);
                menu.addAction("Edit Task", []() { }); // editTask(t); });
                menu.addAction("Delete Task", []() { }); //  { deleteTask(t); });
                menu.exec(event->globalPos());
                return true;
            }
            return false;
        });
        break;
    }
    case (Qt::LeftButton): {
        std::any_of(m_tasks.begin(), m_tasks.end(), [this, event](Task& t){
            if (isIntersectBorder(t, event->pos())) {
                m_dragMode = ResizeBottom;
                m_draggedRect = &t;
                m_lastMousePos = event->pos();
                qDebug() << "Started resizing:" << t.name();
                return true;
            } else if (isIntersectBody(t, event->pos())) {
                m_dragMode = Move;
                m_draggedRect = &t;
                m_lastMousePos = event->pos();
                qDebug() << "Started dragging:" << t.name();
                return true;
            }
            return false;
        });
    }
    default: {}

    }
}

void CalendarView::mouseMoveEvent(QMouseEvent *event) {
    adjustCursor(event);

    // TODO Some lost delta on fast movement?
    int deltaY = event->pos().y() - m_lastMousePos.y();
    int deltaSegments = deltaY / (m_hourHeight * minDeltaSegmentSize);
    float deltaHours = deltaSegments * minDeltaSegmentSize;

    switch (m_dragMode) {
    case (Move): {
        qDebug() << "Delta: " << deltaY << ", " << deltaSegments << ", " << deltaHours;

        float newStartHour = m_draggedRect->start + deltaHours;
        if (deltaHours != 0 && taskDrawable(newStartHour, m_draggedRect->duration)) {
            m_draggedRect->start = newStartHour;
            m_lastMousePos = event->pos();
            updateDraggingTaskRect();
        }
        break;
    }
    case (ResizeBottom): {
        float newDuration = m_draggedRect->duration + deltaHours;
        if (deltaHours != 0
            && newDuration >= minDeltaSegmentSize
            && taskDrawable(m_draggedRect->start, newDuration)
        ) {
            m_draggedRect->duration = newDuration;
            m_lastMousePos = event->pos();
            updateDraggingTaskRect();
        }
        break;
    }
    default: {}
    };
}

void CalendarView::mouseReleaseEvent(QMouseEvent *event) {
    // Adjust tasks
    // TODO Adjust partially (needed?)
    // TODO cast works?
    if (dynamic_cast<Task*>(m_draggedRect)) {
        updateTasksRects();
    }

    m_dragMode = None;
    m_draggedRect = nullptr;

    adjustCursor(event);
}

QSize CalendarView::sizeHint() const {
    return QSize(m_calendarWidth, m_calendarHeight);
}

void CalendarView::adjustCursor(QMouseEvent *event) {
    switch (m_dragMode) {
    case (Move): {
        setCursor(Qt::ClosedHandCursor);
        return;
    }
    case (ResizeBottom): {
        setCursor(Qt::SizeVerCursor);
        return;
    }
    case (None): {
        // TODO Sort by max startHour
        auto isCovering = std::any_of(m_tasks.begin(), m_tasks.end(), [this, event](const Task& t){
            if (isIntersectBorder(t, event->pos())) {
                setCursor(Qt::SizeVerCursor);
                return true;
            } else if (isIntersectBorder(t, event->pos())) {
                setCursor(Qt::OpenHandCursor);
                return true;
            }
            return false;
        });

        if (!isCovering) {
            setCursor(Qt::ArrowCursor);
        }
        break;
    }
    default: {
        qDebug() << "Unknown drag state?: " << m_dragMode;
    }
    }
}

bool CalendarView::isIntersectBorder(const CalendarRect &cr, const QPoint& pos) const {
    QRect rect = cr.getRect();

    QRect topEdge = rect.adjusted(0, 0, 0, m_edgeEventMargin - rect.height());
    QRect bottomEdge = rect.adjusted(0, rect.height() - m_edgeEventMargin / 2, 0, m_edgeEventMargin / 2);

    // return topEdge.contains(pos) || bottomEdge.contains(pos);
    return bottomEdge.contains(pos);
}

bool CalendarView::isIntersectBody(const CalendarRect &cr, const QPoint& pos) const {
    return cr.getRect().contains(pos);
}

bool CalendarView::taskDrawable(float startHour, float duration) const {
    return startHour >= m_startHour && (startHour + duration) <= m_endHour;
}

QRect CalendarView::calcDrawRect(const CalendarRect &cr, int clusterIndex, int clusterSize) const {
    float hourRelative = cr.start - m_startHour;

    int width = m_eventWidth / clusterSize;
    int height = cr.duration * m_hourHeight;

    int x = m_eventLeftPadding + clusterIndex * width;
    int y = hourRelative * m_hourHeight;
    return QRect(x, y, width, height);
}

