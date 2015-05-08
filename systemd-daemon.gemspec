# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'systemd-daemon/version'

Gem::Specification.new do |spec|
  spec.name          = "systemd-daemon"
  spec.version       = SystemdDaemon::VERSION
  spec.authors       = ["Christos Trochalakis"]
  spec.email         = ["yatiohi@ideopolis.gr"]
  spec.license       = "MIT"

  spec.summary       = %q{Systemd's sd-daemon ruby bindings}
  spec.description   = %q{Systemd's sd-daemon ruby bindings}
  spec.homepage      = "https://github.com/ctrochalakis/systemd-daemon"
  spec.files         = `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|bin)/}) }

  spec.extensions    = 'ext/systemd-daemon/extconf.rb'
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.9"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler", "~> 0.9.5"
  spec.add_development_dependency "test-unit", "~> 3.0.9"
end
