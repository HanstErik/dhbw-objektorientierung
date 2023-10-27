#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class GameWindow : public Gosu::Window
{
public:
	float player_x = 100, player_y = 300;
	float g = 5;
	float dy = -10;
	bool space_d = false;

	GameWindow()
		: Window(1000, 1000)
	{
		set_caption("Gosu Tutorial mit Git");
	}

	struct Player
	{
		Gosu::Image bild = Gosu::Image::Image("Mario.png");
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
			Gosu::Color::Color(0xFF3000FF),
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
		if (p1.y <=  200 || Gosu::Input::down(Gosu::Button::KB_SPACE))
		{
			p1.y += g + dy;
		}

		if (dy < 0)
		{
			dy += 0.25;
		}

		if (Gosu::Input::down(Gosu::Button::KB_RIGHT))
		{
			p1.x += 2;
		}
		if (Gosu::Input::down(Gosu::Button::KB_LEFT))
		{
			p1.x -= 2;
		}
		if (Gosu::Input::down(Gosu::Button::KB_SPACE) && space_d == false)
		{

			dy = -10;
			
			space_d = true;
		}
		if (!Gosu::Input::down(Gosu::Button::KB_SPACE))
		{
			space_d = false;
		}
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
