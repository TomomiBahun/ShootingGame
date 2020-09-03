#include "HitCheck.h"
#include "Define.h"
#include <math.h>
#include <algorithm>


/* this checks if bullet hit a charactor
   @shot vector of bullets
   @n index of bullets vector
   @x the charactor's location on x coordinate
   @y the charactor's location on y coordinate
   @range the charactor's hit range. If a bullet is in the range, the charactor gets hit*/
bool HitCheck::didBulletHitPlayer(std::vector<Bullet> shot, int n, float x, float y, float range)
{
	if (shot[n].getFlag() != 0) { // if the bullet is active on the board
		float hit_x = shot[n].getX() - x;
		float hit_y = shot[n].getY() - y;

		float hit_range = shot[n].getRange() + range;

		if (shot[n].getSpeed() > hit_range) {
			float pre_x = shot[n].getX() + cos(shot[n].getAngle() + Define::PI) * shot[n].getSpeed();
			float pre_y = shot[n].getY() + sin(shot[n].getAngle() + Define::PI) * shot[n].getSpeed();
			float px, py;
			for (int i = 0; i < shot[n].getSpeed() / hit_range; i++) {
				px = pre_x - x;
				py = pre_y - y;
				if (px * px + py + py < hit_range * hit_range)
					return true; // hit
				pre_x += cos(shot[n].getAngle()) * hit_range;
				pre_y += sin(shot[n].getAngle()) * hit_range;
			}
		}
		if (hit_x * hit_x + hit_y * hit_y < hit_range * hit_range) {
			return true; // hit
		}
	}
	return false; // no hit
}

/* this checks if bullet hit enemy. Also save the index number of player's bullets that hit enemy
   @shot[] vector of bullets. Size of vector is always 150(MAX_BULLETS)
   @n index of bullets vector
   @x the charactor's location on x coordinate
   @y the charactor's location on y coordinate
   @range the charactor's hit range. If a bullet is in the range, the charactor gets hit*/
bool HitCheck::didBulletHitEnemy(Bullet shot[], int n, float x, float y, float range)
{
	if (shot[n].getFlag() != 0) { // if the bullet is active on the board
		float hit_x = shot[n].getX() - x;
		float hit_y = shot[n].getY() - y;

		float hit_range = shot[n].getRange() + range;

		if (shot[n].getSpeed() > hit_range) {
			float pre_x = shot[n].getX() + cos(shot[n].getAngle() + Define::PI) * shot[n].getSpeed();
			float pre_y = shot[n].getY() + sin(shot[n].getAngle() + Define::PI) * shot[n].getSpeed();
			float px, py;
			for (int i = 0; i < shot[n].getSpeed() / hit_range; i++) {
				px = pre_x - x;
				py = pre_y - y;
				if (px * px + py + py < hit_range * hit_range) {
					playerShotHitIndex.push_back(n); // if hit, save the index number
					return true; // hit
				}
				pre_x += cos(shot[n].getAngle()) * hit_range;
				pre_y += sin(shot[n].getAngle()) * hit_range;
			}
		}
		if (hit_x * hit_x + hit_y * hit_y < hit_range * hit_range) {
			playerShotHitIndex.push_back(n); // if hit, save the index number
			return true; // hit
		}
	}
	return false; // no hit
}

std::vector<int>& HitCheck::getPlayerShotHitIndex()
{
	// player bullets might hit several enemies in a frame. sort the vector before using it
	unique(playerShotHitIndex.begin(), playerShotHitIndex.begin() + playerShotHitIndex.size());
	sort(playerShotHitIndex.begin(), playerShotHitIndex.end());

	return playerShotHitIndex;
}

void HitCheck::clearPlayerShotHitIndex()
{
	playerShotHitIndex.clear();
}