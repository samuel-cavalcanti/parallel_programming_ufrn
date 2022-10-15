cmake_minimum_required(VERSION 3.10)


# set the project name
project(swaptions)


set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

include_directories("src")
link_directories("src")


# seaching all .h files in src directory
# and put the list of files on variable  HEADERS
# I do the same thing with  .cpp , .c  files 
file(GLOB_RECURSE HEADERS  "src/*.h" )

file(GLOB_RECURSE SOURCE_CPP  "src/*.cpp" )

file(GLOB_RECURSE SOURCE_C  "src/*.c" )

file(GLOB_RECURSE UNITARY_TEST  "src/*_test.cpp" )

file(GLOB_RECURSE MAIN_FILE  "src/main.cpp" )

#remving double int main definition
list(REMOVE_ITEM SOURCE_CPP  ${MAIN_FILE})

#remobing all test files from SOURCE_CPP
list(REMOVE_ITEM SOURCE_CPP ${UNITARY_TEST})


add_executable(swaptions-pthreads
                        ${MAIN_FILE}
                        ${HEADERS}
                        ${SOURCE_CPP}
                        ${SOURCE_C})

                        
add_executable(swaptions-openmp
                        ${MAIN_FILE}
                        ${HEADERS}
                        ${SOURCE_CPP}
                        ${SOURCE_C})

add_executable(swaptions-pthreads-test 
                        ${HEADERS}
                        ${SOURCE_CPP}
                        ${SOURCE_C}
                        ${UNITARY_TEST})

add_executable(swaptions-openmp-test 
                        ${HEADERS}
                        ${SOURCE_CPP}
                        ${SOURCE_C}
                        ${UNITARY_TEST})


# finding Pthreads and OpenMP
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

# find the pascal library   
find_library(PASCAL manualinst PATHS "pascal-releases/lib/" REQUIRED)
message(STATUS "Found Pascal: ${PASCAL}")

#adding OpenMP
target_link_libraries(swaptions-openmp PRIVATE OpenMP::OpenMP_CXX OpenMP::OpenMP_C)
target_link_libraries(swaptions-openmp-test PRIVATE OpenMP::OpenMP_CXX OpenMP::OpenMP_C)

#adding pthreads
target_link_libraries(swaptions-pthreads PRIVATE Threads::Threads)
target_link_libraries(swaptions-pthreads-test PRIVATE Threads::Threads)

#adding pascal
target_link_libraries(swaptions-pthreads PRIVATE ${PASCAL})
target_link_libraries(swaptions-openmp PRIVATE ${PASCAL})
target_link_libraries(swaptions-pthreads-test PRIVATE ${PASCAL})
target_link_libraries(swaptions-openmp-test PRIVATE ${PASCAL})



target_compile_definitions(swaptions-pthreads PUBLIC ENABLE_THREADS=1 PTHREADS_VERSION=1)
target_compile_definitions(swaptions-pthreads-test PUBLIC ENABLE_THREADS=1 PTHREADS_VERSION=1)


target_compile_definitions(swaptions-openmp PUBLIC ENABLE_THREADS=1 OPENMP_VERSION=1)
target_compile_definitions(swaptions-openmp-test PUBLIC ENABLE_THREADS=1 OPENMP_VERSION=1)
