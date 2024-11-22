#include "CalendarView.h"
#include <QPainter>
#include <QMouseEvent>
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
    , m_eventXPadding(50)
    , m_calendarWidth(m_eventWidth + m_eventXPadding + 25)
    , m_calendarHeight(m_hourHeight * (m_endHour - m_startHour) + 25)
    , minDeltaSegmentSize(0.5)
    , m_edgeEventMargin(12)
{

    setMinimumSize(
        m_calendarWidth,
        m_calendarHeight);
    setMouseTracking(true);

    m_tasks.append({"QStr", 10, 2.5, QColor::fromHsv(200, 255, 200)});
}

bool CalendarView::addTask(Task task) {
    if (!taskDrawable(task.startHour, task.duration)) {
        qDebug() << "Error: To be added task is not drawable";
        return false;
    }

    m_tasks.append(task);
    update();
    return true;
}

void CalendarView::clearTasks() {
    m_tasks.clear();
    update();
}

void CalendarView::paintEvent(QPaintEvent *event) {
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

    for (const Task &task : m_tasks) {
        QRect taskRect = getTaskRect(task);
        painter.setBrush(task.color);
        painter.setPen(Qt::black);
        painter.drawRect(taskRect); //, 10, 5, Qt::RelativeSize);

        painter.setPen(Qt::white);
        painter.drawText(taskRect, Qt::AlignCenter, task.name);
    }
}

void CalendarView::mousePressEvent(QMouseEvent *event) {
    m_dragMode = None;
    m_draggedTask = nullptr;

    std::any_of(m_tasks.begin(), m_tasks.end(), [this, event](Task& t){
        if (isCursorCoversBorder(t, event->pos())) {
            m_dragMode = ResizeBottom;
            m_draggedTask = &t;
            m_lastMousePos = event->pos();
            qDebug() << "Started resizing:" << t.name;
            return true;
        } else if (isCursorCoversBody(t, event->pos())) {
            m_dragMode = Move;
            m_draggedTask = &t;
            m_lastMousePos = event->pos();
            qDebug() << "Started dragging:" << t.name;
            return true;
        }
        return false;
    });
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

        float newStartHour = m_draggedTask->startHour + deltaHours;
        if (deltaHours != 0 && taskDrawable(newStartHour, m_draggedTask->duration)) {
            m_draggedTask->startHour = newStartHour;
            m_lastMousePos = event->pos();
            update();
        }
        break;
    }
    case (ResizeBottom): {
        float newDuration = m_draggedTask->duration + deltaHours;
        if (deltaHours != 0
            && newDuration >= minDeltaSegmentSize
            && taskDrawable(m_draggedTask->startHour, newDuration)
        ) {
            m_draggedTask->duration = newDuration;
            m_lastMousePos = event->pos();
            update();
        }
        break;
    }
    default: {}
    };
}

void CalendarView::mouseReleaseEvent(QMouseEvent *event) {
    m_dragMode = None;
    m_draggedTask = nullptr;

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
        auto isCovering = std::any_of(m_tasks.begin(), m_tasks.end(), [this, event](const Task& t){
            if (isCursorCoversBorder(t, event->pos())) {
                setCursor(Qt::SizeVerCursor);
                return true;
            } else if (isCursorCoversBody(t, event->pos())) {
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

bool CalendarView::isCursorCoversBorder(const Task& t, const QPoint& pos) {
    QRect taskRect = getTaskRect(t);

    QRect topEdge = taskRect.adjusted(0, 0, 0, m_edgeEventMargin - taskRect.height());
    QRect bottomEdge = taskRect.adjusted(0, taskRect.height() - m_edgeEventMargin / 2, 0, m_edgeEventMargin / 2);

    // return topEdge.contains(pos) || bottomEdge.contains(pos);
    return bottomEdge.contains(pos);
}

bool CalendarView::isCursorCoversBody(const Task& t, const QPoint& pos) {
    QRect taskRect = getTaskRect(t);
    return taskRect.contains(pos);
}

bool CalendarView::taskDrawable(float startHour, float duration) const {
    return startHour >= m_startHour && (startHour + duration) <= m_endHour;
}

QRect CalendarView::getTaskRect(const Task &task) const {
    float hourRelative = task.startHour - m_startHour;

    int y = hourRelative * m_hourHeight;
    int height = task.duration * m_hourHeight;
    return QRect(m_eventXPadding, y, m_eventWidth, height);
}

