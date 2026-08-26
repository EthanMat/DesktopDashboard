cmake_minimum_required(VERSION 3.20)

project(Backdrop VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# WebView2 SDK
set(WEBVIEW2_ROOT
    "${CMAKE_SOURCE_DIR}/third-party/microsoft.web.webview2.1.0.4129.50/build/native"
)

add_executable(Backdrop WIN32
    src/main.cpp
    src/app.rc
)

# WebView2 headers
target_include_directories(Backdrop PRIVATE
    "${WEBVIEW2_ROOT}/include"
    "${WEBVIEW2_ROOT}/include-winnt"
)

# WebView2 library
target_link_libraries(Backdrop PRIVATE
    "${WEBVIEW2_ROOT}/x64/WebView2LoaderStatic.lib"
)

# Windows libraries required by WebView2
target_link_libraries(Backdrop PRIVATE
    ole32
    oleaut32
    user32
    shlwapi
    version
)