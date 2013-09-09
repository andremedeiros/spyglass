$LOAD_PATH.unshift('lib')
require 'spyglass'

include Spyglass

puts <<-eos
WARNING: This demo might not work well with cameras that do auto-exposure.
         If you can, disable those features for a better feel of this demo.

eos

bg      = BackgroundSubtractor.new
window  = GUI::Window.new("Beach!")
cap     = VideoCapture.new 0, width: 640, height: 480
frame   = Image.new
result  = Image.new
beach   = Image.load File.expand_path('images/beach.jpg', File.dirname(__FILE__))

puts "Step off the camera's field of vision!"
3.times { |sec| puts "... #{ 3 - sec }"; sleep(1) }

puts "Training background subtractor..."
20.times { cap >> frame; bg.subtract frame, 0.05 }

puts "Show yourself ;-)"

loop do
  cap >> frame

  result.copy!(beach)
  delta = bg.subtract(frame, 0)
  result.copy!(frame, delta);

  window.show result

  break if GUI::wait_key(10) > 0
end
