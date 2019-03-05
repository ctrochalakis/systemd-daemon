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
```ruby
read_fd, write_fd = IO.pipe
state = {
  FDSTORE: 1,
  FDNAME: pid
}
SystemdDaemon::Notify.notify_with_fds(0, state, read_fd.to_i)
```

### Listen fds with names
```ruby
fds_hash = SystemdDaemon::Notify.listen_fds_with_names
# fds_hash = {"5304"=>3, "5301"=>4, "5298"=>5, "5295"=>6}
```


## Contributing

1. Fork it ( https://github.com/ctrochalakis/systemd-daemon/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
