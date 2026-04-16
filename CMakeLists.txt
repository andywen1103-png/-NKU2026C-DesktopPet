cmake_minimum_required(VERSION 3.16)
project(DesktopPet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia Network)

add_executable(DesktopPet
    main.cpp
    desktop_pet.cpp
    desktop_pet.h
    pet_menu.cpp
    pet_menu.h
    tododialog.cpp
    tododialog.h
)

target_link_libraries(DesktopPet
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Network
)
