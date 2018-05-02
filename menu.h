#include <vector>
#include <string>
#include "SDL.h"
#include "gamedata.h"
#include "clock.h"
#include "ioMod.h"
#include "sprite.h"
#include "multisprite.h"

class Menu {
public:
  Menu(SDL_Renderer*);
  void draw() const;
  void update();
  void incrIcon();
  void decrIcon();
  void lightOn()  { currentClick = 1; }
  void lightOff() { currentClick = 0; }
  void updatePosMenu(float, float);

  int getOptionNo() const { return currentOption; }
  int getNumStars() const;

  Menu(const Menu&) = delete;
  Menu& operator=(const Menu&) = delete;
private:
  SDL_Renderer* renderer;
  Gamedata& gdata;
  Clock& clock;
  IoMod& io;
  int width;
  int height;
  MultiSprite background;
  std::vector<std::string> options;
  std::vector<int> optionLoc;
  std::vector<Sprite> clicks;
  int currentClick;
  int currentOption;
  int spaces;
  int startClickX;
  int startClickY;
  int clickX;
  int clickY;
  void drawBackground() const;
  int getInputEventLoop() const;
};
