project(core C CXX)

include_directories(.)

file(GLOB src_impl yack/*.c yack/*.cpp)
file(GLOB src_decl yack/*.h yack/*.hpp)
set(src ${src_impl} ${src_decl})

add_library(yack
${src}
)

add_subdirectory(tests)

