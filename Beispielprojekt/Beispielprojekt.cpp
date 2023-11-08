#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <iostream>
#include <vector>

using namespace std;

class Block
{
public:
	int x = 0, y = 0;

	Gosu::Image image = Gosu::Image::Image("grasblock.png");

	Block()
	{

	}

	Block(int px, int py)
	{
		x = px;
		y = py;
	}

	// Displaying a block
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
	bool bpos[100];

	vector<Block> blocks;

	Level(int pstage, bool pos[100])
	{	
		// Creating the vector "blocks" depending on the 10x10 array
		for (int i = 0; i < sizeof(bpos) / sizeof(bool); i++)
		{
			bpos[i] = pos[i];

			if (bpos[i])
			{
				blocks.push_back(Block());

				blocks.back().x = i * 32 % 320;
				blocks.back().y = floor(i / 10) * 32;
			}
		}

	}

	// Displaying every block of the level
	void draw()
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks.at(i).draw();
		}
	}
};

class Player
{
public:
	int player_x = 0, player_y = 0;

	// Gravity and jumping/falling variable
	double g = 4;
	double dy = 0;


	// Images for different movements and orientations
	Gosu::Image image_r = Gosu::Image::Image("Right.png");
	Gosu::Image image_rjump = Gosu::Image::Image("Rightjmp.png");
	Gosu::Image image_l = Gosu::Image::Image("Left.png");
	Gosu::Image image_ljump = Gosu::Image::Image("Leftjmp.png");

	// Current image
	Gosu::Image image;

	// 0 for right 1 for left
	bool facing;

	Player(int px, int py)
	{
		facing = 0;
		player_x = px;
		player_y = py;
		image = image_r;
	}

	// Movement
	void right()
	{
		facing = 0;
		player_x += 2;
	}

	void left()
	{
		facing = 1;
		player_x -= 2;
	}

	void jump()
	{
		dy = -12;
	}

	// Displaying the player
	void draw()
	{
		image.draw(
			player_x, player_y,
			5
		);
	}

	char collision_rl(Level plevel)
	{
		// Check every block of the current level
		for (int i = 0; i < plevel.blocks.size(); i++)
		{
			// Check for collision right
			if (player_x >= int(plevel.blocks.at(i).x - image.width()) && player_x <= int(plevel.blocks.at(i).x) && player_y > int(plevel.blocks.at(i).y - image.height()) && player_y < int(plevel.blocks.at(i).y + plevel.blocks.at(i).image.height()))
			{
				return 'r';
			}

			// Check for collision left
			if (player_x <= int(plevel.blocks.at(i).x + plevel.blocks.at(i).image.width()) && player_x >= int(plevel.blocks.at(i).x) && player_y > int(plevel.blocks.at(i).y - image.height()) && player_y < int(plevel.blocks.at(i).y + plevel.blocks.at(i).image.height()))
			{
				return 'l';
			}
		}


	}

	char collision_ud(Level plevel)
	{
		// Check every block of the current level
		for (int i = 0; i < plevel.blocks.size(); i++)
		{
			// Check for collision down
			if ((player_y >= int(plevel.blocks.at(i).y - image.height()) && player_y <= int(plevel.blocks.at(i).y) && player_x > int(plevel.blocks.at(i).x - image.width()) && player_x < int(plevel.blocks.at(i).x + plevel.blocks.at(i).image.width())) || player_y >= 288)
			{
				if (player_y >= 288)
				{
					player_y = 288;
				}
				else
				{
					player_y = plevel.blocks.at(i).y - image.height();
				}

				// Set ground image
				if (facing)
				{
					image = image_l;
				}
				else
				{
					image = image_r;
				}

				return 'd';
			}
			// Set jumping image
			else
			{
				if (facing)
				{
					image = image_ljump;
				}
				else
				{
					image = image_rjump;
				}
			}

			// Check for collision up
			if ((player_y < int(plevel.blocks.at(i).y + plevel.blocks.at(i).image.height()) && player_y >= int(plevel.blocks.at(i).y) && player_x > int(plevel.blocks.at(i).x - image.width()) && player_x < int(plevel.blocks.at(i).x + plevel.blocks.at(i).image.width())) && dy != -g)
			{
				player_y = int(plevel.blocks.at(i).y + plevel.blocks.at(i).image.height());

				dy = -g + 0.5;

				return 'u';
			}
		}
	}
};

class GameWindow : public Gosu::Window
{
public:
	// Initaialize player at x = 0, y = 288 (bottom left)
	Player player_1 = Player(0, 288);


	/* Test Level
					  { 0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0 };
	*/

	// Levelcreation. Array of 100 meaning a 10x10 grid in which you can place blocks

	bool pos_1[100] = { 0,0,0,1,1,0,0,0,0,0,
						0,0,0,0,0,1,0,0,0,0,
						0,0,0,0,0,0,0,0,1,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,1,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,1,0,0,
						0,0,0,0,1,0,0,1,0,0,
						0,1,0,0,1,1,1,1,1,1,
						0,1,0,0,1,1,1,1,1,1 };

	Level level_1 = Level(0, pos_1);

	bool pos_2[100] = { 0,0,0,0,1,0,0,0,0,0,
						1,1,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,1,0,0,0,0,0,0,0,
						0,0,0,0,0,1,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,1,0,0,0,0,0,
						0,0,1,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,1,0,0,0,0,0,0,0,0 };

	Level level_2 = Level(0, pos_2);

	// Stages contains the different levels
	Level stages[2] = { level_1 , level_2 };
	int stage = 0;
	const int num_of_stages = 2;

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

		player_1.draw();
		stages[stage].draw();

		/*for (int i = 0; i < l1.blocks.size(); i++)
		{
			cout << l1.blocks.at(i).x << " ";
			cout << l1.blocks.at(i).y << endl;
		}

		cout << endl;

		if (4 > -10)
		{
			cout << "lost";
		}*/
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		// Inputs
		if (Gosu::Input::down(Gosu::Button::KB_RIGHT) && player_1.collision_rl(stages[stage]) != 'r')
		{
			player_1.right();
		}

		if (Gosu::Input::down(Gosu::Button::KB_LEFT) && player_1.collision_rl(stages[stage]) != 'l')
		{
			player_1.left();
		}

		if (Gosu::Input::down(Gosu::Button::KB_SPACE) && player_1.collision_ud(stages[stage]) == 'd')
		{
			player_1.jump();
			player_1.player_y += player_1.g + player_1.dy;
		}

		// Gravity
		if (player_1.dy < 0)
		{
			player_1.dy += 0.5;
		}

		if (player_1.collision_ud(stages[stage]) != 'd')
		{
			player_1.player_y += player_1.g + player_1.dy;
		}

		// Set Stage
		if (player_1.player_y <= -int(player_1.image.height()) && player_1.collision_ud(stages[stage]) == 'd' && stage < num_of_stages - 1)
		{
			player_1.player_x = 0;
			player_1.player_y = 288;

			stage++;
		}

		/*
		if (Gosu::Input::down(Gosu::Button::KB_UP) && player_.collision_ud(level_1) != 'u')
		{
			player_1.up();
		}

		if (Gosu::Input::down(Gosu::Button::KB_DOWN) && player_1.collision_ud(level_1) != 'd')
		{
			player_1.down();
		}*/

		//cout << player_1.collision_ud(l1);
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
