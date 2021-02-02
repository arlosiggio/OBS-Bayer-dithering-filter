# CMakeList.txt: proyecto de CMake para plugin, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

include("${libobs_DIR}/cmake/LibObs/LibObsConfig.cmake")

# Agregue un origen al ejecutable de este proyecto.
set(plugin_SOURCES plugin.c)

add_library(filtro MODULE ${plugin_SOURCES})
target_link_libraries(filtro libobs)
target_include_directories(filtro PRIVATE "${libobs_DIR}/include")



# Detect Architecture
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")

install(TARGETS filtro
        RUNTIME DESTINATION "${OBS_DIR}/obs-plugins/${BITS}bit/" COMPONENT Runtime
        LIBRARY DESTINATION "${OBS_DIR}/obs-plugins/${BITS}bit/" COMPONENT Runtime)
if(MSVC)
	install(FILES $<TARGET_PDB_FILE:filtro>
		    DESTINATION "${OBS_DIR}/obs-plugins/${BITS}bit/"
		    OPTIONAL)
endif()
	
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data/"
	    DESTINATION "${OBS_DIR}/data/obs-plugins/filtro/")