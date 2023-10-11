#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class GameWindow : public Gosu::Window
{
public:
	float player_x = 100, player_y = 300;
	float g = 5;
	float dy = -10;

	GameWindow()
		: Window(800, 600)
	{
		set_caption("Gosu Tutorial mit Git");
	}

	struct Player
	{
		Gosu::Image bild = Gosu::Image::Image("Bitmap.bmp");
		float x = 50, y = 50;

	};

	Player p1;

	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		// Hintergrund
		graphics().draw_rect(
			0, 0, 
			width(), height(),
			Gosu::Color::Color(0xFF30BBFF),
			0
		);

		// Player
		/*graphics().draw_rect(
			player_x, player_y,
			20, 30,
			Gosu::Color::BLACK,
			5
		);*/

		/*Gosu::Image::Image("Bitmap.bmp").draw(
			player_x, player_y,
			5
		);*/

		p1.bild.draw(
			p1.x, p1.y,
			5
		);
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (dy < 0)
		{
			dy += 0.25;
		}
		
		p1.x += 0.5;
		if (p1.y <= 200)
		{
			p1.y += g + dy;
		}
		
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
