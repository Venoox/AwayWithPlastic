#include "Texture.h"
#include <list>
#include <cstdlib> 
#include <ctime>
#include <cmath>
#include "Game.h"
#include "GameObject.h"
#include "Renderer.h"
#include <fstream>
#include "Highscore.h"
#include "Replay.h"
#include "Save.h"
#include "TextureManager.h"



int main(int argc, char* argv[])
{
	std::string name;
	
	
	srand(time(nullptr));
	Game game;
	if (game.init("Away With Plastic", 1280, 720, 64) != 0) return -1;

	TextureManager texturemanager;
	texturemanager.add("AwayWithPlastic/res/textures/sea.png");
	texturemanager.add("AwayWithPlastic/res/textures/land.png");
	texturemanager.add("AwayWithPlastic/res/textures/player_ship.png");
	texturemanager.add("AwayWithPlastic/res/textures/trash.png");
	texturemanager.add("AwayWithPlastic/res/textures/player.png");
	texturemanager.add("AwayWithPlastic/res/textures/pirate_ship.png");
	texturemanager.add("AwayWithPlastic/res/textures/pirate.png");
	texturemanager.add("AwayWithPlastic/res/textures/menu.png");
	texturemanager.add("AwayWithPlastic/res/textures/highscore.png");


	int map[10][16] = {
		1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,
		1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,
		1,1,1,1,2,1,1,1,1,1,1,2,2,2,2,1,
		1,1,1,1,2,2,2,1,1,1,1,2,2,2,2,1,
		1,1,1,1,2,2,2,1,1,1,1,2,2,2,2,1,
		1,1,1,1,2,2,2,1,1,1,1,2,2,2,2,1,
		1,1,1,1,2,2,2,1,1,1,1,2,2,2,2,1,
		1,1,1,1,2,2,2,1,1,1,1,2,2,2,2,1,
		1,1,1,1,2,2,2,1,1,1,1,1,2,2,1,1,
		1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1
	};

	int map2[10][16] = {
		1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,1,
		1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,1,
		1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,1,
		1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,1,
		1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,1,
		1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,
		1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,
		1,1,1,1,1,1,1,1,1,1,1,2,2,0,0,1,
		2,2,2,1,1,1,1,1,1,1,1,1,0,0,1,1,
		2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1
	};

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GameObject * player;
	GameObject * ship;
	std::list<GameObject*> enemy;
	std::list<GameObject*> trash;
	std::list<GameObject*> teammate;

	bool isReplaying = false;
	int difficulty;
	int points;
	GameObject tex(game, 1);
	bool menu = true;
	bool highscoremenu = false;

	SDL_Event event;
	Renderer renderer(game.GetWidth(), game.GetHeight());
	Replay replay;
	Save save;
	Highscore highscore;
	highscore.read();

	int n = 0;

	bool shouldCloseWindow = false;

	while (!shouldCloseWindow)
	{
		
		renderer.Clear();

		if (highscoremenu) {
			GameObject menu(game, 9, 0, 0, game.GetWidth(), game.GetHeight());
			renderer.BatchBegin();
			renderer.BatchDraw(menu);
			renderer.BatchEnd();
			renderer.Flush(menu.GetTex());
		}
		else if (menu) {
			GameObject menu(game, 8, 0, 0, game.GetWidth(), game.GetHeight());
			renderer.BatchBegin();
			renderer.BatchDraw(menu);
			renderer.BatchEnd();
			renderer.Flush(menu.GetTex());

			renderer.BatchBegin();
			renderer.BatchDrawString(450 - (name.size()*5), 450, name);
			renderer.BatchEnd();
			renderer.Flush(10);
		}
		else {

			if (isReplaying) {

				for (int t = 1; t < 3; t++) {
					renderer.BatchBegin();
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 16; j++)
						{
							tex.SetCoord(j * game.tile, i * game.tile);
							tex.SetTex(map[i][j]);
							if (tex.GetTex() == t) {
								renderer.BatchDraw(tex);
							}
							if (map[i][j] == 2) { //land
								if (player->Collision(tex)) {
									if (ship->hidden == true) {
										ship->SetCoord(player->GetX(), player->GetY());
										ship->hidden = false;
									}
									player->SetTex(5);
									bool sea = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if (player->Collision(tex2)) {
													sea = true;
												}
											}
										}
									}
									if (!sea && !player->Collision(*ship)) {
										player->locked = true;
									}
									if (player->Collision(*ship)) {
										player->locked = false;
									}
									if (sea && !player->Collision(*ship)) {
										player->locked2 = true;
									}


								}
							}
							if (map[i][j] == 1) { //sea
								if (player->Collision(tex)) {
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if (player->Collision(tex2)) {
													land = true;
												}
											}
										}
									}
									if (!land && player->Collision(*ship)) {
										ship->hidden = true;
										ship->SetCoord(-1000, -1000);
										player->locked2 = false;
									}
									player->SetTex(3);

								}
							}
						}
					}
					renderer.BatchEnd();
					renderer.Flush(t);
				}

				renderer.BatchBegin();
				renderer.BatchDraw(*ship);
				renderer.BatchEnd();
				renderer.Flush(ship->GetTex());

				replay.read();
				//std::cout << replay.getX() << " " << replay.getY() << std::endl;
				player->SetCoord(replay.getX(), replay.getY());
				renderer.BatchBegin();
				renderer.BatchDraw(*player);
				renderer.BatchEnd();
				renderer.Flush(player->GetTex());
				if (replay.isEOF()) {
					isReplaying = false;
					menu = true;
					replay.stop();
				}
			}
			else {

				if (trash.empty()) {
					difficulty++;
					if (difficulty == 3) {
						std::cout << "Win!" << std::endl;
						std::cout << "Points: " << points << std::endl;
						highscore.add(name, points);
						teammate.clear();
						trash.clear();
						enemy.clear();
						delete player;
						delete ship;
						menu = true;
					}
					std::cout << "Next level!" << std::endl;
					memcpy(map2, map, 10 * 16 * sizeof(int));
					delete player;
					delete ship;
					player = new GameObject(game, 3);
					ship = new GameObject(game, 3);
					ship->SetCoord(-1000, -1000);
					ship->hidden = true;
					enemy.clear();
					trash.clear();
					teammate.clear();
					for (int i = 0; i < 3; i++)
					{
						int x = rand() % (game.GetWidth() - game.tile + 1);
						int y = rand() % (game.GetHeight() - game.tile + 1);
						teammate.push_back(new GameObject(game, 5, x, y));
					}
					for (int i = 0; i < 7; i++)
					{
						int x = rand() % (game.GetWidth() - game.tile + 1);
						int y = rand() % (game.GetHeight() - game.tile + 1);
						trash.push_back(new GameObject(game, 4, x, y));
					}
					for (int i = 0; i < 7; i++)
					{
						int x = rand() % (game.GetWidth() - game.tile + 1);
						int y = rand() % (game.GetHeight() - game.tile + 1);
						enemy.push_back(new GameObject(game, 6, x, y));
					}

				}


				//Draw map and set texture of players/enemies based on sea/land
				for (int t = 1; t < 3; t++) {
					renderer.BatchBegin();
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 16; j++)
						{
							tex.SetCoord(j * game.tile, i * game.tile);
							tex.SetTex(map[i][j]);
							if (tex.GetTex() == t) {
								renderer.BatchDraw(tex);
							}
							if (map[i][j] == 2) { //land
								if (player->Collision(tex)) {
									if (ship->hidden == true) {
										ship->SetCoord(player->GetX(), player->GetY());
										ship->hidden = false;
									}
									player->SetTex(5);
									bool sea = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if (player->Collision(tex2)) {
													sea = true;
												}
											}
										}
									}
									if (!sea && !player->Collision(*ship)) {
										player->locked = true;
									}
									if (player->Collision(*ship)) {
										player->locked = false;
									}
									if (sea && !player->Collision(*ship)) {
										player->locked2 = true;
									}


								}
								for (auto it = enemy.begin(); it != enemy.end(); it++)
								{
									if ((**it).Collision(tex)) {
										(**it).SetTex(7);
									}
								}
								for (auto it = teammate.begin(); it != teammate.end(); it++)
								{
									if ((**it).Collision(tex)) {
										(**it).SetTex(5);
									}
								}
							}
							if (map[i][j] == 1) { //sea
								if (player->Collision(tex)) {
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if (player->Collision(tex2)) {
													land = true;
												}
											}
										}
									}
									if (!land && player->Collision(*ship)) {
										ship->hidden = true;
										ship->SetCoord(-1000, -1000);
										player->locked2 = false;
									}
									player->SetTex(3);

								}
								for (auto it = enemy.begin(); it != enemy.end(); it++)
								{
									if ((**it).Collision(tex)) {
										(**it).SetTex(6);
									}
								}
								for (auto it = teammate.begin(); it != teammate.end(); it++)
								{
									if ((**it).Collision(tex)) {
										(**it).SetTex(3);
									}
								}
								//Move only trash on the sea
								if (trash.empty()) {
									for (auto it = trash.begin(); it != trash.end(); it++)
									{
										if ((**it).Collision(tex)) {
											int x = rand() % 200 + 1;
											int y = rand() % 8;
											if (x < 5) {
												if (y == 0) {
													(**it).Move(5, 5);
												}
												else if (y == 1) {
													(**it).Move(5, -5);
												}
												else if (y == 2) {
													(**it).Move(-5, 5);
												}
												else if (y == 3) {
													(**it).Move(-5, -5);
												}
												else if (y == 4) {
													(**it).Move(-5, 0);
												}
												else if (y == 5) {
													(**it).Move(5, 0);
												}
												else if (y == 6) {
													(**it).Move(0, -5);
												}
												else if (y == 7) {
													(**it).Move(0, 5);
												}
											}
										}
									}
								}
							}
						}
					}
					renderer.BatchEnd();
					renderer.Flush(t);
				}
				//std::cout << player->locked << std::endl;
				//Draw player
				//renderer.Draw(va,ib,shader,*player,game);



				//Check if enemy collides with player
				for (auto it = enemy.begin(); it != enemy.end(); )
				{
					//Player kills enemy
					if (player->Collision(**it)) {
						bool power = false;
						for (auto it2 = enemy.begin(); it2 != enemy.end(); it2++) {
							if (it != it2) {
								if ((**it).Collision(**it2)) {
									power = true;
								}
							}
						}
						if (power) {
							std::cout << "Lost!" << std::endl;
							std::cout << "Points: " << points << std::endl;
							delete player;
							player = nullptr;
							break;
						}
						else {
							delete* it;
							it = enemy.erase(it);
							points += 2;
						}
					}
					else {
						it++;
					}
				}

				if (player == nullptr) {
					std::cout << "Lost!" << std::endl;
					highscore.add(name, points);
					teammate.clear();
					trash.clear();
					enemy.clear();
					delete player;
					delete ship;
					menu = true;
				}

				//Check if trash collides with the player
				for (auto it = trash.begin(); it != trash.end(); )
				{
					if (player->Collision(**it)) {
						delete* it;
						it = trash.erase(it);
						points += 1;
					}
					else {
						it++;
					}
				}

				//Check if teammate collides with trash, enemy or the player
				for (auto it = teammate.begin(); it != teammate.end(); )
				{
					if (player->Collision(**it)) {
						delete* it;
						it = teammate.erase(it);
						points -= 2;
					}
					else {
						for (auto it2 = enemy.begin(); it2 != enemy.end(); ) {
							if ((**it).Collision(**it2)) {
								delete* it2;
								it2 = enemy.erase(it2);
								points += 2;
							}
							else {
								it2++;
							}
						}

						for (auto it2 = trash.begin(); it2 != trash.end(); ) {
							if ((**it).Collision(**it2)) {
								delete* it2;
								it2 = trash.erase(it2);
								points += 1;
							}
							else {
								it2++;
							}
						}
						it++;
					}

				}

				//Move teammates
				if (n == 2) {
					for (auto it = teammate.begin(); it != teammate.end(); it++)
					{
						if ((**it).GetTex() == 3) {
							if ((**it).tox == (**it).GetX() && (**it).toy == (**it).GetY()) {
								(**it).tox = -1;
								(**it).toy = -1;
							}
							if ((**it).tox == -1 && (**it).toy == -1) {
								bool notgood = false;
								do {
									int x = rand() % (1024 - 64);
									int y = rand() % (640 - 64);
									(**it).tox = x;
									(**it).toy = y;
									GameObject tmp(game, 5, x, y, 64, 64);
									GameObject tex2(game, 1);
									notgood = false;
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if (tmp.Collision(tex2)) {
													notgood = true;
													break;
												}
											}
										}
										if (notgood) break;
									}
								} while (notgood);
							}
							else {
								if ((**it).tox > (**it).GetX()) {
									(**it).Move(1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(-1, 0);
									}
								}
								else if ((**it).tox < (**it).GetX()) {
									(**it).Move(-1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(1, 0);
									}
								}
								if ((**it).toy > (**it).GetY()) {
									(**it).Move(0, 1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, -1);
									}
								}
								else if ((**it).toy < (**it).GetY()) {
									(**it).Move(0, -1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, 1);
									}
								}
							}
						}
						else {

							if ((**it).tox == (**it).GetX() && (**it).toy == (**it).GetY()) {
								(**it).tox = -1;
								(**it).toy = -1;
							}
							if ((**it).tox == -1 && (**it).toy == -1) {
								bool notgood = false;
								do {
									int x = rand() % (1024 - 64);
									int y = rand() % (640 - 64);
									(**it).tox = x;
									(**it).toy = y;
									GameObject tmp(game, 5, x, y, 64, 64);
									GameObject tex2(game, 1);
									notgood = false;
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if (tmp.Collision(tex2)) {
													notgood = true;
													break;
												}
											}
										}
										if (notgood) break;
									}
								} while (notgood);
							}
							else {
								if ((**it).tox > (**it).GetX()) {
									(**it).Move(1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(-1, 0);
									}
								}
								else if ((**it).tox < (**it).GetX()) {
									(**it).Move(-1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(1, 0);
									}
								}
								if ((**it).toy > (**it).GetY()) {
									(**it).Move(0, 1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, -1);
									}
								}
								else if ((**it).toy < (**it).GetY()) {
									(**it).Move(0, -1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, 1);
									}
								}
							}
						}
					}
				}

				//Move enemies
				if (n == 2) {
					for (auto it = enemy.begin(); it != enemy.end(); it++)
					{
						if ((**it).GetTex() == 6) {
							if ((**it).tox == (**it).GetX() && (**it).toy == (**it).GetY()) {
								(**it).tox = -1;
								(**it).toy = -1;
							}
							if ((**it).tox == -1 && (**it).toy == -1) {
								bool notgood = false;
								do {
									int x = rand() % (1024 - 64);
									int y = rand() % (640 - 64);
									(**it).tox = x;
									(**it).toy = y;
									GameObject tmp(game, 5, x, y, 64, 64);
									GameObject tex2(game, 1);
									notgood = false;
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if (tmp.Collision(tex2)) {
													notgood = true;
													break;
												}
											}
										}
										if (notgood) break;
									}
								} while (notgood);
							}
							else {
								if ((**it).tox > (**it).GetX()) {
									(**it).Move(1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(-1, 0);
									}
								}
								else if ((**it).tox < (**it).GetX()) {
									(**it).Move(-1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(1, 0);
									}
								}
								if ((**it).toy > (**it).GetY()) {
									(**it).Move(0, 1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, -1);
									}
								}
								else if ((**it).toy < (**it).GetY()) {
									(**it).Move(0, -1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 2) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, 1);
									}
								}
							}
						}
						else {

							if ((**it).tox == (**it).GetX() && (**it).toy == (**it).GetY()) {
								(**it).tox = -1;
								(**it).toy = -1;
							}
							if ((**it).tox == -1 && (**it).toy == -1) {
								bool notgood = false;
								do {
									int x = rand() % (1024 - 64);
									int y = rand() % (640 - 64);
									(**it).tox = x;
									(**it).toy = y;
									GameObject tmp(game, 5, x, y, 64, 64);
									GameObject tex2(game, 1);
									notgood = false;
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if (tmp.Collision(tex2)) {
													notgood = true;
													break;
												}
											}
										}
										if (notgood) break;
									}
								} while (notgood);
							}
							else {
								if ((**it).tox > (**it).GetX()) {
									(**it).Move(1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(-1, 0);
									}
								}
								else if ((**it).tox < (**it).GetX()) {
									(**it).Move(-1, 0);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(1, 0);
									}
								}
								if ((**it).toy > (**it).GetY()) {
									(**it).Move(0, 1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, -1);
									}
								}
								else if ((**it).toy < (**it).GetY()) {
									(**it).Move(0, -1);
									bool land = false;
									GameObject tex2(game, 1);
									for (int m = 0; m < 10; m++) {
										for (int n = 0; n < 16; n++) {
											tex2.SetCoord(n * game.tile, m * game.tile);
											tex2.SetTex(map[m][n]);
											if (map[m][n] == 1) {
												if ((**it).Collision(tex2)) {
													land = true;
													break;
												}
											}
										}
										if (land) break;
									}
									if (land) {
										(**it).tox = -1;
										(**it).toy = -1;
										(**it).Move(0, 1);
									}
								}
							}
						}
						//Enemy randomly throws trash
						int t = rand() % 30000 + 1;
						if (t < 5) {
							trash.push_back(new GameObject(game, 4, (**it).GetX(), (**it).GetY()));
						}
					}
					n = 0;
				}
				n++;

				replay.write(name, player->GetX(), player->GetY());

				//Render entities
				for (int t = 1; t < texturemanager.size(); t++)
				{
					renderer.BatchBegin();
					for (auto it = enemy.begin(); it != enemy.end(); it++)
					{
						if ((**it).GetTex() == t) {
							if (player->CheckDistance(**it) < 300)
								renderer.BatchDraw(**it);
						}
					}
					for (auto it = trash.begin(); it != trash.end(); it++)
					{
						if ((**it).GetTex() == t) {
							if (player->CheckDistance(**it) < 300)
								renderer.BatchDraw(**it);
						}
					}
					for (auto it = teammate.begin(); it != teammate.end(); it++)
					{
						if ((**it).GetTex() == t) {
							if (player->CheckDistance(**it) < 300)
								renderer.BatchDraw(**it);
						}
					}
					if ((*player).GetTex() == t)
						renderer.BatchDraw(*player);
					if ((*ship).GetTex() == t && (*ship).hidden == false)
						renderer.BatchDraw(*ship);
					renderer.BatchEnd();
					renderer.Flush(t);
				}
				std::string point = "Points: " + std::to_string(points);
				renderer.BatchBegin();
				renderer.BatchDrawString(0, 600, point);
				renderer.BatchEnd();
				renderer.Flush(10);
			}
		}

		//Swap back and front buffers
		game.SwapBuffer();

		

		//Poll and process events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				shouldCloseWindow = true;
				break;
			}
			if (event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					game.setWindowSize(event.window.data1, event.window.data2);
					renderer.SetWindowsSize(event.window.data1, event.window.data2);
				}
			}
			if (event.type == SDL_KEYDOWN) {
				const Uint8 *state = SDL_GetKeyboardState(nullptr);
				if (!menu) {
					if (state[SDL_SCANCODE_LEFT]) {
						player->Move(-4, 0);
						if (player->locked) {
							bool sea = false;
							GameObject tex2(game, 1);
							for (int m = 0; m < 10; m++) {
								for (int n = 0; n < 16; n++) {
									tex2.SetCoord(n*game.tile, m*game.tile);
									tex2.SetTex(map[m][n]);
									if (map[m][n] == 1) {
										if (player->Collision(tex2)) {
											sea = true;
										}
									}
								}
							}
							if (sea) {
								player->Move(4, 0);
							}
						}
						if (player->locked2) {
							if (!player->Collision(*ship)) {
								player->Move(4, 0);
							}
						}
					}
					if (state[SDL_SCANCODE_RIGHT]) {
						player->Move(4, 0);
						if (player->locked) {
							bool sea = false;
							GameObject tex2(game, 1);
							for (int m = 0; m < 10; m++) {
								for (int n = 0; n < 16; n++) {
									tex2.SetCoord(n*game.tile, m*game.tile);
									tex2.SetTex(map[m][n]);
									if (map[m][n] == 1) {
										if (player->Collision(tex2)) {
											sea = true;
										}
									}
								}
							}
							if (sea) {
								player->Move(-4, 0);
							}
						}
						if (player->locked2) {
							if (!player->Collision(*ship)) {
								player->Move(-4, 0);
							}
						}
					}
					if (state[SDL_SCANCODE_DOWN]) {
						player->Move(0, -4);
						if (player->locked) {
							bool sea = false;
							GameObject tex2(game, 1);
							for (int m = 0; m < 10; m++) {
								for (int n = 0; n < 16; n++) {
									tex2.SetCoord(n*game.tile, m*game.tile);
									tex2.SetTex(map[m][n]);
									if (map[m][n] == 1) {
										if (player->Collision(tex2)) {
											sea = true;
										}
									}
								}
							}
							if (sea) {
								player->Move(0, 4);
							}
						}
						if (player->locked2) {
							if (!player->Collision(*ship)) {
								player->Move(0, 4);
							}
						}
					}
					if (state[SDL_SCANCODE_UP]) {
						player->Move(0, 4);
						if (player->locked) {
							bool sea = false;
							GameObject tex2(game, 1);
							for (int m = 0; m < 10; m++) {
								for (int n = 0; n < 16; n++) {
									tex2.SetCoord(n*game.tile, m*game.tile);
									tex2.SetTex(map[m][n]);
									if (map[m][n] == 1) {
										if (player->Collision(tex2)) {
											sea = true;
										}
									}
								}
							}
							if (sea) {
								player->Move(0, -4);
							}
						}
						if (player->locked2) {
							if (!player->Collision(*ship)) {
								player->Move(0, -4);
							}
						}
					}
					if (state[SDL_SCANCODE_ESCAPE]) {
						if (highscoremenu) {
							highscoremenu = false;
							menu = true;
						}
						else if (!isReplaying && !highscoremenu) {
							highscore.add(name, points);
							save.saveAll(name, enemy, trash, teammate, player, ship, points, difficulty);
							teammate.clear();
							trash.clear();
							enemy.clear();
							delete player;
							delete ship;
						}
						else {
							isReplaying = false;
							replay.stop();
						}
						highscore.print();
						menu = true;
					}

				}
			}
			if (menu) {
				if (event.type == SDL_KEYDOWN) {
					const Uint8* state = SDL_GetKeyboardState(nullptr);
					if (name.size() < 10) {
						if (state[SDL_SCANCODE_A]) name += 'a';
						if (state[SDL_SCANCODE_B]) name += 'b';
						if (state[SDL_SCANCODE_C]) name += 'c';
						if (state[SDL_SCANCODE_D]) name += 'd';
						if (state[SDL_SCANCODE_E]) name += 'e';
						if (state[SDL_SCANCODE_F]) name += 'f';
						if (state[SDL_SCANCODE_G]) name += 'g';
						if (state[SDL_SCANCODE_H]) name += 'h';
						if (state[SDL_SCANCODE_I]) name += 'i';
						if (state[SDL_SCANCODE_J]) name += 'j';
						if (state[SDL_SCANCODE_K]) name += 'k';
						if (state[SDL_SCANCODE_L]) name += 'l';
						if (state[SDL_SCANCODE_M]) name += 'm';
						if (state[SDL_SCANCODE_N]) name += 'n';
						if (state[SDL_SCANCODE_O]) name += 'o';
						if (state[SDL_SCANCODE_P]) name += 'p';
						if (state[SDL_SCANCODE_R]) name += 'r';
						if (state[SDL_SCANCODE_S]) name += 's';
						if (state[SDL_SCANCODE_T]) name += 't';
						if (state[SDL_SCANCODE_U]) name += 'u';
						if (state[SDL_SCANCODE_V]) name += 'v';
						if (state[SDL_SCANCODE_Z]) name += 'z';
						if (state[SDL_SCANCODE_Q]) name += 'q';
						if (state[SDL_SCANCODE_W]) name += 'w';
						if (state[SDL_SCANCODE_Y]) name += 'y';
						if (state[SDL_SCANCODE_X]) name += 'x';
					}
					if (state[SDL_SCANCODE_BACKSPACE]) {
						if(!name.empty())
							name.pop_back();
					}
				}
				if (event.type == SDL_MOUSEBUTTONUP) {
					if (event.button.button == SDL_BUTTON_LEFT) {
						//std::cout << event.button.x << std::endl;
						//std::cout << event.button.y << std::endl;
						if (!name.empty()) {
							if (event.button.x > 512 && event.button.x < 767 && event.button.y < 300 && event.button.y > 245) { //NEW GAME
								for (int i = 0; i < 5; i++)
								{
									int x;
									int y;
									bool land = false;
									bool sea = false;
									do {
										x = rand() % (game.GetWidth() - game.tile + 1);
										y = rand() % (game.GetHeight() - game.tile + 1);
										land = false;
										sea = false;
										GameObject tex2(game, 1);
										GameObject tmp(game, 1, x, y, 64, 64);
										for (int m = 0; m < 10; m++) {
											for (int n = 0; n < 16; n++) {
												tex2.SetCoord(n * game.tile, m * game.tile);
												tex2.SetTex(map[m][n]);
												if (map[m][n] == 1) {
													if (tmp.Collision(tex2)) {
														land = true;
													}
												}
												if (map[m][n] == 2) {
													if (tmp.Collision(tex2)) {
														sea = true;
													}
												}
											}
										}
									} while (land && sea);
									teammate.push_back(new GameObject(game, 5, x, y));
								}
								for (int i = 0; i < 8; i++)
								{
									int x;
									int y;
									bool land = false;
									bool sea = false;
									do {
										x = rand() % (game.GetWidth() - game.tile + 1);
										y = rand() % (game.GetHeight() - game.tile + 1);
										land = false;
										sea = false;
										GameObject tex2(game, 1);
										GameObject tmp(game, 1, x, y, 64, 64);
										for (int m = 0; m < 10; m++) {
											for (int n = 0; n < 16; n++) {
												tex2.SetCoord(n * game.tile, m * game.tile);
												tex2.SetTex(map[m][n]);
												if (map[m][n] == 1) {
													if (tmp.Collision(tex2)) {
														land = true;
													}
												}
												if (map[m][n] == 2) {
													if (tmp.Collision(tex2)) {
														sea = true;
													}
												}
											}
										}
									} while (land && sea);
									trash.push_back(new GameObject(game, 4, x, y));
								}
								for (int i = 0; i < 5; i++)
								{
									int x;
									int y;
									bool land = false;
									bool sea = false;
									do {
										x = rand() % (game.GetWidth() - game.tile + 1);
										y = rand() % (game.GetHeight() - game.tile + 1);
										land = false;
										sea = false;
										GameObject tex2(game, 1);
										GameObject tmp(game, 1, x, y, 64, 64);
										for (int m = 0; m < 10; m++) {
											for (int n = 0; n < 16; n++) {
												tex2.SetCoord(n * game.tile, m * game.tile);
												tex2.SetTex(map[m][n]);
												if (map[m][n] == 1) {
													if (tmp.Collision(tex2)) {
														land = true;
													}
												}
												if (map[m][n] == 2) {
													if (tmp.Collision(tex2)) {
														sea = true;
													}
												}
											}
										}
									} while (land && sea);
									enemy.push_back(new GameObject(game, 6, x, y));
								}
								player = new GameObject(game, 3);
								ship = new GameObject(game, 3);
								ship->SetCoord(-1000, -1000);
								ship->hidden = true;
								difficulty = 1;
								points = 0;
								replay.clear(name);
								menu = false;
							}
							else if (event.button.x > 512 && event.button.x < 767 && event.button.y < 391 && event.button.y > 334) { //LOAD GAME
								if (save.loadAll(name, enemy, trash, teammate, player, ship, points, difficulty)) {
									menu = false;
									if (difficulty == 2) {
										memcpy(map2, map, 10 * 16 * sizeof(int));
									}
								}
							}
							else if (event.button.x > 512 && event.button.x < 767 && event.button.y < 566 && event.button.y > 509) { //REPLAY
								isReplaying = true;
								replay.start(name);
								player = new GameObject(game, 3);
								ship = new GameObject(game, 3);
								ship->SetCoord(-1000, -1000);
								ship->hidden = true;
								menu = false;
							}
						}
						else if (event.button.x > 512 && event.button.x < 767 && event.button.y < 473 && event.button.y > 417) { //HIGHSCORES
							highscoremenu = true;
							menu = false;
						}
					}
				}
			}
		}
	}
	highscore.write();
	return 0;
}
