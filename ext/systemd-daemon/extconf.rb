require 'mkmf'

def asplode missing
  abort "#{missing} is missing."
end

sd_daemon_h = have_header('systemd/sd-daemon.h')
# Exit early on OS X
if RbConfig::CONFIG["host_os"] =~ /mac|darwin/
  create_makefile('systemd-daemon/sd_native')
  exit
end

asplode('systemd/sd-daemon.h') if !sd_daemon_h
asplode('libsystemd/libsystemd-daemon') if !have_library('systemd') && !have_library('systemd-daemon')
asplode('sd_notify') if not have_func('sd_notify', 'systemd/sd-daemon.h')

have_func('sd_watchdog_enabled')

create_makefile('systemd-daemon/sd_native')
