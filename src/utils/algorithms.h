#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "calendar/tasks.h"

#include <QDebug>

namespace Algorithms {

struct ScanlineItem {
    float x;
    bool isStart;
    float relatedIndex;

    bool operator < (const ScanlineItem& i) const {
        return (x < i.x);
    }
};

using ScanlineData = QList<ScanlineItem>;
using ScanlineResult = QList<QList<float>>;

/*
 * Requirements on T:
 *  t.start
 *  t.duration
 */
template <template <typename T> class Container, typename T>
ScanlineData rectsToScanlineInput(Container<T>& container);

// Get chunks of chained items
ScanlineResult scanlineAlgo(const ScanlineData& data);

void printScanlineData(const ScanlineData& data);
void printScanlineResult(const ScanlineResult& res);

}

#include "algorithms.tpp"

#endif // ALGORITHMS_H
