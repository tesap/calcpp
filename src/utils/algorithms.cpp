#include "algorithms.h"

namespace {

template <typename T>
std::pair<int, int> fillFirstSegment(QList<T>& l, T elem, T set, int bound) {
    int first = -1;
    int width = 0;

    for (int i = 0; i < bound; i++) {
        if (l[i] == elem) {
            if (first == -1) {
                first = i;
            }
            width++;
            l[i] = set;
        } else if (width > 0) {
            return {first, width};
        }
    }

    return {first, width};
    // auto it = std::find(l.begin(), l.end(), elem);
    // return (it != l.end()) ? (it - l.begin()) : (-1);
}

} // namespace

void testFind() {
    QList<int> l{1, 2, 3, 4, 5, 6, 7};
    // assert(findFirstSegment(l, 10) == {-1, 0});
    // assert(findFirstSegment(l, 5) == {4, 1});
}

namespace Algorithms {

void scanlineIter(const SlEvents& data, SlResult& result, bool reversed) {
    QList<int> columns(data.size(), -1); // Keep track of currently taken columns
    QList<int> curChunk;                 // Keep track of currently started slots;
    int curColsCount = 1;

    for (int ii = 0; ii < data.size(); ii++) {
        // int i = (!reversed) ? (data.size()) : (0);
        // (!reversed) ? (i < data.size()) : (i > 0);
        // (!reversed) ? (i++) : (i--))
        int i = (reversed) ? (data.size() - 1 - ii) : (ii);
        int curSlot = data[i].id;

        qDebug() << "VACANCIES: " << columns;

        // start rev
        // 0 0 -> 0
        // 0 1 -> 1
        // 1 0 -> 1
        // 1 1 -> 0
        bool isStart = (!reversed) ? (data[i].isStart) : (!data[i].isStart);

        if (isStart) {
            // Assign to free columns
            auto [firstFreeCol, freeCols] = fillFirstSegment(columns, -1, curSlot, curColsCount);

            if (freeCols == 0) {
                // Taking new col
                curColsCount++;
                if (curColsCount > result[curSlot][2]) {
                    int pos = curColsCount - 1;
                    int width = 1;
                    pos = (!reversed) ? (pos) : (curColsCount - pos - width); // Rev case
                    result[curSlot] = {pos, width, curColsCount};
                }
                columns[curColsCount - 1] = curSlot;
            } else {
                // Filling existing cols
                if (curColsCount > result[curSlot][2]) {
                    int pos = firstFreeCol;
                    int width = freeCols;
                    pos = (!reversed) ? (pos) : (curColsCount - pos - width); // Rev case
                    result[curSlot] = {pos, width, curColsCount};
                }
            }

            // Increase count for each currently open slots
            curChunk.append(curSlot);
            std::for_each(curChunk.begin(), curChunk.end(), [&result, &curChunk, &curColsCount](int slotId) {
                auto m1 = qMax(result[slotId][2], curChunk.size());
                auto m2 = qMax(m1, curColsCount);
                if (m2 > result[slotId][2]) {
                    result[slotId][2] = m2;
                }
            });
        } else {
            // Free vacant place
            // auto pos = result[curSlot][0];

            auto [firstFreeCol, freeCols] = fillFirstSegment(columns, curSlot, -1, curColsCount);

            // if (firstFreeCol == (curColsCount - 1)) {
                // curColsCount--;
            // }

            // Set cols count to max(current open slots)
            std::for_each(curChunk.begin(), curChunk.end(), [&curColsCount, &result](int el){
                int elCols = result[el][2];
                curColsCount = qMax(curColsCount, elCols);
            });

            // columns[pos] = -1;

            curChunk.removeOne(curSlot);
        }
    }
}

SlResult scanlineAlgo(const SlEvents& data) {
    // Greedy algo that assigns each slot to most left available column,
    // ensuring all slots are placed at parallel collumns when intersect each other
    // returns: Pair for each slot (col pos, width, count of parallel slots)

    assert(data.size() % 2 == 0);
    QList<QList<int>> result;

    for (int i = 0; i < (data.size() / 2); i++) {
        result.append({0, 1, 1});
    }

    scanlineIter(data, result, false);
    printSlResult(result);

    // scanlineIter(data, result, true);
    // printSlResult(result);

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

} // namespace Algorithms
