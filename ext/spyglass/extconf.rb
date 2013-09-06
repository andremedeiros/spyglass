require 'mkmf'

$CFLAGS     = ENV["CFLAGS"].to_s  + " " + `pkg-config opencv --cflags`.chomp
$LOCAL_LIBS = ENV["LIBS"].to_s    + " " + `pkg-config opencv --libs`.chomp

create_makefile("spyglass/spyglass")
