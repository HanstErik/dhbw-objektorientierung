#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <iostream>
#include <vector>

using namespace std;

const int grid_width = 32;
const int grid_size = 20;
const int window_size = grid_width * grid_size;
const int grid_square = grid_size * grid_size;

class Block
{
public:
	int x = 0, y = 0;

	Gosu::Image dirt = Gosu::Image::Image("Dirt.png");
	Gosu::Image gras = Gosu::Image::Image("Grasblock.png");

	Gosu::Image image = gras;

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
	bool bpos[grid_square];

	vector<Block> blocks;

	Level(bool pos[grid_square])
	{
		// Creating the vector "blocks" depending on the 10x10 array
		for (int i = 0; i < sizeof(bpos) / sizeof(bool); i++)
		{
			bpos[i] = pos[i];

			if (bpos[i])
			{
				blocks.push_back(Block());

				blocks.back().x = i * grid_width % window_size;
				blocks.back().y = floor(i / grid_size) * grid_width;

				if (i >= grid_size && bpos[i - grid_size])
				{
					blocks.back().image = blocks.back().dirt;
				}
			}
		}

	}


	Level(string pos)
	{
		// Creating the vector "blocks" depending on the 10x10 array
		for (int i = 0; i < sizeof(bpos) / sizeof(bool); i++)
		{
			bpos[i] = bool(pos[i] - 48);

			if (bpos[i])
			{
				blocks.push_back(Block());

				blocks.back().x = i * grid_width % window_size;
				blocks.back().y = floor(i / grid_size) * grid_width;

				if (i >= 10 && bpos[i - grid_size])
				{
					blocks.back().image = blocks.back().dirt;
				}
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
			if ((player_y >= int(plevel.blocks.at(i).y - image.height()) && player_y <= int(plevel.blocks.at(i).y) && player_x > int(plevel.blocks.at(i).x - image.width()) && player_x < int(plevel.blocks.at(i).x + plevel.blocks.at(i).image.width())) || player_y >= window_size - grid_width)
			{
				if (player_y >= window_size - grid_width)
				{
					player_y = window_size - grid_width;
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
	Player player_1 = Player(0, window_size - grid_width);


	/* Test Level 10x10
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

	Test Level 20x20
					  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


	Test Level 10x10 String
	"0000000000"
	"0000000000"
	"0000000000"
	"0000000000"
	"0000000000"
	"0000000000"
	"0000000000"
	"0000000000"
	"0000000000"
	"0000000000"

	Test Level 20x20 String
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"
	"00000000000000000000"

	*/

	// Levelcreation. Array of 100 meaning a 10x10 grid in which you can place blocks

	// 10x10 Levels
	/*
	bool pos_1[100] = {0,0,0,1,1,0,0,0,0,0,
						0,0,0,0,0,1,0,0,0,0,
						0,0,0,0,0,0,0,0,1,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,1,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,1,0,0,
						0,0,0,0,1,0,0,1,0,0,
						0,1,0,0,1,1,1,1,1,1,
						0,1,0,0,1,1,1,1,1,1 };

	Level level_1 = Level(pos_1);

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

	Level level_2 = Level(pos_2);
	*/
	// 20x20 Levels

	bool pos_1[400] = { 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,
						0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,
						1,1,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,
						1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,1,
						1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,1,1,
						1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,
						1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,
						1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
						1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,
						1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,
						1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,
						1,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1,
						1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,
						1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,
						1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,
						1,1,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,
						0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

	Level level_1 = Level(pos_1);

	/*bool pos_2[400] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 };*/

	string pos_2 =
		"00000000100000000000"
		"11110000000000000000"
		"00000010000000000000"
		"00000000100000010000"
		"00000000000000000000"
		"00000000001001000000"
		"00000000000000010010"
		"00000000000000000000"
		"00000000000000001001"
		"00000000000000000000"
		"00000000000000000010"
		"00000000001001001000"
		"00001110000000000000"
		"00000000100000000000"
		"00000000001001000000"
		"00000000000000000000"
		"00000000010000001110"
		"00000000000001000000"
		"00000001001000000000"
		"00001000000000000000";

	Level level_2 = Level(pos_2);

	// 10x10 with Strings

	/*
		string pos_1 =
			"0000000000"
			"0010000000"
			"1000000000"
			"1000000000"
			"1110000000"
			"0000000000"
			"0000111000"
			"0000000001"
			"0000000001"
			"0000011111";

		Level level_1 = Level(pos_1);

		string pos_2 =
			"0000000000"
			"1000000000"
			"0100000000"
			"0011001000"
			"0000000000"
			"0000000100"
			"0000000000"
			"0000100001"
			"0000100001"
			"0001111111";

	Level level_2 = Level(pos_2);
	*/
	// Stages contains the different levels
	Level stages[2] = { level_1, level_2 };
	int stage = 0;
	const int num_of_stages = 2;

	GameWindow()
		: Window(window_size, window_size)
	{
		set_caption("Cooles Spiel");
	}

	Gosu::Font font = Gosu::Font::Font(window_size / grid_size * 2, "Georgia");

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

		if (player_1.player_y <= 0 && player_1.collision_ud(stages[stage]) == 'd' && stage >= num_of_stages - 1)
		{
			font.draw_text_rel("Victory", window_size / 2, grid_width * grid_size / 4, 10, 0.5, 0.5, 1, 1, Gosu::Color::Color(0xFFF5D525));
		}

	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		// Inputs
		if ((Gosu::Input::down(Gosu::Button::KB_RIGHT) || Gosu::Input::down(Gosu::Button::KB_D)) && player_1.collision_rl(stages[stage]) != 'r')
		{
			player_1.right();
		}

		if ((Gosu::Input::down(Gosu::Button::KB_LEFT) || Gosu::Input::down(Gosu::Button::KB_A)) && player_1.collision_rl(stages[stage]) != 'l')
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

		// Set Stage -int(player_1.image.height())
		if (player_1.player_y <= 0 && player_1.collision_ud(stages[stage]) == 'd' && stage < num_of_stages - 1)
		{

			player_1.player_x = 0;
			player_1.player_y = window_size - grid_width;

			stage++;
		}

		//cout << player_1.collision_ud(l1);
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
