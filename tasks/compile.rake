require "rake/extensiontask"

Rake::ExtensionTask.new("spyglass") do |extension|
  extension.lib_dir = "lib/spyglass"
end

