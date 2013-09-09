$LOAD_PATH.unshift('lib')
require 'spyglass'

include Spyglass 

classifier  = CascadeClassifier.new("spec/fixtures/haarcascade_frontalface_default.xml")
window      = GUI::Window.new "Video"
cap         = VideoCapture.new 0
frame       = Image.new

loop do
  cap >> frame

  rects = classifier.detect(frame, scale_factor: 1.5, min_size: Size.new(30, 30))
  rects.map { |r| frame.draw_rectangle(r, Color.new(255, 0, 0)) }

  window.show(frame)

  break if GUI::wait_key(10) > 0
end

