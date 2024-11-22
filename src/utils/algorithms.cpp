#include "algorithms.h"

namespace {

template <typename T>
int findFirst(QList<T>& l, T elem) {
    auto it = std::find(l.begin(), l.end(), elem);
    return (it != l.end()) ? (it - l.begin()) : (-1);
}

}

void testFind() {
    QList<int> l{1, 2, 3, 4, 5, 6, 7};
    assert(findFirst(l, 10) == -1);
    assert(findFirst(l, 5) == 4);
}

namespace Algorithms {

SlResult scanlineAlgo(const SlEvents& data) {
    // Greedy algo that assigns each slot to most left available column,
    // ensuring all slots are placed at parallel collumns when intersect each other
    // returns: Pair for each slot (assigned column, count of parallel slots)

    assert(data.size() % 2 == 0);
    QList<QList<int>> result(data.size() / 2);

    QList<int> columns(data.size(), -1); // Keep track of currently taken columns
    QList<int> curChunk; // Keep track of currently started slots;

    for (int i = 0; i < data.size(); i++) {
        int curSlot = data[i].id;

        if (data[i].isStart) {
            result[curSlot] = {0, 1};
            auto firstFreeCol = findFirst(columns, -1);

            // Assign to free column
            result[curSlot][0] = firstFreeCol;
            columns[firstFreeCol] = data[i].id;

            // Increase count for each currently open slots
            curChunk.append(curSlot);
            std::for_each(curChunk.begin(), curChunk.end(), [&result, &curChunk](int slotId){
                result[slotId][1] = curChunk.size();
            });
        } else {
            // Free vacant place
            auto pos = result[curSlot][0];
            columns[pos] = -1;

            curChunk.removeOne(curSlot);
        }
    }

    return result;
}

void printSlEvents(const SlEvents& data) {
    for (int i = 0; i < data.size(); i++) {
        qDebug() << "Elem: " << data[i].x << ", " << (data[i].isStart ? "<" : ">") << ", " << data[i].id;
    }
}

void printSlResult(const SlResult& res) {
    for (int i = 0; i < res.size(); i++) {
        qDebug() << "Chunk: " << res[i];
    }
}

}
