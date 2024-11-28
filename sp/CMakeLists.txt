add_executable(sp
        main.cpp
)
target_link_libraries(sp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

