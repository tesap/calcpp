#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QWidget>
#include <QList>
#include <QRect>
#include <QString>


struct Task {
    QString name;
    float startHour;
    float duration;
    QColor color;
};

class CalendarView : public QWidget {
    Q_OBJECT

public:

    explicit CalendarView(QWidget *parent = nullptr);
    bool addTask(Task task);
    void clearTasks();

protected:
    virtual QSize sizeHint() const override;
    virtual void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool taskDrawable(float startHour, float duration) const;
    QRect getTaskRect(const Task &task) const;

    bool m_isDragging{false};
    Task* m_draggedTask{nullptr};
    QPoint m_lastMousePos;

    QList<Task> m_tasks;

    QColor m_bgColor;
    int m_startHour;
    int m_endHour;

    int m_hourHeight;
    int m_hourXPadding;
    int m_hourYPadding;

    int m_eventWidth;
    int m_eventXPadding;

    int m_calendarWidth;
    int m_calendarHeight;
};

#endif // CALENDARVIEW_H

