cmake_minimum_required(VERSION 3.10)


# set the project name
project(swaptions-pthreads C CXX)

project(swaptions-openmp C CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

include_directories("src")
link_directories("src")


file(GLOB_RECURSE HEADERS  "src/*.h" )

file(GLOB_RECURSE SOURCE_CPP  "src/*.cpp" )

file(GLOB_RECURSE SOURCE_C  "src/*.c" )




# add the executable
add_executable(swaptions-pthreads 
                        ${HEADERS}
                        ${SOURCE_CPP}
                        ${SOURCE_C})

                        # add the executable
add_executable(swaptions-openmp 
                        ${HEADERS}
                        ${SOURCE_CPP}
                        ${SOURCE_C})
#adding pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(TBB REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(swaptions-openmp PRIVATE OpenMP::OpenMP_CXX)

target_link_libraries(swaptions-pthreads PRIVATE Threads::Threads)


target_compile_definitions(swaptions-pthreads PUBLIC ENABLE_THREADS=1 ENABLE_PTHREADS=1)
target_compile_definitions(swaptions-openmp PUBLIC ENABLE_THREADS=1 OPENMP_VERSION=1)


set_target_properties(swaptions-openmp PROPERTIES COMPILE_FLAGS -fopenmp)

# para degugs de variáveis 
# macro(print_all_variables)
#    message(STATUS "print_all_variables------------------------------------------{")
#    foreach (file ${HEADERS})
#        message(STATUS "soruce: ${file}\n")
#    endforeach()
#    message(STATUS "print_all_variables------------------------------------------}")
# endmacro()

# print_all_variables()