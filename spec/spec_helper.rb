require 'spyglass/spyglass'
require 'tmpdir'

require 'matchers/close_to'

def fixture_path(fixture)
  File.join(File.dirname(__FILE__), 'fixtures', fixture)
end
