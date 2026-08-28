cmake_minimum_required(VERSION 3.20)
project(MangaDexExporter VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(ZLIB REQUIRED)

qt_add_executable(MangaDexExporter
    main.cpp
    icons.qrc
)

# Windows: embed the .ico into the .exe resource table
if(WIN32)
    target_sources(MangaDexExporter PRIVATE hitpaw.rc)
    set_target_properties(MangaDexExporter PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

target_link_libraries(MangaDexExporter PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    ZLIB::ZLIB
)

# macOS bundle
if(APPLE)
    set_target_properties(MangaDexExporter PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE hitpaw.icns
    )
endif()
