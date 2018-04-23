#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Player;
class CollisionStrategy;
class SmartSprite;
class Hud;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void menu();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World bricks;
  World world;
  World clouds;
  Viewport& viewport;

  std::vector<ShootingSprite*> shooters;
  std::vector<Drawable*> dummies;
  std::vector<SmartSprite*> smarties;
  Player* player;
  std::vector<CollisionStrategy*> strategies;
  Hud* theHud;
  int currentStrategy;
  bool collision;
  bool hudOn;
  int popUpDelay;
  int delayCount;
  int indexUp = Gamedata::getInstance().getXmlInt("Keys/indexU");
  int indexDown = Gamedata::getInstance().getXmlInt("Keys/indexD");
  int indexL = Gamedata::getInstance().getXmlInt("Keys/indexL");
  int indexR = Gamedata::getInstance().getXmlInt("Keys/indexR");
  int indexCol = Gamedata::getInstance().getXmlInt("Keys/indexCol");
  int indexHUD = Gamedata::getInstance().getXmlInt("Keys/indexHUD");
  int indexPause = Gamedata::getInstance().getXmlInt("Keys/indexPause");

  bool makeVideo;

  void drawMenu() const;
  void resetDelay();
  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
