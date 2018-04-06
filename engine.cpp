#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "multisprite2way.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() {
  /*delete star;
  delete spinningStar;*/
  std::vector<Drawable*>::const_iterator itr =
    sprites.begin();
  for ( ; itr != sprites.end(); itr++) {
    delete *itr;
  }

  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  bricks("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world("middle", Gamedata::getInstance().getXmlInt("middle/factor") ),
  clouds("top", Gamedata::getInstance().getXmlInt("top/factor") ),
  viewport( Viewport::getInstance() ),
  //star(new Sprite("YellowStar")),
  //spinningStar(new MultiSprite("SpinningStar")),
  currentSprite(0),
  makeVideo( false )
{
  srand(time(NULL));
  //sprites.emplace_back(new Sprite("RunningMan"));
  //sprites.emplace_back(new MultiSprite("RunningMan2R"));
  sprites.emplace_back(new MultiSprite("Student"));
  for (int i = 0; i < 7; i++) {
    sprites.emplace_back(new MultiSprite("Paper", float(rand()%350), float(rand()%400)));
  }
  //Viewport::getInstance().setObjectToTrack(star);
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  bricks.draw();
  world.draw();

  //star->draw();
  //spinningStar->draw();
  std::vector<Drawable*>::const_iterator itr =
    sprites.begin();
  for ( ; itr != sprites.end(); itr++) {
    (*itr)->draw();
  }
  //sprites[0]->draw();
  //sprites[1]->draw();

  clouds.draw();

  viewport.draw();
  viewport.drawFPS(clock.getFps());
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  bricks.update();
  world.update();

  //star->update(ticks);
  //spinningStar->update(ticks);
  //sprites[0]->update(ticks);
  //sprites[1]->update(ticks);
  std::vector<Drawable*>::const_iterator itr =
    sprites.begin();
  for ( ; itr != sprites.end(); itr++) {
    (*itr)->update(ticks);
  }

  clouds.update();
  viewport.drawFPS(clock.getFps());
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  //currentSprite = currentSprite % Gamedata::getInstance().getXmlInt("numSprites") ;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
