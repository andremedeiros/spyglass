$LOAD_PATH.unshift('lib')
require 'spyglass'

include Spyglass

window  = GUI::Window.new "Video"
cap     = VideoCapture.new 0
frame   = Image.new

loop do
  cap >> frame
  window.show(frame)

  break if GUI::wait_key(100) > 0
end
