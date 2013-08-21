require 'ropencv/ropencv'
require 'tmpdir'

def fixture_path(fixture)
  File.join(File.dirname(__FILE__), 'fixtures', fixture)
end
