
include_directories(../core)

add_definitions(-DYACK_SYSTEM_PARAMS=\"${YACK_SYSTEM_PARAMS}\")
add_executable(gen-sys-params gen-sys-params.cpp)
