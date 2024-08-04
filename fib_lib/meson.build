lib_inc = include_directories('include')
sources = [
  'src/fib_impl.c',
  'src/num.c'
]
lib = static_library('fiblib', sources, include_directories: lib_inc)
lib_dep = declare_dependency(link_with: lib, include_directories: lib_inc)
