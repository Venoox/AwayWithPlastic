#include "GameObject.h"

void GameObject::Move(int x, int y) {
	this->x += x;
	this->y += y;

    if(this->x < 0) this->x = 0;
    if(this->y < 0) this->y = 0;
    if(this->x+width > 1024) this->x = 1024 - width;
    if(this->y+height > 640) this->y = 640 - height;
}

void GameObject::SetCoord(int x, int y) {
    this->x = x;
    this->y = y;
}

void GameObject::SetTex(unsigned int slot) {
    tex = slot; 
}

GameObject::GameObject()
{
	std::cout << "Initializing GameObject variable without game contraints!" << std::endl;
	this->locked = false;
	this->locked2 = false;
	this->hidden = false;
	this->tex = 1;
	this->x = 0;
	this->y = 0;
	this->width = 64;
	this->height = 64;
	this->tox = -1;
	this->toy = -1;
	this->game_width = 1280;
	this->game_height = 720;
}

GameObject::GameObject(const GameObject& copy)
{
	this->locked = copy.locked;
	this->locked2 = copy.locked2;
	this->hidden = copy.hidden;
	this->tex = copy.tex;
	this->x = copy.x;
	this->y = copy.y;
	this->width = copy.width;
	this->height = copy.height;
	this->tox = copy.tox;
	this->toy = copy.toy;
	this->game_width = copy.game_width;
	this->game_height = copy.game_height;
}

GameObject::GameObject(Game &game) {
	this->locked = false;
	this->hidden = false;
	this->locked2 = false;
	this->tex = 0;
	this->width = 64;
	this->height = 64;
	this->x = 0;
	this->y = 0;
	this->tox = -1;
	this->toy = -1;
	this->game_width = game.GetWidth();
	this->game_height = game.GetHeight();
}

GameObject::GameObject(Game& game, int texture) {
	this->locked = false;
	this->locked2 = false;
	this->hidden = false;
	this->tex = texture;
	this->width = 64;
	this->height = 64;
	this->x = 0;
	this->y = 0;
	this->tox = -1;
	this->toy = -1;
	this->game_width = game.GetWidth();
	this->game_height = game.GetHeight();
}

GameObject::GameObject(Game& game, int texture, int x, int y) {
	this->locked = false;
	this->locked2 = false;
	this->hidden = false;
	this->tex = texture;
    this->x = x;
    this->y = y;
	this->width = 64;
	this->height = 64;
	this->tox = -1;
	this->toy = -1;
	this->game_width = game.GetWidth();
	this->game_height = game.GetHeight();
}

GameObject::GameObject(Game& game, int texture, int x, int y, int width, int height) {
	this->locked = false;
	this->locked2 = false;
	this->hidden = false;
	this->tex = texture;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
	this->tox = -1;
	this->toy = -1;
	this->game_width = game.GetWidth();
	this->game_height = game.GetHeight();
}

bool GameObject::Collision(const GameObject &b) {
    if (x + 64 > b.x && y + 64 > b.y && x < b.x + 64 && y < b.y + 64) {
        return true;                
    }
    else return false;
}

double GameObject::CheckDistance(const GameObject &b) {
    return sqrt(((b.x - x) * (b.x - x)) +
                 ((b.y - y) * (b.y - y)));
}
