function open_plasma_ui()
{
  print("Opening plasma ui!");
  gui_elements_dro();
  gui_elements_controls();
  gui_elements_indicators();
  gui_elements_viewer();
}
function close_plasma_ui()
{
  print("Closing plasma ui!");
  gui_elements_dro_close();
  gui_elements_controls_close();
  gui_elements_indicators_close();
  gui_elements_viewer_close();
}
