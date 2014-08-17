/* 
 * Copyright (C) 2014, Enrico M. Crisostomo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBFSW_H
#  define LIBFSW_H

#  include "cevent.h"
#  include "cmonitor.h"
#  include "cfilter.h"
#  include "error.h"

#  ifdef __cplusplus
extern "C"
{
#  endif

  typedef unsigned int FSW_HANDLE;
  typedef int FSW_STATUS;

#  define FSW_INVALID_HANDLE -1

#  if defined(HAVE_CXX_THREAD_LOCAL)
#    define FSW_THREAD_LOCAL thread_local
#  endif

  /*
   * The libfswatch C API let users create monitor sessions and receive file
   * system events matching the specified criteria.  Most API functions return 
   * a status code of type FSW_STATUS which can take any value specified in
   * the error.h header.  A successful API call returns FSW_OK and the last
   * error can be obtained calling the fsw_last_error() function.
   * 
   * If the compiler and the C++ library used to build libfswatch support the
   * thread_local storage specified then this API is thread safe and a
   * different state is maintained on a per-thread basis.
   * 
   * Session-modifying API calls (such as fsw_add_path) will take effect the
   * next time a monitor is started with fsw_start_monitor.
   * 
   * Currently not all monitors supports being stopped, in which case
   * fsw_start_monitor is a non-returning API call.
   * 
   * A basic session needs at least:
   * 
   *   * A path to watch.
   *   * A callback to process the events sent by the monitor.
   * 
   * as shown in the next example (error checking code was omitted).
   * 
   * +------------------------------------------------------------------------+
   * // Use the default monitor.
   * const FSW_HANDLE handle = fsw_init_session();
   *
   * fsw_add_path(handle, "my/path");
   * fsw_set_callback(handle, my_callback);
   * 
   * fsw_start_monitor(handle);
   * +------------------------------------------------------------------------+
   * 
   * A suitable callback function is a function pointer of type
   * FSW_CEVENT_CALLBACK, that is it is a function conforming with the
   * following signature:
   * 
   * void c_process_events(fsw_cevent const * const * const events,
   *                       const unsigned int event_num);
   * 
   * When a monitor receives change events satisfying all the session criteria,
   * the callback is invoked and passed a copy of the events.
   */

  /*
   * This function creates a new monitor session using the specified monitor
   * and returns an handle to it.  This function is the libfswatch API entry
   * point.
   */
  FSW_HANDLE fsw_init_session(const fsw_monitor_type type = system_default_monitor_type);

  /*
   * Adds a path to watch to the specified session.  At least one path must be
   * added to the current session in order for it to be valid.
   */
  FSW_STATUS fsw_add_path(const FSW_HANDLE handle, const char * path);

  /*
   * Sets the callback the monitor invokes when some events are received.  The
   * callback must be set in the current session in order for it to be valid.
   */
  FSW_STATUS fsw_set_callback(const FSW_HANDLE handle,
                              const FSW_CEVENT_CALLBACK callback);
  FSW_STATUS fsw_set_latency(const FSW_HANDLE handle, const double latency);
  FSW_STATUS fsw_set_recursive(const FSW_HANDLE handle, const bool recursive);
  FSW_STATUS fsw_set_follow_symlinks(const FSW_HANDLE handle,
                                     const bool follow_symlinks);
  FSW_STATUS fsw_add_filter(const FSW_HANDLE handle,
                            const fsw_cmonitor_filter filter);
  FSW_STATUS fsw_start_monitor(const FSW_HANDLE handle);
  FSW_STATUS fsw_destroy_session(const FSW_HANDLE handle);
  FSW_STATUS fsw_set_last_error(const int error);
  FSW_STATUS fsw_last_error();
  // TODO: implement function to signal a monitor to stop.
  bool fsw_is_verbose();

#  ifdef __cplusplus
}
#  endif

#endif /* LIBFSW_H */