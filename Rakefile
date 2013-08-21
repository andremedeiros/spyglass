require "bundler/gem_tasks"
require "rake/extensiontask"
require "rspec/core/rake_task"

Rake::ExtensionTask.new("ropencv") do |extension|
  extension.lib_dir = "lib/ropencv"
end

RSpec::Core::RakeTask.new('spec') do |t|
  t.verbose = true
end

task :default => [:clean, :compile, :spec]
