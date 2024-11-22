#include "algorithms.h"

// namespace Algorithms {

// template <template <typename T> class Container>
// void elementsToScanlineData(Container<CalendarRect> container);


int testSimple() {
    Algorithms::ScanlineData data = {
                         {0, true, 0},
                         {1, false, 0},

                         {5, true, 1},
                         {6, false, 1},

                         {7, true, 2},
                         {8, false, 2},

                         {9, true, 3},

                         {10, true, 4},
                         {11, false, 4},

                         {12, false, 3},

                         {13, true, 5},
                         {14, true, 6},
                         {15, false, 5},
                         {16, true, 7},
                         {17, false, 6},
                         {18, false, 7},

                         };

    printScanlineData(data);
    auto res = scanlineAlgo(data);

    Algorithms::printScanlineResult(res);

    assert(res.size() == 5);

    return 0;
}

void testConversion() {
    QList<CalendarRect> list = {
        {0, 1},
        {1, 1},
        {5, 5}, // 5 - 10
        {5, 1}, // 5 - 6
        {6, 1}, // 6 - 7
        {7, 5}, // 7 - 12
        {11, 4}, // 11 - 15
        {20, 1}, // 20 - 21
    };
    auto data = Algorithms::elementsToScanlineData(list);
    Algorithms::printScanlineData(data);

    auto result = Algorithms::scanlineAlgo(data);
    Algorithms::printScanlineResult(result);

    assert(result.size() == 4);
}

int main() {
    qDebug() << "===================";
    testSimple();
    qDebug() << "===================";
    testConversion();
    qDebug() << "===================";
}

// }
