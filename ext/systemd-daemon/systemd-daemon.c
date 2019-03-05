#include <ruby.h>

#ifdef HAVE_SYSTEMD_SD_DAEMON_H
#include <systemd/sd-daemon.h>
#endif

#ifdef HAVE_SYSTEMD_SD_DAEMON_H
static VALUE _sd_notify(VALUE mod, VALUE unset_env, VALUE state)
{
  const char * sd_state;
  int return_code;

  sd_state = StringValuePtr(state);
  return_code = sd_notify(FIX2INT(unset_env), sd_state);

  return INT2FIX(return_code);
}

/* Supports only one fd per call */
static VALUE _sd_pid_notify_with_fds(VALUE mod, VALUE pid, VALUE unset_env, VALUE state, VALUE fd)
{
  const char *sd_state;
  int return_code;
  int fd_ref = FIX2INT(fd);

  sd_state = StringValuePtr(state);

  return_code = sd_pid_notify_with_fds(FIX2INT(pid), FIX2INT(unset_env), sd_state, &fd_ref, 1);

  return INT2FIX(return_code);
}

static VALUE _sd_listen_fds_with_names(VALUE mod, VALUE unset_env)
{
  char **fd_names = NULL;
  int i, fds_size;
  VALUE result;

  fds_size = sd_listen_fds_with_names(FIX2INT(unset_env), &fd_names);
  if (fds_size < 0)
    rb_raise(rb_eRuntimeError, "Failed to get listening fds: %d", fds_size);

  result = rb_hash_new();
  for (i = 0; i < fds_size; i++)
    rb_hash_aset(result, rb_str_new_cstr(fd_names[i]), INT2FIX(SD_LISTEN_FDS_START + i));

  free(fd_names);

  return result;
}

static VALUE _sd_watchdog_enabled(VALUE mod, VALUE unset_env)
{
  uint64_t period;
  int r;

#ifdef HAVE_SD_WATCHDOG_ENABLED
  r = sd_watchdog_enabled(FIX2INT(unset_env), &period);
#else
  r = 0;
#endif
  if (r < 0)
    rb_raise(rb_eRuntimeError, "sd_watchdog_enabled returned %d", r);
  else if (r == 0)
    return Qfalse;
  else
    return INT2NUM(period);

}
#else
static VALUE _not_implemented(VALUE mod, VALUE args)
{
  rb_raise(rb_eNotImpError, "systemd-daemon is not supported for this platform");
}
#endif

void Init_sd_native()
{
  VALUE mSD = rb_define_module("SystemdDaemon");
  VALUE mSDNotify = rb_define_module_under(mSD, "Notify");
#ifdef HAVE_SYSTEMD_SD_DAEMON_H
  rb_define_singleton_method(mSDNotify, "_sd_notify", _sd_notify, 2);
  rb_define_singleton_method(mSDNotify, "_sd_pid_notify_with_fds", _sd_pid_notify_with_fds, 4);
  rb_define_singleton_method(mSDNotify, "_sd_listen_fds_with_names", _sd_listen_fds_with_names, 1);
  rb_define_singleton_method(mSDNotify, "_sd_watchdog_enabled", _sd_watchdog_enabled, 1);
#else
  rb_define_singleton_method(mSDNotify, "_sd_notify", _not_implemented, -2);
  rb_define_singleton_method(mSDNotify, "_sd_pid_notify_with_fds", _not_implemented, -2);
  rb_define_singleton_method(mSDNotify, "_sd_listen_fds_with_names", _not_implemented, -2);
  rb_define_singleton_method(mSDNotify, "_sd_watchdog_enabled", _not_implemented, -2);
#endif
}
