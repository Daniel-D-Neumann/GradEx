cmake_minimum_required(VERSION 3.16)

project(ProceduralAudioGenerator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

set(PROJECT_SOURCES
        main.cpp
        pagwindow.cpp
        pagwindow.h
        pagwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProceduralAudioGenerator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Envelopes.cpp Envelopes.h FourierTransformation.cpp FourierTransformation.h Instrument.cpp Instrument.h InstrumentSerialser.cpp InstrumentSerialser.h json.hpp json_fwd.hpp MarkovChain.cpp MarkovChain.h MarkovConsts.h MidiParser.cpp MidiParser.h Oscillator.cpp Oscillator.h SoundGenerator.cpp SoundGenerator.h Timer.cpp Timer.h Vector.cpp Vector.h WriteToWav.cpp WriteToWav.h
        MidiFile/Binasc.cpp MidiFile/Binasc.h MidiFile/MidiEvent.cpp MidiFile/MidiEvent.h MidiFile/MidiEventList.cpp MidiFile/MidiEventList.h MidiFile/MidiFile.cpp MidiFile/MidiFile.h MidiFile/MidiMessage.cpp MidiFile/MidiMessage.h
        loadingwindow.h loadingwindow.cpp loadingwindow.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ProceduralAudioGenerator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ProceduralAudioGenerator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ProceduralAudioGenerator
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ProceduralAudioGenerator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ProceduralAudioGenerator PRIVATE Qt6::Multimedia)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ProceduralAudioGenerator)
endif()
set_target_properties(ProceduralAudioGenerator PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ProceduralAudioGenerator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProceduralAudioGenerator)
endif()

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets
)
