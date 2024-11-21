#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QWidget>
#include <QList>
#include <QRect>
#include <QString>

// Task structure to represent a task in the calendar
struct Task {
    QString name;
    int startHour;  // Start time (hour)
    int duration;   // Duration in hours
    QColor color;   // Priority-based color
};

class CalendarView : public QWidget {
    Q_OBJECT

public:
    explicit CalendarView(QWidget *parent = nullptr);
    void addTask(const Task &task); // Add a new task
    void clearTasks();             // Clear all tasks

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QList<Task> tasks; // List of tasks
    int hourHeight;    // Height of one hour slot
    int calendarWidth; // Width of the calendar
    QRect getTaskRect(const Task &task) const; // Get rectangle for a task
};

#endif // CALENDARVIEW_H

