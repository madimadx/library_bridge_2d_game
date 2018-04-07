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
#include "player.h"

Engine::~Engine() {
  std::vector<Drawable*>::const_iterator itr =
    sprites.begin();
  for ( ; itr != sprites.end(); itr++) {
    delete *itr;
  }
  delete player;

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
  player(new Player("Student")),
  currentSprite(0),
  makeVideo( false )
{
  srand(time(NULL));

  for (int i = 0; i < 7; i++) {
    sprites.emplace_back(new MultiSprite("Paper", float(rand()%350), float(rand()%400)));
  }
  Viewport::getInstance().setObjectToTrack(player->getPlayer());
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  bricks.draw();
  world.draw();
  player->draw();
  std::vector<Drawable*>::const_iterator itr =
    sprites.begin();
  for ( ; itr != sprites.end(); itr++) {
    (*itr)->draw();
  }

  clouds.draw();

  viewport.draw();
  //viewport.drawFPS(clock.getFps());
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  bricks.update();
  world.update();

  std::vector<Drawable*>::const_iterator itr =
    sprites.begin();
  for ( ; itr != sprites.end(); itr++) {
    (*itr)->update(ticks);
  }
  player->update(ticks);

  clouds.update();
  //viewport.drawFPS(clock.getFps());
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % (sprites.size()+1);
  if (currentSprite != 0)
    Viewport::getInstance().setObjectToTrack(sprites[currentSprite-1]);
  else
    Viewport::getInstance().setObjectToTrack(player->getPlayer());
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
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
