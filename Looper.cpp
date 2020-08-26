#include "Looper.h"
#include "TitleScene.h"
#include "Error.h"
#include "GameScene.h"
#include "keyboard.h"
#include "Image.h"

#define ERR(str) Error::finish(str, __FUNCTION__, __LINE__);

using namespace std;

/* constructor */
Looper::Looper()
{
	Image::getIns()->load();
	Parameter parameter;
	// the first scene is always the title
	_sceneStack.push(make_shared<TitleScene>(this, parameter));
}


/* here the main loop */
bool Looper::loop()
{
	Keyboard::getIns()->update();
	_sceneStack.top()->update();
	_sceneStack.top()->draw();
	_fps.draw();
	_fps.wait();
	return true;
}

void Looper::onSceneChanged(const eScene scene, const Parameter& parameter, const bool stackClear)
{
	if (stackClear) {// if stackClear parameter is passed for this function
		while (!_sceneStack.empty()) { // check if the stack is empty already, and pop all the stacks
			_sceneStack.pop();
		}
	}

	switch (scene) {
	case Title:
		_sceneStack.push(make_shared<TitleScene>(this, parameter));
		break;
	case Game:
		_sceneStack.push(make_shared<GameScene>(this, parameter));
		break;
	default:
		ERR("Nonexistent scene was called");
		break;
	}
}