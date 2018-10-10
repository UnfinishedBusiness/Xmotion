# Navifation Bar
	- CNC Nav Button
		* There should be a general config option in the ini that will specify a section for the CNC Nav to be created from.
		* nav_cnc = plasma_ui | plasma_ui is a config section that will define all the elements and their individual options that create that "UI". This way users can easily customize the UI without
		having to hard code.
		* CAD & CAM Nav options are hard coded other than an config option to hide them.
	- CAD
		* Implement JetCad. JetCad's core engine will need to be heavily modified and current SVG rendering (for browser) calls will need to be switched to native application calls. JetCad Tools will be the same Javascript
		file that is being served on the cloud-based system so as to not have to maintain multiple released. The idea is to have the GUI pull for updates periodically... Current Class implementation that is being used in the cloud
		needs to be changed to RequireJS module export style because the DukTape VM does not seem to work with Javascript Classes.
	- CAM
		* The only reason JetCad's CAM cababilities are not finished is because client-side Javascript just doesn't seem to have the muscle to deal with complicated toolpaths quickly
		with the current geometry api, most of this is becuase Maker.js's offsetting algorythm is really slow.
		 I believe the fix for this is to use the Clipper Library, create Javascript Binding interface to clipper.
		* I want the CAM to work pretty close to the same way as sheetcam but simpler. When a DXF drawing is opened, itterate all the entities and create an array with every closed contour's points. Ideally
		this will all be done in C/C++ code but will fallback to exporting it off to MakerJS in javascript code if I can't seem to get an efficient chaining algorythm working (Struggled on this
		3 years ago while developing ProfileCAD).
		* After All the closed contours are in an array, single click a contour chain and click "i" on keyboard for inside offset, "o" for outside offset, and "n" for no-cut, "c" for cut on center
		* Click New Operation, window opens. Specify inside, outside, center offset, feedrate, and tool diameter (Tool Library will be down the road, for now we will start simple)
		* Post Gcode. Post Processor will be a Javascript file and the output will be posted to the "post_directory" ini setting

# Backend
	- Add F3 hotkey to edit TODO.md
	- Add F4 hottkeyy to edit BUGS.md
# Open File Dialog
	- Add toolbar button for mounting USB drive
	- Add toolbar button for un-mounting USB drive

# LinuxCNC/MachineKit/Motion Control
	- Figure out how to deal with NML Message differences between 2.6 and 2.7
		* Should be able to use a VERSION define shared includes
	- Integrate BeagleG motion controller so Xmotion on the Xmotion Main Board (1Ghz ARM + 2 PRU units) is a fully integrated motion controller (LinuxCNC/MachineKit not needed)
	- Jog Step Controls need to be implemented
