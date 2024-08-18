project('simple-psb', 'cpp',
  version : '1.0',
  meson_version : '>= 0.53.2',
  default_options : [ 'buildtype=release', 'cpp_std=c++17', 'warning_level=2'])

# Date used in man pages, update upon release
man_date = '2024-08-18'

# Include directories
subdir('src')
subdir('man')
subdir('systemd')

# Optionally include the examples directory if the feature is enabled
if get_option('examples')
  subdir('examples')
endif

