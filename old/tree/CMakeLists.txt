set(TREE_SOURCES
    src/tree/TreeItem.h
    src/tree/TreeItem.cpp
    src/tree/TreeModel.h
    src/tree/TreeModel.cpp
    src/tree/TreeView.h
    src/tree/TreeView.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${TREE_SOURCES})
