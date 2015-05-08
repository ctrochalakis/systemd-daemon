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

## Contributing

1. Fork it ( https://github.com/ctrochalakis/systemd-daemon/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
