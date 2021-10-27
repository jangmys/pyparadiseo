cmake_minimum_required(VERSION 3.13)

project(pyparadiseo
    VERSION "0.1"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS python3 REQUIRED)
find_package(Boost COMPONENTS numpy3 REQUIRED)
find_package(PythonLibs 3)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_PATH})

link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "-Wall -O2 -fPIC")

set(commonSources
    src/cpp/utils/converters.cpp
    src/cpp/common/random_numbers.cpp
    src/cpp/common/init.cpp
    src/cpp/common/eval.cpp
)

set(moSources
    src/cpp/mo/pymo.cpp
    src/cpp/mo/moEvaluators.cpp
    src/cpp/mo/moNeighborhood.cpp
    src/cpp/mo/moExplorer.cpp
    src/cpp/mo/moContinuator.cpp
    src/cpp/mo/moAlgo.cpp
)

set(eoSources
    src/cpp/eo/pop.cpp
    src/cpp/eo/geneticOps.cpp
    src/cpp/eo/transform.cpp
    src/cpp/eo/selectOne.cpp
    src/cpp/eo/selectors.cpp
    src/cpp/eo/replacement.cpp
    src/cpp/eo/breeders.cpp
    src/cpp/eo/eoAlgos.cpp
)

set(EO_SRC_DIR "src/cpp/eo/src" CACHE INTERNAL "ParadisEO-EO source directory" FORCE)

#inheriting from this somewhere...do we really need these?...
set(eoDep
    ${EO_SRC_DIR}/eoFunctorStore.cpp
    ${EO_SRC_DIR}/utils/eoLogger.cpp
)

add_library(_core SHARED src/cpp/core.cpp ${commonSources} ${eoSources} ${moSources} ${eoDep})

include_directories(${PROJECT_SOURCE_DIR}/src/cpp/)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/mo/)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/mo/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/moeo/src)

set_target_properties(_core PROPERTIES PREFIX "")
target_link_libraries(_core ${PYTHON_LIBRARY} ${Boost_LIBRARIES})

install(TARGETS _core LIBRARY DESTINATION ./ COMPONENT libraries)