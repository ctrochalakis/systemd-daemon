require "bundler/gem_tasks"

require 'rake/extensiontask'
require 'rake/testtask'

Rake::TestTask.new do |t|
  t.libs << 'test'
  t.pattern = "test/test_*.rb"
end

Rake::ExtensionTask.new('sd_native') do |ext|
  ext.ext_dir = 'ext/systemd-daemon'
  ext.lib_dir = 'lib/systemd-daemon'
end

task test: [:compile]
