macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

if(SKBUILD)
  message(STATUS "The project is built using scikit-build")
endif()

cmake_minimum_required(VERSION 3.14)

project(pyparadiseo
    VERSION "0.3.1"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

# set(BOOST_ROOT /usr/local/)
# set(BOOST_INCLUDEDIR /usr/local/include/boost/)
# set(BOOST_LIBRARYDIR /usr/local/lib/)
# set(BOOST_ROOT /usr/)
# set(BOOST_INCLUDEDIR /usr/include/boost169/)
# set(BOOST_LIBRARYDIR /usr/lib64/boost169/)

find_package(Python3 COMPONENTS Interpreter Development.Module NumPy)

message(STATUS "Python_VERSION=${Python3_VERSION}")
message(STATUS ${${Python3_VERSION}})
message(STATUS ${Python3_VERSION})

find_package(Boost COMPONENTS python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR} numpy${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR} REQUIRED)

# print_all_variables()

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Python3_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})

###!!!boost.python uses deprecated PyEval_CallFunction : lots of warnings!!!
set(CMAKE_CXX_FLAGS "-Wall -Wno-maybe-uninitialized -Wno-deprecated-declarations -O2 -fPIC -DBOOST_NO_AUTO_PTR")

set(commonSources
    src/cpp/utils/converters.cpp
    src/cpp/utils/valueParam.cpp
    src/cpp/utils/stats.cpp
    src/cpp/utils/monitors.cpp
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
    src/cpp/mo/moComparator.cpp
    src/cpp/mo/moSampling.cpp
    src/cpp/mo/moStat.cpp
)

set(moeoSources
    src/cpp/moeo/moeoAbstract.cpp
    src/cpp/moeo/moeoAlgos.cpp
    src/cpp/moeo/moeoFitnessAssign.cpp
    src/cpp/moeo/moeoDiversityAssign.cpp
    src/cpp/moeo/moeoDistances.cpp
    src/cpp/moeo/moeoArchive.cpp
)

set(eoSources
    src/cpp/eo/abstract.cpp
    src/cpp/eo/pop.cpp
    src/cpp/eo/geneticOps.cpp
    src/cpp/eo/mergers.cpp
    src/cpp/eo/reduce.cpp
    src/cpp/eo/transform.cpp
    src/cpp/eo/selectOne.cpp
    src/cpp/eo/selectors.cpp
    src/cpp/eo/replacement.cpp
    src/cpp/eo/continuators.cpp
    src/cpp/eo/breeders.cpp
    src/cpp/eo/eoAlgos.cpp
    src/cpp/eo/bounds.cpp
    src/cpp/eo/int_bounds.cpp
    src/cpp/eo/real_op.cpp
    src/cpp/eo/bit_op.cpp
    src/cpp/pso/eoPSO.cpp
)

set(EO_SRC_DIR "src/paradiseo/eo" CACHE INTERNAL "ParadisEO-EO source directory" FORCE)

#inheriting from this somewhere...do we really need these?...
set(eoDep
    ${EO_SRC_DIR}/eoFunctorStore.cpp
    ${EO_SRC_DIR}/utils/eoLogger.cpp
    ${EO_SRC_DIR}/utils/eoRealBounds.cpp
)

add_library(_core SHARED src/cpp/core.cpp ${commonSources} ${eoSources} ${moSources} ${moeoSources} ${eoDep})

add_library(_mod SHARED src/cpp/mod.cpp)

#PARADISEO sources
include_directories(${PROJECT_SOURCE_DIR}/src/paradiseo/eo/)
include_directories(${PROJECT_SOURCE_DIR}/src/paradiseo/mo/)
include_directories(${PROJECT_SOURCE_DIR}/src/paradiseo/moeo/)

#BOOST PYTHON sources
#COMMON
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/operators)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/src/es)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/src/pso)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/mo/)


set_target_properties(_core PROPERTIES PREFIX "")
set_target_properties(_mod PROPERTIES PREFIX "")

target_link_libraries(_core Python3::Module ${Boost_LIBRARIES} Python3::NumPy)

target_link_libraries(_mod Python3::Module ${Boost_LIBRARIES} Python3::NumPy)

install(TARGETS _core LIBRARY DESTINATION ./ COMPONENT libraries)
install(TARGETS _mod LIBRARY DESTINATION ./ COMPONENT libraries)
