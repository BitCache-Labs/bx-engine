cmake_minimum_required (VERSION 3.22)

project (bx-engine)

# External libraries
#add_subdirectory(extern)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_subdirectory (wren)

# Setup bx-engine library
set (BX_ENGINE_SRCS
	"src/bx/engine/data.cpp"
	"src/bx/engine/ecs.cpp"
	"src/bx/engine/engine.cpp"
	"src/bx/engine/resource.cpp"
	"src/bx/engine/script.cpp"
)

add_library (bx-engine STATIC ${BX_ENGINE_SRCS})
target_link_libraries (bx-engine PUBLIC bx wren file_embed)
target_include_directories (bx-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(bx-engine PRIVATE BX_ENGINE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

# Editor sources and libraries
if (BX_BUILD_EDITOR)
    target_sources(bx-engine PRIVATE
		"src/bx/engine/editor/views/assets_view.cpp"
		"src/bx/engine/editor/views/console_view.cpp"
		"src/bx/engine/editor/views/data_view.cpp"
		"src/bx/engine/editor/views/entity_view.cpp"
		"src/bx/engine/editor/views/profiler_view.cpp"
		"src/bx/engine/editor/views/script_view.cpp"
		"src/bx/engine/editor/views/settings_view.cpp"

		"src/bx/engine/editor/assets.cpp"
		"src/bx/engine/editor/asset_importer.cpp"
		"src/bx/engine/editor/toolbar.cpp"
	)
	#target_link_libraries(bx-engine PRIVATE implot)
endif()