# CMakeList.txt : CMake project for UsbSpy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(
	"UsbSpy"
	VERSION 0.1.0
	DESCRIPTION "Tells you all about USB device activity"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(wtl)

# Add source to this project's executable.
add_executable (UsbSpy WIN32
	"MainFrame.cpp"
	"MainFrame.h"
	"main.cpp"
	
	"app.manifest"
	"resource.rc"
	"resource.h"
)
target_compile_definitions(UsbSpy PRIVATE UNICODE)

target_precompile_headers(UsbSpy PRIVATE
	# ATL sources
	<atlbase.h>
	<atlapp.h>
	<atlctrls.h>
)

target_link_libraries(UsbSpy
	wtl
	comctl32
	uxtheme
	dwmapi
	msimg32
	opengl32
	propsys
)

# TODO: Add tests and install targets if needed.
