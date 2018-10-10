gui_set_background(10, 10, 10);
gui_elements_nav_first_close_item("close_plasma_ui();");
gui_elements_nav_register_item("CNC", "open_plasma_ui();", "close_plasma_ui();");
gui_elements_nav_register_item("CAM", "open_cam_ui();", "close_cam_ui();");
gui_elements_nav();
open_plasma_ui();
