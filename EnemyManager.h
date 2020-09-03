#pragma once
#include "Task.h"
#include <memory>
#include <vector>
#include "Bullet.h"
#include "AbstractEnemy.h"
#include "AbstractShot.h"
#include <algorithm> // for copy()
#include <iterator> // for back_inserter 


class EnemyManager : public Task
{
public:

	const static int ENEMY_SHOT_POWER = 10;

	EnemyManager();
	virtual ~EnemyManager() = default;
	bool update() override;
	void draw() const override;

	std::vector<Bullet>& getActiveEnemyBullet();
	std::vector<std::shared_ptr<AbstractEnemy>>& getList() { return _list; }
	std::vector<int>& getPlayerShotIndex() { return playerShotIndex; } // this will return the index of player shot that hit the enemy. These shots needs to be deleted.
	bool getFlag() const { return _flag; }
	void setPlayerX(float x) { _playerX = x; }
	void setPlayerY(float y) { _playerY = y; }
	void setPlayerPower(int power) { _playerPower = power; }
	void setActivePlayerBullets(std::vector<Bullet>& playerBullets);

	void setPlayerShot(AbstractShot& shot); // testing

	float angleEnemyAndPlayer(float enemyX, float enemyY) const;

private:
	bool _flag; // when true, boss should appear on the game board
	int _count;
	float _playerX, _playerY;
	int _playerPower;
	AbstractShot* playerShot; // testing
	std::vector<int> playerShotIndex;
	std::vector<Bullet> _activeEnemyBullets; // this is to pass all active enemy bullets to Player class
	std::vector<Bullet> _activePlayerBullets; // this is to check if player's shot hits enemy
	std::vector<std::shared_ptr<AbstractEnemy>> _list;
	std::vector<std::shared_ptr<AbstractShot>> _shotList;
	std::vector<std::shared_ptr<AbstractShot>> _continueShotList; // even after enemy dies, bullets on the board need to keep moving. 

	bool didBulletHitMe(std::shared_ptr<AbstractEnemy> Enemy);
	void loadEnemyAndShots();
};