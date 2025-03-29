#include "algorithms.h"

namespace Algorithms {

template <template <typename T> class Container, typename T>
SlEvents calRectsToSlEvents(Container<T>& container) {
    SlEvents result;

    for (int i = 0; i < container.size(); i++) {
        float start = container[i].start;
        float end = start + container[i].duration;
        float len = container[i].duration;
        result.append({start, true, i, len});
        result.append({end, false, i, len});
    }

    std::sort(result.begin(), result.end());
    return result;
}

} // namespace Algorithms
