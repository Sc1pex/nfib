sources = [
  'src/cli.c', 
  'src/csv.c',
  'src/run.c',
  'src/sync/arc.c',
  'src/sync/channel.c',
  'src/sync/ringbuf.c',
]
sources_main = sources + ['src/main.c']

test_sources = [
  'test/channel.cpp',
  'test/cli.cpp',
  'test/ringbuff.cpp',
]

cli_inc = include_directories('src')

executable(
  'clitest', 
  sources: sources + test_sources,
  include_directories: cli_inc, 
  dependencies: [lib_dep, gtest_dep],
)

executable('nfib', sources: sources_main, include_directories: cli_inc, dependencies: lib_dep)
