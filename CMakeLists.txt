project(server)
set(CMAKE_CXX_STANDARD 17)

include_directories()
file(GLOB SOURCES "src/*.cpp")
add_executable(server server.pp)
if(WIN32)
  target_link_libraries(MyExe wsock32 ws2_32)
endif()
