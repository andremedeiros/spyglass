$LOAD_PATH.unshift('lib')
require 'spyglass'

include Spyglass 

window  = GUI::Window.new "Original"
apple   = Image.load File.expand_path('images/apple.jpg', File.dirname(__FILE__))

window.show(apple)

canny = apple.convert(ColorSpace[:BGR => :Gray])
canny.canny!(200, 300)

result = Image.new
result.copy!(apple)
result.draw_contours(canny.contours, Color.new(255, 0, 0))

result_window = GUI::Window.new "Contours"
result_window.show(result)

loop do
  break if GUI::wait_key(10) > 0
end

