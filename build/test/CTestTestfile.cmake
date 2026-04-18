# CMake generated Testfile for 
# Source directory: C:/Users/HP/Downloads/Transaction_manager/test
# Build directory: C:/Users/HP/Downloads/Transaction_manager/build/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(TransactionTests "C:/Users/HP/Downloads/Transaction_manager/build/test/test_transactions.exe")
set_tests_properties(TransactionTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/HP/Downloads/Transaction_manager/test/CMakeLists.txt;24;add_test;C:/Users/HP/Downloads/Transaction_manager/test/CMakeLists.txt;0;")
add_test(DeadlockTests "C:/Users/HP/Downloads/Transaction_manager/build/test/test_deadlock.exe")
set_tests_properties(DeadlockTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/HP/Downloads/Transaction_manager/test/CMakeLists.txt;25;add_test;C:/Users/HP/Downloads/Transaction_manager/test/CMakeLists.txt;0;")
add_test(RecoveryTests "C:/Users/HP/Downloads/Transaction_manager/build/test/test_recovery.exe")
set_tests_properties(RecoveryTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/HP/Downloads/Transaction_manager/test/CMakeLists.txt;26;add_test;C:/Users/HP/Downloads/Transaction_manager/test/CMakeLists.txt;0;")
