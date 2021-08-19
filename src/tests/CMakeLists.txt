FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.0-preview3)
FetchContent_MakeAvailable(catch2)

#needed for include(catch) since we are using fetchcontent
#for catch2:
#list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
#for catch3:
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

add_executable(fannn-tests tests.cpp)
target_link_libraries(fannn-tests PRIVATE Catch2::Catch2WithMain fannn-shared)

include(CTest)
include(Catch)
catch_discover_tests(fannn-tests)