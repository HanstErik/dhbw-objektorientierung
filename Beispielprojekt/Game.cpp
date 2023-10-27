#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class GameWindow : public Gosu::Window
{
public:
	GameWindow()
		:Window(600, 600)
	{
		set_caption("Game");
	}

	void draw() override
	{
		// Hintergrund
		graphics().draw_rect(
			0, 0,
			width(), height(),
			Gosu::Color::Color(0xFF3000FF),
			0
		);
	}

	void update() override
	{

	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}