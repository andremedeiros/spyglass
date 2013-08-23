RSpec::Matchers.define :be_close_to do |expected, threshold|
  match do |actual|
    (expected-threshold..expected+threshold).include? actual
  end
end
