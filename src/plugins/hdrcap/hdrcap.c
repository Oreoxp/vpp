

VLIB_

VLIB_PLUGIN_REGISTER() = {
  .name = "hdrcap",
  .description = "hdrcap",
  .version = "1.0",
}


static clib_error_t *
hdrcap_init (vlib_main_t *vm)
{
  hdrcap_main.vnet_main = vnet_get_main ();
  return 0;
}

VLIB_INIT_FUNCTION (hdrcap_init);

VNET_FEATURE_INIT (hdrcap, hdrcap_main)
{
  .arc_name = "ip4-unicast", .node_name = "hdrcap",
  .runs_before = VNET_FEATURES ("ip4-lookup"),
}