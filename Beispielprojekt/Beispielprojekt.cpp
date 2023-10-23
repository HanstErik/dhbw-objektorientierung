#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class Block
{
public:
	int x = 0, y = 0;

	Gosu::Image image = Gosu::Image::Image("Block.png");

	Block(int px, int py)
	{
		x = px;
		y = py;
	}

	void draw()
	{
		image.draw(
			x, y,
			5
		);
	}
};

class Level
{
public:
	int stage = 0;

	Level()
	{
		
	}

	/*
	Erstellung von einzelnen Stages, bestehend aus mehreren Blöcken (Arrays?).
	Stage wechselt nach oben hin (Level ist sozusagen ein Turm)
	*/
	
};

class Player
{
public:
	int player_x = 0, player_y = 0;

	// Images for different movements and orientations
	Gosu::Image r = Gosu::Image::Image("Right.png");
	Gosu::Image rjump = Gosu::Image::Image("Right.png");
	Gosu::Image l = Gosu::Image::Image("Left.png");
	Gosu::Image ljump = Gosu::Image::Image("Left.png");

	// Current image
	Gosu::Image image;

	// 0 for right 1 for left
	bool facing;

	Player(int px, int py)
	{
		facing = 0;
		player_x = px;
		player_y = py;
		image = r;
	}

	void right()
	{
		facing = 0;
		image = r;
		player_x += 1;
	}

	void left()
	{
		facing = 1;
		image = l;
		player_x -= 1;
	}

	void draw()
	{
		image.draw(
			player_x, player_y,
			5
		);
	}

	// Wahrscheinlich mit Level statt Block
	char collision(Block b)
	{
		if (player_x >= b.x - image.width() && player_y > b.y - image.height() && player_y < b.y + b.image.height())
		{
			return 'r';
		}

		if (player_x <= b.x + b.image.width() && player_y > b.y - image.height() && player_y < b.y + b.image.height())
		{
			return 'l';
		}
	}
};

class GameWindow : public Gosu::Window
{
public:
	/*float player_x = 100, player_y = 300;
	float g = 5;
	float dy = -10;*/

	Player p1 = Player(50, 550);

	Block b1 = Block(200, 550);

	GameWindow()
		: Window(800, 600)
	{
		set_caption("Gosu Tutorial mit Git");
	}

	/*struct Player
	{
		Gosu::Image bild = Gosu::Image::Image("Image.png");
		float x = 50, y = 50;

	};*/

	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		// Hintergrund
		graphics().draw_rect(
			0, 0, 
			width(), height(),
			Gosu::Color::Color(0xFF30C0FF),
			0
		);

		p1.draw();
		b1.draw();
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (Gosu::Input::down(Gosu::Button::KB_RIGHT) && p1.collision(b1) != 'r')
		{
			p1.right();
		}
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
