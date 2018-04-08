#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "collisionStrategy.h"

Engine::~Engine() {
  for (Drawable* dummie: dummies) {
    delete dummie;
  }
  for ( Drawable* smartie : smarties ) {
    delete smartie;
  }

  delete player;

  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
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
  dummies(),
  smarties(),
  player(new Player("Student")),
  strategies(),
  currentStrategy(0),
  collision(false),
  popUpDelay(Gamedata::getInstance().getXmlInt("popUpDelay")),
  delayCount(0),
  makeVideo( false )
{
  srand(time(NULL));

  Vector2f pos = (player->getPlayer())->getPosition();
  int w = (player->getPlayer())->getScaledWidth();
  int h = (player->getPlayer())->getScaledHeight();
  for (int i = 0; i < 7; i++) {
    smarties.emplace_back(new SmartSprite("Paper", pos, w, h, float(rand()%350), float(rand()%400)));
    player->attach( smarties[i] );
  }

  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player->getPlayer());
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  bricks.draw();
  world.draw();
  player->draw();

  for (const Drawable* dummie : dummies) {
    dummie->draw();
  }
  for ( const Drawable* smartie : smarties ) {
      smartie->draw();
  }

  clouds.draw();

  IoMod::getInstance().writeText("Press m to change strategy", 30, 30);
  strategies[currentStrategy]->draw();
  if ( collision )
    IoMod::getInstance().writeText("Oops: Collision", 30, 110);

  viewport.draw();
  //viewport.drawFPS(clock.getFps());
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = smarties.begin();
  while ( it != smarties.end() ) {
    if ( strategies[currentStrategy]->execute(*(player->getPlayer()), **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      collision = true;
      resetDelay();
      it = smarties.erase(it);
    }
    else ++it;
  }
}

void Engine::resetDelay() {
  delayCount = 0;
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  if ((delayCount++) == popUpDelay) {
      resetDelay();
      collision = false;
  }
  bricks.update();
  world.update();

  for (Drawable* dummie : dummies) {
    dummie->update(ticks);
  }
  for ( Drawable* smartie : smarties ) {
    smartie->update( ticks );
  }
  player->update(ticks);

  clouds.update();
  //viewport.drawFPS(clock.getFps());
  viewport.update(); // always update viewport last
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
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        /*if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }*/
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
      delayCount++;
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
