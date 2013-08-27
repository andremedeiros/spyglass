# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'spyglass/version'

Gem::Specification.new do |spec|
  spec.name          = "spyglass"
  spec.version       = Spyglass::VERSION
  spec.authors       = ["André Medeiros"]
  spec.email         = ["me@andremedeiros.info"]
  spec.description   = %q{OpenCV in ruby, made simple.}
  spec.summary       = %q{OpenCV in ruby, made simple.}
  spec.homepage      = "http://github.com/andremedeiros/spyglass"
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "rspec"
end
