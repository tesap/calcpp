#include "CalendarView.h"
#include <QPainter>
#include <QMouseEvent>

CalendarView::CalendarView(QWidget *parent)
    : QWidget(parent), hourHeight(50), calendarWidth(400) {
    setMinimumSize(calendarWidth, hourHeight * 24); // 24-hour calendar
}

void CalendarView::addTask(const Task &task) {
    tasks.append(task);
    update(); // Trigger repaint
}

void CalendarView::clearTasks() {
    tasks.clear();
    update();
}

void CalendarView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Draw background
    painter.fillRect(rect(), Qt::white);

    // Draw time slots (24 hours)
    for (int i = 0; i < 12; ++i) {
        int y = i * hourHeight;
        QRect slotRect(0, y, calendarWidth, hourHeight);
        painter.setPen(Qt::lightGray);
        painter.drawRect(slotRect);

        // Draw time labels
        painter.setPen(Qt::black);
        painter.drawText(10, y + 20, QString("%1:00").arg(i, 2, 10, QChar('0')));
    }

    // Draw tasks
    for (const Task &task : tasks) {
        QRect taskRect = getTaskRect(task);
        painter.setBrush(task.color);
        painter.setPen(Qt::black);
        painter.drawRect(taskRect);

        // Draw task name
        painter.setPen(Qt::white);
        painter.drawText(taskRect, Qt::AlignCenter, task.name);
    }
}

void CalendarView::mousePressEvent(QMouseEvent *event) {
    // Check if a task was clicked
    for (const Task &task : tasks) {
        QRect taskRect = getTaskRect(task);
        if (taskRect.contains(event->pos())) {
            qDebug() << "Clicked on task:" << task.name;
            return;
        }
    }
}

QRect CalendarView::getTaskRect(const Task &task) const {
    int y = task.startHour * hourHeight;
    int height = task.duration * hourHeight;
    return QRect(50, y, calendarWidth - 100, height); // Indented task rect
}

