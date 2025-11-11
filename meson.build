project('s2s', 'cpp',
  version: '1.0.0',
  default_options: [
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
