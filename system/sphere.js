/*global sphere*/

(function(global) {
	var activeGame,
		required_scripts = {},
		screen,
		screen_width,
		screen_height,
		black = new sphere.graphics.Color(0, 0, 0, 255);

	function extend(target) {
		var sources = Array.prototype.slice.call(arguments, 1);
	
		for (var i = 0; i < sources.length; i++) {
			for (var p in sources[i]) {
				if (sources[i].hasOwnProperty(p)) {
					target[p] = sources[i][p];
				}
			}
		}
	
		return target;
	}
	
	function pathjoin() {
		var args = Array.prototype.slice.call(arguments, 0);
		return args.join(sphere.fs.path_separator);
	}
	
	var SphereAPI = {
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
			sphere.events.poll();
		},
		Rectangle: function(x, y, w, h, c) {
			return screen.rectangle(x, y, w, h, c);
		},
		CreateColor: function(r, g, b, a) {
			if (a === undefined) {
				a = 255;
			}
			return new sphere.graphics.Color(r, g, b, a);
		},
		GetScreenWidth: function() {
			return screen_width;
		},
		GetScreenHeight: function() {
			return screen_height;
		}
	};
	
	function ConfigFile(path) {
		var text = sphere.fs.readTextFile(path);
		var lines = text.split('\n');
	
		this.values = {};
	
		for (var i = 0; i < lines.length; i++) {
			var kvp = lines[i].split('=');
			this.values[kvp[0].trim()] = kvp[1].trim();
		}
	}
	
	ConfigFile.prototype.read = function(key, def) {
		if (key in this.values) {
			return this.values[key];
		}
		return def;
	};
	
	function Game(path) {
		this.path = path;
	
		this.configuration = new ConfigFile(pathjoin(path, 'game.sgm'));
	}

	if (sphere.engine.argv.length > 1) {
		activeGame = new Game(sphere.engine.argv[1]);
	}
	else {
		activeGame = new Game('startup');
	}

	screen = sphere.graphics.setVideoMode(
		screen_width = activeGame.configuration.read('screen_width'),
		screen_height = activeGame.configuration.read('screen_height')
	);
	
	sphere.engine.setWindowTitle(activeGame.configuration.read('name'));
	
	var script_path = activeGame.configuration.read('script');
	var script = sphere.fs.readTextFile(pathjoin(activeGame.path, 'scripts', script_path));
	
	// evalInContext doesn't expose this to us to call from here,
	// so append it to the script.
	script += "\ngame();";
	
	sphere.engine.evalInContext(script, SphereAPI, script_path);
})(this);