cmake_minimum_required(VERSION 3.14)

project(pyparadiseo
    VERSION "0.1"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT /usr/)

set(BOOST_INCLUDEDIR /usr/include/boost169/)
set(BOOST_LIBRARYDIR /usr/lib64/boost169/)

find_package(Python3 3.6.15 EXACT COMPONENTS Interpreter Development.Module NumPy)
find_package(Boost COMPONENTS python36 numpy36 REQUIRED)
# find_package(Boost 1.65 COMPONENTS numpy3 REQUIRED)

message("==> Python Sitelib : " ${Python3_SITELIB})
message("==> Boost include : " ${Boost_INCLUDE_DIRS})
message("==> Python include : " ${Python3_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Python3_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "-Wall -Wno-maybe-uninitialized -O2 -fPIC -DBOOST_NO_AUTO_PTR")

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
    src/cpp/mo/moComparator.cpp
)

set(eoSources
    src/cpp/eo/pop.cpp
    src/cpp/eo/geneticOps.cpp
    src/cpp/eo/transform.cpp
    src/cpp/eo/selectOne.cpp
    src/cpp/eo/selectors.cpp
    src/cpp/eo/replacement.cpp
    src/cpp/eo/continuators.cpp
    src/cpp/eo/breeders.cpp
    src/cpp/eo/eoAlgos.cpp
    src/cpp/eo/bounds.cpp
    src/cpp/eo/real_op.cpp
    src/cpp/eo/bit_op.cpp
)

set(EO_SRC_DIR "src/cpp/eo/src" CACHE INTERNAL "ParadisEO-EO source directory" FORCE)

#inheriting from this somewhere...do we really need these?...
set(eoDep
    ${EO_SRC_DIR}/eoFunctorStore.cpp
    ${EO_SRC_DIR}/utils/eoLogger.cpp
    ${EO_SRC_DIR}/utils/eoRealBounds.cpp
)

add_library(_core SHARED src/cpp/core.cpp ${commonSources} ${eoSources} ${moSources} ${eoDep})

include_directories(${PROJECT_SOURCE_DIR}/src/cpp/)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/eo/src/es)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/mo/)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/mo/src)
include_directories(${PROJECT_SOURCE_DIR}/src/cpp/moeo/src)

set_target_properties(_core PROPERTIES PREFIX "")


target_link_libraries(_core Python3::Module ${Boost_LIBRARIES} Python3::NumPy)

install(TARGETS _core LIBRARY DESTINATION ./core/ COMPONENT libraries)
