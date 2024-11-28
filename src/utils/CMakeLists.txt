add_executable(utils
        algorithms.h
        algorithms.cpp
        algorithms.tpp
        algorithms-tests.cpp
        ../old/calendar/tasks.h
        ../old/calendar/tasks.cpp
)
target_link_libraries(utils PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set(UTILS_SOURCES
    src/utils/algorithms.h
    src/utils/algorithms.cpp
    src/utils/algorithms.tpp
)

target_sources(${PROJECT_NAME} PRIVATE ${UTILS_SOURCES})

