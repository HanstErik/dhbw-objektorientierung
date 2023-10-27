#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class Block
{
public:
	int x = 0, y = 0;

	Gosu::Image image = Gosu::Image::Image("Block.png");

	Block()
	{

	}

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
	int stage;

	bool bpos[100];

	Block blocks[100];

	Level(int pstage, bool pos[100])
	{
		stage = pstage;

		for (int i = 0; i < sizeof(bpos) / sizeof(bool); i++)
		{
			bpos[i] = pos[i];
		}
		
	}
	
	void draw()
	{
		for (int i = 0; i < sizeof(bpos) / sizeof(bool); i++)
		{
			if (bpos[i])
			{
				blocks[i].x = i * 32 % 320;
				blocks[i].y = floor(i / 10) * 32;

				blocks[i].draw();
			}
		}
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

	double g = 4;
	double dy = 0;


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
		player_x += 2;
	}

	void left()
	{
		facing = 1;
		image = l;
		player_x -= 2;
	}

	void jump()
	{
		if (facing)
		{
			image = ljump;
		}
		else
		{
			image = rjump;
		}

		dy = -12;
	}

	void up()
	{
		if (facing)
		{
			image = ljump;
		}
		else
		{
			image = rjump;
		}

		player_y -= 2;
	}

	void down()
	{
		player_y += 2;
	}

	void draw()
	{
		image.draw(
			player_x, player_y,
			5
		);
	}

	// Wahrscheinlich mit Level statt Block
	char collision_rl(Level l)
	{
		for (int i = 0; i < sizeof(l.blocks) / sizeof(Block); i++)
		{
			if (player_x >= l.blocks[i].x - image.width() && player_x <= l.blocks[i].x && player_y > l.blocks[i].y - image.height() && player_y < l.blocks[i].y + l.blocks[i].image.height())
			{
				//player_x = l.blocks[i].x - image.width();
				return 'r';
			}

			if (player_x <= l.blocks[i].x + l.blocks[i].image.width() && player_x >= l.blocks[i].x && player_y > l.blocks[i].y - image.height() && player_y < l.blocks[i].y + l.blocks[i].image.height())
			{
				//player_x = l.blocks[i].x + l.blocks[i].image.width();
				return 'l';
			}
		}

		
	}

	char collision_ud(Level l)
	{
		for (int i = 0; i < sizeof(l.blocks) / sizeof(Block); i++)
		{
			if (player_y >= l.blocks[i].y - image.height() && player_y <= l.blocks[i].y && player_x > l.blocks[i].x - image.width() && player_x < l.blocks[i].x + l.blocks[i].image.width())
			{
				player_y = l.blocks[i].y - image.height();
				return 'd';
			}
		}
	}
};

class GameWindow : public Gosu::Window
{
public:
	Player p1 = Player(240, 150);

	bool pos[100] = {0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
				   	 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,1,0,0,0,
					 0,0,0,0,0,0,0,0,1,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,1,0,0,
					 1,1,1,1,1,1,1,1,1,1};

	Level l1 = Level(0, pos);

	GameWindow()
		: Window(320, 320)
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
			Gosu::Color::Color(0xFF87CEFA),
			0
		);

		p1.draw();
		l1.draw();
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (Gosu::Input::down(Gosu::Button::KB_RIGHT) && p1.collision_rl(l1) != 'r')
		{
			p1.right();
		}

		if (Gosu::Input::down(Gosu::Button::KB_LEFT) && p1.collision_rl(l1) != 'l')
		{
			p1.left();
		}

		if (Gosu::Input::down(Gosu::Button::KB_SPACE) && p1.collision_ud(l1) == 'd')
		{
			p1.jump();
			p1.player_y += p1.g + p1.dy;
		}

		if (p1.dy < 0)
		{
			p1.dy += 0.5;
		}
		
		if (p1.collision_ud(l1) != 'd')
		{
			p1.player_y += p1.g + p1.dy;
		}
		
		if (Gosu::Input::down(Gosu::Button::KB_UP) && p1.collision_ud(l1) != 'u')
		{
			p1.up();
		}

		if (Gosu::Input::down(Gosu::Button::KB_DOWN) && p1.collision_ud(l1) != 'd')
		{
			p1.down();
		}
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
