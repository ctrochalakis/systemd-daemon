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

static VALUE _sd_pid_notify_with_fds(VALUE mod, VALUE pid, VALUE unset_env, VALUE state, VALUE fds)
{
  int i, return_code;

#ifdef HAVE_SD_PID_NOTIFY_WITH_FDS
  const char *sd_state;
  int size = RARRAY_LEN(fds);
  int *fds_ref = (int *)malloc(size * sizeof(int));
  pid_t target_pid = FIXNUM_P(pid) ? NUM2PIDT(pid) : 0;

  sd_state = StringValuePtr(state);
  for(i = 0; i < size; i ++)
    fds_ref[i] = FIX2INT(rb_ary_entry(fds, i));
  return_code = sd_pid_notify_with_fds(target_pid, FIX2INT(unset_env), sd_state, fds_ref, size);

  free(fds_ref);
#else
  /* Indicates that the fds were not send. As when $NOTIFY_SOCKET is not set,
   * no status message could be sent and 0 is returned.
  */
  return_code = 0;
#endif
  return INT2FIX(return_code);
}

static VALUE _sd_listen_fds_with_names(VALUE mod, VALUE unset_env)
{
  char **fd_names = NULL;
  int i, fds_size;
  VALUE result;
#ifdef HAVE_SD_LISTEN_FDS_WITH_NAMES
  fds_size = sd_listen_fds_with_names(FIX2INT(unset_env), &fd_names);
  if (fds_size < 0)
    rb_raise(rb_eRuntimeError, "Failed to get listening fds: %d", fds_size);

  result = rb_hash_new();
  for (i = 0; i < fds_size; i++)
    rb_hash_aset(result, rb_str_new_cstr(fd_names[i]), INT2FIX(SD_LISTEN_FDS_START + i));

  free(fd_names);
#else
  /* Returns an empty hash if sd_listen_fds_with_names is not part of the current API */
  result = rb_hash_new();
#endif

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
