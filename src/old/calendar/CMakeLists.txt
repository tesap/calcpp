set(CAL_SOURCES
    src/old/calendar/CalendarView.h
    src/old/calendar/CalendarView.cpp
    src/old/calendar/tasks.h
    src/old/calendar/tasks.cpp
    src/old/calendar/SingleDayCalendar.h src/old/calendar/SingleDayCalendar.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${CAL_SOURCES})

