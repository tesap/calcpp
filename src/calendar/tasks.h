#ifndef TASKS_H
#define TASKS_H

#include <QString>
#include <QRect>
#include <QColor>
#include <QPainter>

// class TasksSetComparator;
// struct SetElem;

// class Task;
// using TaskList = QList<Task>;

class CalendarRect {
public:
    CalendarRect(float start, float duration, QString name = nullptr);
    CalendarRect(const CalendarRect &) = default;

    float start;
    float duration;

    virtual bool operator< (const CalendarRect& b) const;
    virtual bool isOverlap(const CalendarRect& b);
    virtual void draw(QPainter& painter);

    // Getters & Setters
    virtual QRect getRect() const;
    virtual void setRect(const QRect &newRect);

    QColor bgColor() const;
    void setBgColor(const QColor &newBgColor);

    QString name() const;
    void setName(const QString &newName);

protected:
    int m_id; // Unique in single day
    QString m_name;
    QRect m_rect;

private:
    QColor m_bgColor;
     // m_styling;

};

class Zone : public CalendarRect {
public:
    Zone(const Zone &) = default;
};

class Task : public CalendarRect {public:
    Task(float start, float duration, QString name);
};

// template <typename T>
// using CRContainer = QList<T>;

// struct Task {
//     QString name;
//     float startHour;
//     float duration;

// };

// struct SetElem {
//     struct Task t;
//     QRect r;
// };

// class TasksSetComparator {
//     bool operator()(SetElem& a, SetElem& b) const {
//         return a.t.startHour < b.t.startHour;
//     }
// };

// using TasksSet = std::set<SetElem, TasksSetComparator>;


#endif // TASKS_H
