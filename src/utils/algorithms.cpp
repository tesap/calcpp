#include "algorithms.h"

namespace Algorithms {

ScanlineResult scanlineAlgo(const ScanlineData& data) {
    ScanlineResult result(data.size());
    int curChunk = 0;
    int curOpenAtChunk = 0;

    for (int i = 0; i < data.size(); i++) {
        if (data[i].isStart) {
            curOpenAtChunk++;
            result[curChunk].append(data[i].relatedIndex);
        } else {
            if (curOpenAtChunk == 1) {
                curChunk++;
            }
            curOpenAtChunk--;
        }
    }

    result.resize(curChunk);

    return result;
}

void printScanlineData(const ScanlineData& data) {
    for (int i = 0; i < data.size(); i++) {
        qDebug() << "Elem: " << data[i].x << ", " << (data[i].isStart ? "<" : ">") << ", " << data[i].relatedIndex;
    }
}

void printScanlineResult(const ScanlineResult& res) {
    for (int i = 0; i < res.size(); i++) {
        qDebug() << "Chunk: " << res[i];
    }
}

}
