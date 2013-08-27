RSpec::Matchers.define :be_close_to do |expected, opts|
  threshold = opts[:threshold] || 10
  match do |actual|
    (expected-threshold..expected+threshold).include? actual
  end
end
