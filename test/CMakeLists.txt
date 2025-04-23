add_executable(testDbOps ./testDbOps.cpp ../src/DatabaseOps.cpp)

target_link_libraries(testDbOps mysqlclient)