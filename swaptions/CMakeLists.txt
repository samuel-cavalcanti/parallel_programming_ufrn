cmake_minimum_required(VERSION 3.10)

# set(CMAKE_CXX_STANDARD 20)
# set the project name
project(swaptions C CXX)

include_directories("src")
link_directories("src")


file(GLOB_RECURSE HEADERS  "src/*.h" )

file(GLOB_RECURSE SOURCE_CPP  "src/*.cpp" )

file(GLOB_RECURSE SOURCE_C  "src/*.c" )

add_definitions(-DENABLE_THREADS -DENABLE_PTHREADS)


# add the executable
add_executable(swaptions 
                        ${HEADERS}
                        ${SOURCE_CPP}
                        ${SOURCE_C})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(swaptions PRIVATE Threads::Threads)
                        