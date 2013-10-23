require 'mkmf'
dir_config("opencv", "/usr/local/include", "/usr/local/lib")
pkg_config("opencv")

create_makefile("spyglass/spyglass")

