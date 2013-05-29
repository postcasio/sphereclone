(function(global) {
	var _extend = function(target) {
		var sources = Array.prototype.slice.call(arguments, 1);
		for (var i = 0; i < sources.length; i++) {
			for (var p in sources[i]) {
				if (sources[i].hasOwnProperty(p)) {
					target[p] = sources[i][p];
				}
			}
		}
	
		return target;
	};
	
	var required_scripts = {};
	var screen = null;
	var screen_width = 320;
	var screen_height = 240;
	var black = new sphere.graphics.Color(0, 0, 0, 0);
	
	screen = sphere.graphics.setVideoMode(screen_width, screen_height);
	
	_extend(global, {
		RequireScript: function(path) {
			if (path in required_scripts) {
				return;
			}
			required_scripts[path] = true;
			
			return EvaluateScript(path);
		},
		EvaluateScript: function(path) {
			var file = sphere.fs.readTextFile(path);
			return sphere.engine.evalScript(file, path);
		},
		Abort: sphere.engine.abort,
		RestartGame: sphere.engine.restartGame,
		Exit: sphere.engine.exit,
		FlipScreen: function() {
			sphere.graphics.flipScreen();
			screen.rectangle(0, 0, screen_width, screen_height, black);
		},
		Rectangle: function(x, y, w, h, c) {
			return screen.rectangle(x, y, w, h, c);
		},
		CreateColor: function(r, g, b, a) {
			if (a === undefined) {
				a = 255;
			}
			return new sphere.graphics.Color(r, g, b, a);
		}
	});
	
	var color = CreateColor(255, 255, 255, 255);
	var x = 0;
	var y = 0;
	var dx = 1;
	var dy = 1;
	
	while (true) {
		Rectangle(x, y, 10, 10, black);
		x += dx
		y += dy
		Rectangle(x, y, 10, 10, color);
		sphere.graphics.flipScreen();	// internal flipscreen does not automatically blank screen!
		sphere.events.poll();
		if (x == 311 || x == -1) {
			dx *= -1;
		}
		if (y == 231 || y == -1) {
			dy *= -1;
		}
	}
	
})(this);