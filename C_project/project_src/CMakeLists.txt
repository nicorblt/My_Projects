# STUDENT CMakeLists.txt
#
# tabs: 8 space

# CMake initialization

cmake_minimum_required(VERSION 2.8)
project("ProjetC_IG")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Paths

get_filename_component(ROOT_DIR		${CMAKE_CURRENT_LIST_FILE} PATH)
set(HEADER_PATHS			"${ROOT_DIR}/include")
set(TESTS_SRC				"${ROOT_DIR}/tests")
set(UNITS_SRC				"${ROOT_DIR}/units")
set(DOCS_DIR				"${ROOT_DIR}/docs")
set(SRC					"${ROOT_DIR}/src")

set(LIB_FLAGS				-lfreeimage -lSDL2 -lSDL2_ttf -lSDL2_gfx)

# Platform specific definitions

if(IG_ASAN)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif(IG_ASAN)

set(WORDS_BIT_SIZE			32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(WORDS_BIT_SIZE		64)
endif()

if(${APPLE})
	set(HEADER_PATHS		${HEADER_PATHS}
					"/opt/local/include/SDL2"
					"/opt/local/include")
	set(PLATFORM_DIR		"${ROOT_DIR}/_osx")
	set(EIBASE			${PLATFORM_DIR}/libeibase.a)
	set(PLATFORM_LIB_FLAGS		${EIBASE}
					-L/opt/local/lib ${LIB_FLAGS})

	message(STATUS "Building for MacOS with eibase: ${EIBASE}")

elseif(${UNIX})

	set(HEADER_PATHS		${HEADER_PATHS}
					"/usr/include/SDL2")
	set(PLATFORM_DIR		"${ROOT_DIR}/_x11")
	set(EIBASE			${PLATFORM_DIR}/libeibase${WORDS_BIT_SIZE}.a)
	set(PLATFORM_LIB_FLAGS		${EIBASE}
					-L${PLATFORM_DIR} ${LIB_FLAGS} -lm)

	message(STATUS "Building for Linux with eibase: ${EIBASE}")
	add_compile_options(-Wall -Wextra -Wpedantic -pg)

else()

	set(SDL				"C:/projetc/SDL2_windows")
	set(HEADER_PATHS		${HEADER_PATHS}
					"${SDL}/include")
	add_definitions(-D__WIN__=1 -D_CRT_SECURE_NO_WARNINGS=1)

	if(${WORDS_BIT_SIZE} EQUAL 64)
		set(ARCH		x64)
		add_definitions(-D_AMD64_=1)
	else(${WORDS_BIT_SIZE} EQUAL 64)
		set(ARCH		x86)
		add_definitions(-D_X86_=1)
	endif(${WORDS_BIT_SIZE} EQUAL 64)

	set(PLATFORM_DIR		"${ROOT_DIR}/_win")
	set(EIBASE			${PLATFORM_DIR}/eibase${WORDS_BIT_SIZE}.lib)
	set(PLATFORM_LIB_FLAGS		${EIBASE}
					${SDL}/lib/${ARCH}/SDL2.lib
					${SDL}/lib/${ARCH}/SDL2_gfx.lib
					${SDL}/lib/${ARCH}/SDL2_ttf.lib
					${SDL}/lib/${ARCH}/FreeImage.lib)

	message(STATUS "Building for Windows with eibase: ${EIBASE}")

endif(${APPLE})

# General definitions for all targets

include_directories(${HEADER_PATHS})

# target ei (libei)

set(LIB_EI_SOURCES
	${SRC}/ei_draw.c
	${SRC}/ei_widget.c
	${SRC}/ei_event.c
	${SRC}/ei_geometrymanager.c
	${SRC}/ei_application.c
	${SRC}/ei_widgetclass.c
	${SRC}/helpers/ei_memory.c
	${SRC}/helpers/ei_geometry_helpers.c
	${SRC}/helpers/ei_draw_helpers.c
	${SRC}/widgets/ei_frame.c
	${SRC}/widgets/ei_radiobutton.c
	${SRC}/widgets/ei_toplevel.c
	${SRC}/widgetclasses/ei_frameclass.c
	${SRC}/widgetclasses/ei_buttonclass.c
	${SRC}/widgetclasses/ei_toplevelclass.c
	${SRC}/widgetclasses/ei_radiobuttonclass.c
	${SRC}/widgetclasses/internal_events/ei_toplevelclass_internal_event.c
	${SRC}/geometry_managers/ei_placer.c
	${SRC}/iterators/ei_subsurface_iterator.c
	${SRC}/iterators/ei_widget_tree_iterator.c
	${SRC}/internals/ei_geometrymanager_internal.c
	${SRC}/internals/ei_picking_internal.c
	${SRC}/internals/ei_widgetclass_internal.c
	${SRC}/internals/ei_application_internal.c
	${SRC}/internals/event_internals/event_internal.c
	${SRC}/internals/event_internals/linked_callback.c
	${SRC}/internals/event_internals/linked_widget.c
	${SRC}/internals/event_internals/linked_tag_and_widgets.c
	${SRC}/internals/event_internals/widget_hashmap.c
)

add_library(ei STATIC			${LIB_EI_SOURCES})

# target minimal

add_executable(minimal 			${TESTS_SRC}/minimal.c)
target_link_libraries(minimal		${PLATFORM_LIB_FLAGS})

# target frame

add_executable(frame			${TESTS_SRC}/frame.c)
target_link_libraries(frame		ei ${PLATFORM_LIB_FLAGS})

# target button

add_executable(button			${TESTS_SRC}/button.c)
target_link_libraries(button		ei ${PLATFORM_LIB_FLAGS})

# target hello_world

add_executable(hello_world		${TESTS_SRC}/hello_world.c)
target_link_libraries(hello_world	ei ${PLATFORM_LIB_FLAGS})

# target puzzle

add_executable(puzzle			${TESTS_SRC}/puzzle.c)
target_link_libraries(puzzle		ei ${PLATFORM_LIB_FLAGS})

# target two048

add_executable(two048			${TESTS_SRC}/two048.c)
target_link_libraries(two048		ei ${PLATFORM_LIB_FLAGS})

# target minesweeper

add_executable(minesweeper		${TESTS_SRC}/minesweeper.c)
target_link_libraries(minesweeper	ei ${PLATFORM_LIB_FLAGS})

# target parsing

add_executable(parsing			${TESTS_SRC}/parsing.c)
target_link_libraries(parsing		ei ${PLATFORM_LIB_FLAGS})

# target frame_text

add_executable(frame_text			${TESTS_SRC}/frame_text.c)
target_link_libraries(frame_text		ei ${PLATFORM_LIB_FLAGS})

# target frame_image

add_executable(frame_image			${TESTS_SRC}/frame_image.c)
target_link_libraries(frame_image		ei ${PLATFORM_LIB_FLAGS})

# target frame_multiple

add_executable(frame_multiple			${TESTS_SRC}/frame_multiple.c)
target_link_libraries(frame_multiple		ei ${PLATFORM_LIB_FLAGS})

# target frame_relief

add_executable(frame_relief			${TESTS_SRC}/frame_relief.c)
target_link_libraries(frame_relief		ei ${PLATFORM_LIB_FLAGS})

# target button_limit

add_executable(button_limit			${TESTS_SRC}/button_limit.c)
target_link_libraries(button_limit		ei ${PLATFORM_LIB_FLAGS})

# target toplevel

add_executable(toplevel			${TESTS_SRC}/toplevel.c)
target_link_libraries(toplevel		ei ${PLATFORM_LIB_FLAGS})

# target radiobutton

add_executable(radiobutton 			${TESTS_SRC}/radiobutton.c)
target_link_libraries(radiobutton	ei	${PLATFORM_LIB_FLAGS})

# target test_ei_draw

add_executable(test_ei_draw 			${UNITS_SRC}/test_ei_draw.c)
target_link_libraries(test_ei_draw		ei ${PLATFORM_LIB_FLAGS})

# target test_ei_frameclass

add_executable(test_ei_frameclass 			${UNITS_SRC}/test_ei_frameclass.c)
target_link_libraries(test_ei_frameclass	ei	${PLATFORM_LIB_FLAGS})


# target test_ei_geometry_helpers

add_executable(test_ei_geometry_helpers 			${UNITS_SRC}/test_ei_geometry_helpers.c)
target_link_libraries(test_ei_geometry_helpers	ei	${PLATFORM_LIB_FLAGS})


# target test_linked_callback

add_executable(test_linked_callback 			${UNITS_SRC}/event_internals/test_linked_callback.c)
target_compile_definitions(test_linked_callback PUBLIC UNIT_TEST)
target_link_libraries(test_linked_callback	ei	${PLATFORM_LIB_FLAGS})


# target test_linked_widget

add_executable(test_linked_widget 			${UNITS_SRC}/event_internals/test_linked_widget.c)
target_compile_definitions(test_linked_widget PUBLIC UNIT_TEST)
target_link_libraries(test_linked_widget	ei	${PLATFORM_LIB_FLAGS})

# target test_widget_hashmap

add_executable(test_widget_hashmap 			${UNITS_SRC}/event_internals/test_widget_hashmap.c)
target_compile_definitions(test_widget_hashmap PUBLIC UNIT_TEST)
target_link_libraries(test_widget_hashmap	ei	${PLATFORM_LIB_FLAGS})

# target test_linked_tag_and_widgets

add_executable(test_linked_tag_and_widgets 			${UNITS_SRC}/event_internals/test_linked_tag_and_widgets.c)
target_compile_definitions(test_linked_tag_and_widgets PUBLIC UNIT_TEST)
target_link_libraries(test_linked_tag_and_widgets	ei	${PLATFORM_LIB_FLAGS})

# target test_event_internal

add_executable(test_event_internal 			${UNITS_SRC}/event_internals/test_event_internal.c)
target_compile_definitions(test_event_internal PUBLIC UNIT_TEST)
target_link_libraries(test_event_internal	ei	${PLATFORM_LIB_FLAGS})

# target test_ei_widget

add_executable(test_ei_widget 			${UNITS_SRC}/test_ei_widget.c)
target_link_libraries(test_ei_widget	ei	${PLATFORM_LIB_FLAGS})

# target test_ei_application

add_executable(test_ei_application 			${UNITS_SRC}/test_ei_application.c)
target_link_libraries(test_ei_application	ei	${PLATFORM_LIB_FLAGS})


# target to build the documentation

#add_custom_target(doc doxygen		${DOCS_DIR}/doxygen.cfg WORKING_DIRECTORY ${ROOT_DIR})

find_package(Doxygen)
if (DOXYGEN_FOUND)
	set(DOXYGEN_IN ${DOCS_DIR}/doxygen.cfg.in)
	set(DOXYGEN_OUT ${DOCS_DIR}/doxygen.cfg)

	message("Building Doxygen documentation.")
	
	set(DOXYGEN_GENERATE_HTML YES)
	set(DOXYGEN_GENERATE_MAN NO)
	set(DOXYGEN_GENERATE_LATEX NO)
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

	add_custom_target(doc 
	COMMAND doxygen ${DOXYGEN_OUT}
	WORKING_DIRECTORY ${ROOT_DIR})
else ()
	message("Doxygen not found")
endif(DOXYGEN_FOUND)
