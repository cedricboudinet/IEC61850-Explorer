add_executable(test_fun test_fun.C)
add_test(test_fun test_fun)
target_link_libraries(test_fun iec61850Exp)

add_executable(exp61850_svpub exp61850_svpub.C)
target_link_libraries(exp61850_svpub iec61850)
if(UNIX)
	target_link_libraries(exp61850_svpub pthread)
endif(UNIX)
