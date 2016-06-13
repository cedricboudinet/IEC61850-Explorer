add_executable(test_fun test_fun.C)
add_test(test_fun test_fun)
target_link_libraries(test_fun iec61850Exp)
