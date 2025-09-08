/*
 * my_plugin_periodic.c - skeleton plug-in periodic function
 *
 * Copyright (c) <current-year> <your-organization>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vlib/vlib.h>
#include <vppinfra/error.h>
#include <my_plugin/my_plugin.h>

static void
handle_event1 (my_plugin_main_t *pm, f64 now, uword event_data)
{
  clib_warning ("received MY_PLUGIN_EVENT1");
}

static void
handle_event2 (my_plugin_main_t *pm, f64 now, uword event_data)
{
  clib_warning ("received MY_PLUGIN_EVENT2");
}

static void
handle_periodic_enable_disable (my_plugin_main_t *pm, f64 now, uword event_data)
{
   clib_warning ("Periodic timeouts now %s",
     event_data ? "enabled" : "disabled");
   pm->periodic_timer_enabled = event_data;
}

static void
handle_timeout (my_plugin_main_t *pm, f64 now)
{
  clib_warning ("timeout at %.2f", now);
}

static uword
my_plugin_periodic_process (vlib_main_t * vm,
	                  vlib_node_runtime_t * rt, vlib_frame_t * f)
{
  my_plugin_main_t *pm = &my_plugin_main;
  f64 now;
  f64 timeout = 10.0;
  uword *event_data = 0;
  uword event_type;
  int i;

  while (1)
    {
      if (pm->periodic_timer_enabled)
        vlib_process_wait_for_event_or_clock (vm, timeout);
      else
        vlib_process_wait_for_event (vm);

      now = vlib_time_now (vm);

      event_type = vlib_process_get_events (vm, (uword **) & event_data);

      switch (event_type)
	{
	  /* Handle MY_PLUGIN_EVENT1 */
	case MY_PLUGIN_EVENT1:
	  for (i = 0; i < vec_len (event_data); i++)
	    handle_event1 (pm, now, event_data[i]);
	  break;

	  /* Handle MY_PLUGIN_EVENT2 */
	case MY_PLUGIN_EVENT2:
	  for (i = 0; i < vec_len (event_data); i++)
	    handle_event2 (pm, now, event_data[i]);
	  break;
          /* Handle the periodic timer on/off event */
	case MY_PLUGIN_EVENT_PERIODIC_ENABLE_DISABLE:
	  for (i = 0; i < vec_len (event_data); i++)
	    handle_periodic_enable_disable (pm, now, event_data[i]);
	  break;

          /* Handle periodic timeouts */
	case ~0:
	  handle_timeout (pm, now);
	  break;
	}
      vec_reset_length (event_data);
    }
  return 0;			/* or not */
}

void my_plugin_create_periodic_process (my_plugin_main_t *mmp)
{
  /* Already created the process node? */
  if (mmp->periodic_node_index > 0)
    return;

  /* No, create it now and make a note of the node index */
  mmp->periodic_node_index = vlib_process_create (mmp->vlib_main,
    "my_plugin-periodic-process",
    my_plugin_periodic_process, 16 /* log2_n_stack_bytes */);
}

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */
