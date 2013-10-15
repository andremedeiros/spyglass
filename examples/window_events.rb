$LOAD_PATH.unshift('lib')
require 'spyglass'

include Spyglass 

window  = GUI::Window.new "Original"
card    = Image.load File.expand_path('images/card.jpg', File.dirname(__FILE__))

window.on_click do |x, y|
  puts "CLICKED: #{ x }, #{ y }"
end

window.on_double_click do |x, y|
  puts "DOUBLE CLICKED: #{ x }, #{ y }"
end

window.on_right_click do |x, y|
  puts "RIGHT CLICKED: #{ x }, #{ y }"
end

window.show(card)

loop do
  break if GUI::wait_key(500) > 0
end

