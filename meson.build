project('s2s', 'cpp',
  version: '1.0.0',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
  ]
)

srcs = files(
  'src/main.cpp',
  'src/srt.cpp',
  'src/cli.cpp',
  'src/ffmpeg.cpp',
  'src/utils.cpp',
)

deps = []

executable('s2s', srcs, dependencies: deps)
