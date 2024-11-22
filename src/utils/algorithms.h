#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "calendar/tasks.h"

#include <QDebug>

namespace Algorithms {

struct ScanlineEvent {
    float x;
    bool isStart;
    int id;

    bool operator<(const ScanlineEvent& i) const { return (x < i.x); }
};

using SlEvents = QList<ScanlineEvent>;
using SlResult = QList<QList<int>>;

/*
 * Requirements on T:
 *  t.start
 *  t.duration
 */
template <template <typename T> class Container, typename T>
SlEvents calRectsToSlEvents(Container<T>& container);

// Get chunks of chained items
SlResult scanlineAlgo(const SlEvents& data);

void printSlEvents(const SlEvents& data);
void printSlResult(const SlResult& res);

} // namespace Algorithms

#include "algorithms.tpp"

#endif // ALGORITHMS_H
