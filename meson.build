project('nfib', 'c', 'cpp', default_options: ['c_std=c17', 'cpp_std=c++23'])

gtest = subproject('gtest')
gtest_dep = gtest.get_variable('gtest_main_dep')

subdir('fib_lib')
subdir('cli')
