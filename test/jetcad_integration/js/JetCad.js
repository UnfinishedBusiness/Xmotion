var JetCad;
(function (JetCad) {
    var View = {};
    var Stack = [];
    var ToolPaths = {};
    var UndoStack = [];
    var CopiedIndex = [];
    var Model = {};
    var StaticStack = ""; //This stays as SVG Code, no array
    var LiveStack = [];
    var HotKeys = [];
    var SearchTools = [];
    var ModifierKeys = {};
    var Callbacks = {};
    Callbacks.Escape = function(){};
    Callbacks.onTab = function(){};
    Callbacks.onMouse = function(){};
    var Mouse = { x: 0, y: 0 };
    var ClientMouse = { x: 0, y: 0 };
    var DrawSpace;
    var DrawingName = "default";
    var DrawPoint;
    var MakerJS = require('makerjs');
    var AlreadyAttached = false;
    var Debug = false;
    var DisableHotkeys = false;
    var DoubleClickSeconds = Number(new Date().getTime() / 1000);
    var AutoSaveFlag = false;
    var NextClick = null;
    var ToolActive = null;
    var SnappedMouse = null;
    var ToolPoints = [];
    var isMouseDown = false;
    var HandlersAlreadyAttached = false;
    var HandlersActive = false;
    var StackChanged = true;
    function init()
    {
      //$("#Content").html("<svg style='width: " + ($(window).width() - 100) + "px; height: " + ($(window).height() - 100) + "px;' id='draw'><g id='draw-group'></g></svg><div id='draw-info' style='position: absolute; bottom: 20px; right: 40px; font-weight: bold;'></div><input id='draw-search' size='35' style='position: absolute; bottom: 20px; left: 80px; font-weight: bold;'></input>");
      DrawSpace = "";
      DrawPoint = "";
  		View.last_zoom_time = 0;
  		View.pan = {};
  		View.pan.x = 600;
  		View.pan.y = -400;
  		View.zoom = 1;
  		View.padding = {};
  		View.padding.x = 0;
  		View.padding.y = 0;
  		View.zoom_sensititivy = 0.150;
  		View.min_zoom = 0.1;
  		View.max_zoom = 40000;
  		View.MouseOverPadding = 10;
  		View.MouseOverRadius = 4;
  		View.line_width = 1.5;
  		View.ZoomFitPadding = 0.1;
      live_entity_cutoff = 500;
  		View.BoundingBox = {};
  		var el = "";
  		var rect = "";
  		DrawPoint.x = rect.width;
  		DrawPoint.y = rect.height;
      var point = "";
  		View.BoundingBox.height = point.y;
  		View.BoundingBox.width = point.x;

  		ModifierKeys.Shift = false;
  		ModifierKeys.Ctrl = false;

      LatheMode = false;

  		Debug = true;
  		DragSelect = false;
  		DragBox = {};
  		isMouseDown = false;
  		HandlersActive = true;
    }
    JetCad.init = init;

    function RegisterUndo()
  	{
  		AutoSaveFlag = true;
  		UndoStack = JSON.parse(JSON.stringify(Stack));
  	}
    JetCad.RegisterUndo = RegisterUndo;

  	function CommitUndo()
  	{
  		AutoSaveFlag = true;
  		Stack = JSON.parse(JSON.stringify(UndoStack));
  		render();
  	}
    JetCad.CommitUndo = CommitUndo;

  	function log(text)
  	{
  		if (Debug === true)
  		{
  			console.log(text);
  		}
  	}
    JetCad.log = log;

    function ZoomIn()
  	{
  		View.zoom += (View.zoom_sensititivy * View.zoom);
  		if (View.zoom > View.max_zoom)
  		{
  			View.zoom = View.max_zoom;
  		}
  		//render();
  	}
    JetCad.ZoomIn = ZoomIn;

  	function ZoomOut()
  	{
  		View.zoom -= (View.zoom_sensititivy * View.zoom);
  		if (View.zoom < View.min_zoom)
  		{
  			View.zoom = View.min_zoom;
  		}
  		//render();
  	}
    JetCad.ZoomOut = ZoomOut;

    function UpdateZoom(wheel) //Called from C Code!
  	{
  		var old_zoom = View.zoom;

  		var direction = wheel;

  		if (direction > 0)
  		{
  			View.zoom += (View.zoom_sensititivy * View.zoom);
  			if (View.zoom > View.max_zoom)
  			{
  				View.zoom = View.max_zoom;
  			}
  		}
  		else
  		{
  			View.zoom -= (View.zoom_sensititivy * View.zoom);
  			if (View.zoom < View.min_zoom)
  			{
  				View.zoom = View.min_zoom;
  			}
  		}
  		var scalechange = View.zoom - old_zoom;
  		panX((Mouse.x * scalechange) * -1);
  		panY((Mouse.y * scalechange) * -1);
  		updateView();
  		View.last_zoom_time = Number(new Date().getTime() / 1000);
  		//render();
  		//log(View.zoom);
  	}
    JetCad.UpdateZoom = UpdateZoom;

    function UpdateMouse(x, y) //Called from C Code!
    {
  		if (!HandlersActive) return;
  		var snapped = false;
  		var snapped_point = {};
      var loc = cursorPoint(x, y);
      Mouse = { x:  loc.x, y: loc.y};
  		Mouse.x = Number(Mouse.x.toFixed(5));
  		Mouse.y = Number(Mouse.y.toFixed(5));

  		if (DragSelect && isMouseDown)
  		{
  			DragSelectMoved = true;
  			//Update selection box
  			//console.log("Update box");
  			var width = Mouse.x - ToolPoints[0].x;
  			var height = Mouse.y - ToolPoints[0].y;

  			var fill = "fill='#044B94' fill-opacity='0.4' stroke-width='" + View.line_width + "' vector-effect='non-scaling-stroke' ";

  			if (width < 0 && height < 0)
  			{
  				StaticStack = '<rect ' + fill + ' stroke="blue" x="' + Mouse.x + '" y="' + Mouse.y + '" width="' + width * -1 + '" height="' + height * -1 + '" />';
  				//console.log("1");
  				DragBox.TopLeft = { x: Mouse.x , y: Mouse.y + (height * -1) };
  				DragBox.BottomRight = { x: Mouse.x + (width * -1), y: Mouse.y };
  			}
  			else if (width > 0 && height > 0)
  			{
  				StaticStack = '<rect ' + fill + ' stroke="blue" x="' + ToolPoints[0].x + '" y="' + ToolPoints[0].y + '" width="' + width + '" height="' + height + '" />';
  				//console.log("2");
  				DragBox.TopLeft = { x: ToolPoints[0].x, y: ToolPoints[0].y + (height * -1) };
  				DragBox.BottomRight = { x: ToolPoints[0].x + (width * -1), y: ToolPoints[0].y };
  			}
  			else if (width > 0 && height < 0)
  			{
  				StaticStack = '<rect ' + fill + ' stroke="blue" x="' + ToolPoints[0].x + '" y="' + Mouse.y + '" width="' + width + '" height="' + height * -1 + '" />';
  				//console.log("3");
  				DragBox.TopLeft = { x: ToolPoints[0].x, y: Mouse.y + (height * -1) };
  				DragBox.BottomRight = { x: ToolPoints[0].x + (width), y: Mouse.y };
  			}
  			else if (width < 0 && height > 0)
  			{
  				StaticStack = '<rect ' + fill + ' stroke="blue" x="' + Mouse.x + '" y="' + ToolPoints[0].y + '" width="' + width * -1 + '" height="' + height + '" />';
  				DragBox.TopLeft = { x: Mouse.x, y: ToolPoints[0].y + (height) };
  				DragBox.BottomRight = { x: Mouse.x + (width * -1), y: ToolPoints[0].y };
  				//console.log("4");
  			}
  		}
  		SnappedMouse = Mouse; //Zero Entity bug, this was the issue...

      if (DragSelect === false && Callbacks.onMouse === null && LiveStack.length === 0 && isMouseDown)
  		{
  			//console.log("Begin Drag Select!");
  			DragSelect = true;
  			ToolPoints[0] = SnappedMouse; //Drag Select start point
  		}

  		for (var x = 0; x < Stack.length; x++)
  		{
  			if (Stack[x].meta === undefined)
  			{
  				break;
  			}
  			if (Stack[x].meta.SnapPoints !== undefined)
  			{
  				if (Stack[x].meta.SnapPoints.length > 0)
  				{
  					for (var i = 0; i < Stack[x].meta.SnapPoints.length; i++)
  					{
  						//Check if Snap Point is close to mouse point and move curser box to point
  						if(distance(Mouse,  Stack[x].meta.SnapPoints[i]) < (View.MouseOverPadding / View.zoom) )
  						{
  							snapped = true;
  							snapped_point = Stack[x].meta.SnapPoints[i];
                if (LatheMode === true) SetStatusText("D: " + (snapped_point.y * 2).toFixed(4) + " , Z: " + snapped_point.x.toFixed(4) );
                break;
  						}
  					}
  				}
  			}
  		}
  		if(distance(Mouse,  { x: 0, y: 0 }) < (View.MouseOverPadding / View.zoom) )
  		{
  				//Always Snap to origin!
  				snapped = true;
  				snapped_point = { x: 0, y: 0 };
  		}
  		if (snapped) SnappedMouse = snapped_point;
  		if (Callbacks.onMouse != null) (Callbacks.onMouse)();
  		render(true);
  		if (Stack.length < live_entity_cutoff) //We need to change this to a time measurement!
  		{
  			render(false);
  		}

    }

})(JetCad || (JetCad = {}));
module.exports = JetCad;
