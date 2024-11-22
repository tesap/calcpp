#include "algorithms.h"

namespace Algorithms {

template <template <typename T> class Container, typename T>
ScanlineData rectsToScanlineInput(Container<T>& container) {
    ScanlineData result;

    for (int i = 0; i < container.size(); i++) {
        result.append({container[i].start, true, i});
        result.append({container[i].start + container[i].duration, false, i});
    }

    std::sort(result.begin(), result.end());
    return result;
}

}
