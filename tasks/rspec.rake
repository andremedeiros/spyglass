require "rspec/core/rake_task"
namespace :spec do
  desc "Run all examples with Valgrind"
  task :valgrind do
    VALGRIND_OPTS = %w{
      --num-callers=50
      --error-limit=no
      --partial-loads-ok=yes
      --undef-value-errors=no
      --trace-children=yes
    }

    cmd = "valgrind #{VALGRIND_OPTS.join(' ')} bundle exec rake spec"
    puts cmd
    system cmd
  end
end

RSpec::Core::RakeTask.new('spec') do |t|
  t.verbose = true
end
