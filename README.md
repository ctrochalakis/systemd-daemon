# SystemdDaemon

`sd-daemon` bindings for ruby!

## Installation

We need to have `libsystemd-dev` and `libsystemd0` installed in order to
compile systemd-daemon, once those are in place add the following line to your
application's Gemfile:

```ruby
gem 'systemd-daemon'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install systemd-daemon

## Usage

### Ready

```ruby
SystemdDaemon::Notify.ready
```

### Notify with fds
Sending one fd
```ruby
read_fd, write_fd = IO.pipe
state = {
  FDSTORE: 1,
  FDNAME: pid
}
SystemdDaemon::Notify.notify_with_fds(0, state, read_fd.to_i)
```

Sending an array of fds
```ruby
read_fd1, write_fd1 = IO.pipe
read_fd2, write_fd2 = IO.pipe

fds = [read_fd1.to_i, read_fd2.to_i]
state = {
  FDSTORE: 1,
  FDNAME: "testgroup"
}

SystemdDaemon::Notify.notify_with_fds(0, state, fds)
```

### Listen fds with names
```ruby
fds_hash = SystemdDaemon::Notify.listen_fds_with_names
# fds_hash = {"listening_socket_a"=>3, "listening_socket_b"=>4, "listening_socket_c"=>5}
```


## Contributing

1. Fork it ( https://github.com/ctrochalakis/systemd-daemon/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
