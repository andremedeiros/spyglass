$LOAD_PATH.unshift('lib')
require 'spyglass'

include Spyglass 

window  = GUI::Window.new "Original"
output  = GUI::Window.new "Output"
card    = Image.load File.expand_path('images/card.jpg', File.dirname(__FILE__))

window.show(card)

canny = card.convert(ColorSpace[:BGR => :Gray])

canny.canny!(200, 300)
contours  = canny.contours
corners   = contours[0].corners

warped = card.warp_perspective(corners, Size.new(220, 300))

output.show(warped)

loop do
  break if GUI::wait_key(10) > 0
end
