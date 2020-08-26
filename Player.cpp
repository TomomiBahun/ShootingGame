#include "Player.h"
#include "keyboard.h"
#include <DxLib.h>
#include "Image.h"
#include "Define.h"
#include "ImageBullet.h"

using namespace std;

const static float SPEED = 6;

/* set the initial location (x, y) = (100, 100) and load the player image */
Player::Player() :_counter(0), _x(Define::CENTER_X), _y(Define::CENTER_Y + 500), direction(0), _power(1), _health(50), _noHitTimer(0)
{
	//Bullet dummy;
	//activeEnemyBullets.push_back(dummy);
}

bool Player::update()
{
	/* check if any of enemy bullet hits the player
	   if player gets hit, decrease the player health and set the noHitTimer*/
	if (_noHitTimer == 0) {
		if (didBulletHitMe()) {
			_noHitTimer = 12;
		}
	}
	_counter++;
	move();
	shotBullets();
	_playerShot.update();
	return true;
}

void Player::draw() const
{
	if (_noHitTimer == 0) {
		DrawRotaGraphF(_x, _y, 1.5f, 0.0f, Image::getIns()->getPlayer()[direction], TRUE);
	}
	else {
		if (_counter % 7 == 0) { // during no hit time, blink the player image
			DrawRotaGraphF(_x, _y, 1.5f, 0.0f, Image::getIns()->getPlayer()[direction], TRUE);
			_noHitTimer--; // temporarily mutable
		}
	}
	_playerShot.draw();
}

/* get all of the active bullets */
std::vector<Bullet>& Player::getActivePlayerBullet()
{
	_activePlayerBullets.clear(); // before updating the vector, delete the elements from the previous frame
	for (int i = 0; i < AbstractShot::MAX_BULLETS; i++) {
		if (_playerShot.shot[i].getFlag() > 0) {
			_activePlayerBullets.push_back(_playerShot.shot[i]);
		}
	}
	return _activePlayerBullets;
}

void Player::setActiveEnemyBullets(std::vector<Bullet>& enemyBullets)
{
	_activeEnemyBullets.clear(); // delete all elements in vector before updating it
	for (int i = 0; i < enemyBullets.size(); i++) {
		_activeEnemyBullets.push_back(enemyBullets[i]);
	}
}

/* controls player's location and move*/
void Player::move()
{
	// change in x and y
	float moveX = 0;
	float moveY = 0;
	
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_DOWN) > 0) {
		moveY += SPEED;
	}
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_UP) > 0) {
		moveY -= SPEED;
	}
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_LEFT) > 0 ) {
		moveX -= SPEED;
		direction = 2;
	}
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_RIGHT) > 0) {
		moveX += SPEED;
		direction = 4;
	}
	// move diagno
	if (moveX && moveY) {
		moveX /= (float)sqrt(2.0);
		moveY /= (float)sqrt(2.0);
	}
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_LSHIFT) > 0) {
		moveX /= 3;
		moveY /= 3;
	}
	// if no horizontal move, set the direction back to normal
	if (moveX == 0) {
		direction = 0;
	}
	
	if (moveX + _x < Define::IN_X + (_w / 2)) { // if beyond the left side of game board
		_x = (float)(Define::IN_X + (_w / 2));
	}
	else if (moveX + _x > Define::IN_X + Define::INNER_W - (_w / 2)) { // if beyond the right side of game board
		_x = (float)(Define::IN_X + Define::INNER_W - (_w / 2));
	}
	else { // if within the game board
		_x += moveX;
	}
	if (moveY + _y < Define::IN_Y + (_h / 2)) { // if beyond the game board
		_y = (float)(Define::IN_Y + (_h / 2));
	}
	else if (moveY + _y > Define::IN_Y + Define::INNER_H - (_h / 2)) { // if beyond the bottom of the game board
		_y = (float)(Define::IN_Y + Define::INNER_H - (_h / 2));
	}
	else { // if within the game board
		_y += moveY;
	}
}

/* Player's shot properties */
void Player::shotBullets()
{
	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_SPACE) > 0) {
		_playerShot.setBullets(_x, _y, Define::PI/2*3, _power);
	}
}

bool Player::didBulletHitMe()
{
	for (int i = 0; i < _activeEnemyBullets.size(); i++) {
		if (HitCheck::getIns()->didBulletHitMe(_activeEnemyBullets, i, _x, _y, _range)) {
			return true;
		}
	}
	return false;
}