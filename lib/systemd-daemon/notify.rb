module SystemdDaemon
  module Notify
    extend self

    def ready(unset_env=false, state=nil)
      notify(unset_env, READY: 1)
    end

    def notify(unset_env=false, state)
      _sd_notify(unset_env_value(unset_env), hash_to_sd_state(state))
    end

    def watchdog_timer(unset_env=false)
      _sd_watchdog_enabled(unset_env_value(unset_env))
    end

    def watchdog_ping
      notify(false, WATCHDOG: 1)
    end

    def watchdog?
      !! watchdog_timer
    end

    private

    def unset_env_value(value)
      value ? 1 : 0
    end

    def hash_to_sd_state(h)
      h.map {|pair| pair.join('=') }.join("\n")
    end

  end
end
