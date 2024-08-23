sources = [
  'src/fib_impl.c',
  'src/num.c',
  'src/math.c',
]

test_sources = [
  'test/num.cpp'
]

lib_inc = include_directories('include')

executable(
  'libtest', 
  sources: sources + test_sources,
  include_directories: lib_inc, 
  dependencies: gtest_dep,
)

lib = static_library('fiblib', sources, include_directories: lib_inc)
lib_dep = declare_dependency(link_with: lib, include_directories: lib_inc)
