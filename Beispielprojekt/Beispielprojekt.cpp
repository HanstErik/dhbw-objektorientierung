#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class GameWindow : public Gosu::Window
{
public:
	float player_x = 100, player_y = 300;

	GameWindow()
		: Window(800, 600)
	{
		set_caption("Gosu Tutorial mit Git");
	}

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
		graphics().draw_rect(
			player_x, player_y,
			20, 30,
			Gosu::Color::BLACK,
			5
		);

	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		player_x += 1;
		player_y += 0.5;
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
