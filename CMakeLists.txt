cmake_minimum_required(VERSION 3.20)
project(ImtSentra VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(SENTRA_BUILD_TESTS "Build tests" ON)
option(SENTRA_BUILD_CLI "Build CLI tool" ON)
option(SENTRA_BUILD_GUI "Build QML GUI" ON)

# Find dependencies via Conan or system
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Network WebSockets)

# ImtCore dependencies (via Conan or IMTCOREDIR)
if(DEFINED ENV{IMTCOREDIR})
    set(IMTCORE_DIR $ENV{IMTCOREDIR})
    list(APPEND CMAKE_PREFIX_PATH ${IMTCORE_DIR})
endif()

# Core library
add_library(imtsentra
    Impl/CScenarioGraphComp.cpp
    Impl/CScenarioExecutorComp.cpp
    Impl/CNodeActionFactory.cpp
    Impl/CPixelDiffComparatorComp.cpp
    Impl/CBaselineManagerComp.cpp
    Impl/CScreenshotStorageComp.cpp
    Impl/CAppMapBuilderComp.cpp
    Impl/CExecutionEngineComp.cpp
)

target_include_directories(imtsentra PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Tools/Browser
    ${CMAKE_CURRENT_SOURCE_DIR}/Tools/AI
)

target_link_libraries(imtsentra PUBLIC
    Qt6::Core
    Qt6::Network
    Qt6::WebSockets
)

# GraphQL API library
add_library(imtsentragql
    Impl/CScenarioServiceControllerComp.cpp
    Impl/CExecutionServiceControllerComp.cpp
)

target_include_directories(imtsentragql PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(imtsentragql PUBLIC
    imtsentra
    Qt6::Core
    Qt6::Network
)

# CLI Tool
if(SENTRA_BUILD_CLI)
    add_executable(sentra-cli
        Tools/CLI/main.cpp
    )
    target_link_libraries(sentra-cli PRIVATE
        imtsentra
        imtsentragql
        Qt6::Core
    )
endif()

# GUI Application
if(SENTRA_BUILD_GUI)
    qt_add_executable(sentra-gui
        Qml/main.cpp
    )
    target_link_libraries(sentra-gui PRIVATE
        imtsentra
        imtsentragql
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
    )
    qt_add_qml_module(sentra-gui
        URI imtsentragui
        VERSION 1.0
        QML_FILES
            Qml/imtsentragui/GraphEditor/GraphEditor.qml
            Qml/imtsentragui/Timeline/ExecutionTimeline.qml
            Qml/imtsentragui/DiffViewer/DiffViewer.qml
            Qml/imtsentragui/AppMap/AppMapViewer.qml
    )
endif()

# Tests
if(SENTRA_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 REQUIRED COMPONENTS Test)

    add_executable(ScenarioEngineTest
        Tests/ScenarioEngineTest/tst_scenario_graph.cpp
    )
    target_link_libraries(ScenarioEngineTest PRIVATE
        imtsentra
        Qt6::Test
    )
    add_test(NAME ScenarioEngineTest COMMAND ScenarioEngineTest)

    add_executable(ImageComparisonTest
        Tests/ImageComparisonTest/tst_image_comparison.cpp
    )
    target_link_libraries(ImageComparisonTest PRIVATE
        imtsentra
        Qt6::Test
    )
    add_test(NAME ImageComparisonTest COMMAND ImageComparisonTest)
endif()
