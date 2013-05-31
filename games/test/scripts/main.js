function game() {
	var color = CreateColor(255, 255, 255, 255);
	var font = GetSystemFont();
	var x = 0;
	var y = 0;
	var dx = 1;
	var dy = 1;
	var screen_width = GetScreenWidth();
	var screen_height = GetScreenHeight();

	while (true) {
		Rectangle(x, y, 10, 10, color);
		font.drawText(10, 10, "Test!");
		FlipScreen();
		if (x == screen_width - 10 || x == -1) {
			dx *= -1;
		}
		if (y == screen_height - 10 || y == -1) {
			dy *= -1;
		}
		
		x += dx;
		y += dy;
	}
}