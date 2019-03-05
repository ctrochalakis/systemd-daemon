require 'helper'
require 'socket'

class TestSystemdDaemonNotify < Test::Unit::TestCase

  def assert_socket(expected_data, msg=nil)
    notify_socket = "@test-systemd-daemon-#{$$}"
    ENV['NOTIFY_SOCKET'] = notify_socket
    s = Socket.new(:UNIX, :DGRAM, 0)
    s.bind(Addrinfo.unix(notify_socket.gsub('@', "\0")))

    yield

    assert_equal expected_data, s.recvmsg[0], msg
  ensure
    ENV.delete 'NOTIFY_SOCKET'
    s.close
  end

  def with_env(env)
    old_env = {}
    env.each { |k, v| old_env[k], ENV[k] = ENV[k], v.to_s }
    yield
  ensure
    old_env.each { |k,v| ENV[k] = v }
  end

  def test_ready
    assert_socket('READY=1') {
      assert SystemdDaemon::Notify.ready > 0
    }
  end

  def test_unset_env
    assert_socket('READY=1') {
      assert SystemdDaemon::Notify.ready(true) > 0
      assert_equal 0, SystemdDaemon::Notify.ready
    }
  end

  def test_watchdog_ping
    assert_socket('WATCHDOG=1') {
      SystemdDaemon::Notify.watchdog_ping
    }
  end

  def test_watchdog_is_not_enabled
    assert_equal false, SystemdDaemon::Notify.watchdog_timer
    assert_equal false, SystemdDaemon::Notify.watchdog?
  end

  def test_watchdog_is_enabled
    with_env('WATCHDOG_USEC' => '5000', 'WATCHDOG_PID' => $$) {
      assert_equal 5000, SystemdDaemon::Notify.watchdog_timer
      assert_equal true, SystemdDaemon::Notify.watchdog?
    }
  end

  def test_listen_fds_with_names
    fds_with_names = { "foo" => 3, "bar" => 4 }

    with_env('LISTEN_FDNAMES' => fds_with_names.keys.join(":"),
             'LISTEN_FDS' => fds_with_names.size,
             'LISTEN_PID' => $$) {
      assert_equal fds_with_names, SystemdDaemon::Notify.listen_fds_with_names
    }
  end

  def test_notify_with_one_fd
    assert_socket("FDSTORE=1\nFDNAME=test") {
      pid = 0
      fd  = 3
      state = { FDSTORE: 1, FDNAME: 'test' }

      SystemdDaemon::Notify.notify_with_fds(pid, state, fd)
    }
  end

  def test_notify_with_fds
    assert_socket("FDSTORE=1\nFDNAME=testgroup") {
      pid = 0
      fd  = [3, 4]
      state = { FDSTORE: 1, FDNAME: 'testgroup' }

      SystemdDaemon::Notify.notify_with_fds(pid, state, fd)
    }
  end
end
