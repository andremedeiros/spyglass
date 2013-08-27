require 'rake'

Dir['tasks/*.rake'].sort.each { |f| load f }

task :default => [:clean, :compile, :spec]
