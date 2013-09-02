$LOAD_PATH.unshift('lib')
require 'spyglass'

include Spyglass 

window = GUI::Window.new "Original"
shapes = Image.load File.expand_path('images/apple.jpg', File.dirname(__FILE__))

window.show(shapes)

contours = shapes.contours
puts contours.length
img = Image.new
img.copy!(shapes)
img.draw_contours(contours)

result_window = GUI::Window.new "Contours"
result_window.show(img)

loop do
  break if GUI::wait_key(10) > 0
end

