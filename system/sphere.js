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
	
	screen = sphere.graphics.setVideoMode(320, 240);
	
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
		FlipScreen: sphere.graphics.flipScreen,
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
	var color2 = CreateColor(255, 0, 0, 128);
	while (true) {
		Rectangle(10, 10, 300, 220, color);
		screen.rectangle(20, 20, 280, 200, color2);
		FlipScreen();
	}
	
})(this);