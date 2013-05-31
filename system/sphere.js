/*global sphere*/

(function(global, _sphere) {
	var required_scripts = {},
		screen,
		screen_width,
		screen_height,
		log = sphere.engine.log,
		black = new _sphere.graphics.Color(0, 0, 0, 255);

	log("Sphere interface starting up\n");

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
		return args.join(_sphere.fs.path_separator);
	}
	
	function int16val(ba) {
		return ba[0] | ba[1] << 8;
	}
	
	var SphereAPI = {
		RequireScript: function(path) {
			if (path in required_scripts) {
				return;
			}
			required_scripts[path] = true;
			
			return SphereAPI.EvaluateScript(pathjoin(_sphere.game.path, 'scripts', path));
		},
		EvaluateScript: function(path) {
			var file = _sphere.fs.readTextFile(path);
			return _sphere.engine.evalScript(file, path);
		},
		Abort: _sphere.engine.abort,
		RestartGame: _sphere.engine.restartGame,
		Exit: _sphere.engine.exit,
		OpenFile: function(path) {
			return new ConfigFile(path);
		},
		FlipScreen: function() {
			_sphere.graphics.flipScreen();
			screen.rectangle(0, 0, screen_width, screen_height, black);
			_sphere.events.poll();
		},
		Rectangle: function(x, y, w, h, c) {
			return screen.rectangle(x, y, w, h, c);
		},
		CreateColor: function(r, g, b, a) {
			if (a === undefined) {
				a = 255;
			}
			return new _sphere.graphics.Color(r, g, b, a);
		},
		GetScreenWidth: function() {
			return screen_width;
		},
		GetScreenHeight: function() {
			return screen_height;
		},
		GetSystemFont: function() {
			return new Font(pathjoin('system', engine_configuration.read('Font')));
		},
		LoadFont: function(path) {
			return new Font(pathjoin('fonts', path));
		},
		OpenRawFile: function(path, mode) {
			return new _sphere.fs.File(path, mode);
		}
	};
	
	
	function Font(path) {
		this.file = new _sphere.fs.File(path);
	
		var signature = this.file.read(4).toString();
		var version = int16val(this.file.read(2));
		var num_glyphs = int16val(this.file.read(2));
	
		this.file.read(248);	// reserved.
	
		this.glyphs = [];
	
		for (var i = 0; i < num_glyphs; i++) {
			var width = int16val(this.file.read(2));
			var height = int16val(this.file.read(2));
	
			this.file.read(28);	// reserved.
	
			if (version != 2) {
				_sphere.engine.abort("Expected version 2 rfn");
			}
			else {
				var bytes = width * height * 4;
				this.glyphs.push({
					width: width,
					height: height,
					surface: this.file.read(bytes).toSurface(width, height)
				});
			}
		}
	}
	
	Font.prototype.drawText = function(x, y, string) {
		for (var i = 0; i < string.length; i++) {
			var chr = string.charCodeAt(i);
			screen.blitSurface(this.glyphs[chr].surface, x, y);
			x += this.glyphs[chr].width;
		}
	};
	
	function ConfigFile(path) {
		var text = _sphere.fs.readTextFile(path);
		var lines = text.split(/[\r\n]+/);
	
		this.values = {};
	
		for (var i = 0; i < lines.length; i++) {
			if (lines[i].trim() == '') {
				continue;
			}
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
	
	Game.prototype.start = function() {
		screen = _sphere.graphics.setVideoMode(
			screen_width = _sphere.game.configuration.read('screen_width'),
			screen_height = _sphere.game.configuration.read('screen_height')
		);
	
		var legacy = parseInt(_sphere.game.configuration.read('api', 1)) === 1;

		_sphere.engine.setWindowTitle(_sphere.game.configuration.read('name'));
		
		var script_path = _sphere.game.configuration.read('script');
		var script = _sphere.fs.readTextFile(pathjoin(_sphere.game.path, 'scripts', script_path));
		
		log("Starting game\n");

		if (legacy) {
			log("Note: This is a classic Sphere game. New 'sphere' API will be unavailable.\n");
			log("Add api=2 to game.sgm to enable it. Add oldapi=0 to disable the emulation\n");
			log("layer.\n");
			delete sphere;
		}
		
		try {
			if (parseInt(_sphere.game.configuration.read('oldapi', 1)) === 1) {
				extend(global, SphereAPI);
			}
			_sphere.engine.evalScript(script, script_path);
			game();
		}
		catch (e) {
			log(e.stack ? e.stack : e.toString());
			log("\n");
		}

	};
	
	try {
		var engine_configuration = new ConfigFile("system/system.ini");

		if (_sphere.engine.argv.length > 1) {
			_sphere.game = new Game(_sphere.engine.argv[1]);
		}
		else {
			_sphere.game = new Game('startup');
		}
	
		_sphere.game.start();
	}
	catch (e) {
		log(e.stack ? e.stack : e.toString());
		log("\n");
	}

})(this, sphere);