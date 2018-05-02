#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "smartSprite.h"
#include "hud.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "collisionStrategy.h"

Engine::~Engine() {
  //delete static_cast<ShootingSprite*>(dummies[0])
  delete sound;
  for (ShootingSprite* shooter: shooters) {
    for (Bullet* bullA : shooter->getBulletListActive())
      delete bullA;
    for (Bullet* bullI : shooter->getBulletListInactive())
      delete bullI;
    delete shooter;
  }
  for (Drawable* dummie: dummies) {
    delete dummie;
  }
  for ( Drawable* smartie : smarties ) {
    delete smartie;
  }
  delete theHud;
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
  sound(new SDLSound()),
  renderer( rc->getRenderer() ),
  viewport( Viewport::getInstance() ),
  bricks("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world("middle", Gamedata::getInstance().getXmlInt("middle/factor") ),
  clouds("top", Gamedata::getInstance().getXmlInt("top/factor") ),
  menuEngine(),
  dummies(),
  smarties(),
  player(new Player("Student")),
  strategies(),
  theHud(new Hud("Sticky")),
  currentStrategy(0),
  collision(false),
  hudOn(false),
  popUpDelay(Gamedata::getInstance().getXmlInt("popUpDelay")),
  delayCount(0),
  makeVideo( false )
{
  srand(time(NULL));
  std::cout << viewport.getX() << " " <<  viewport.getY() << std::endl;
  //std::cout << clouds.getX() << " " <<  clouds.getY() << std::endl;
  //std::cout << player->getX() << " " <<  player->getY() << std::endl;
  //Vector2f pos = (player->getPlayer())->getPosition();
  //int w = (player->getPlayer())->getScaledWidth();
  //int h = (player->getPlayer())->getScaledHeight();
/*
  for (int i = 0; i < 7; i++) {
    smarties.emplace_back(new SmartSprite("Paper", pos, w, h, float(rand()%350), float(rand()%400)));
    player->attach( smarties[i] );
  }
  */
  //dummies.emplace_back(new MultiSprite("Paper", float(rand()%350), float(rand()%400)));
  shooters.emplace_back( new ShootingSprite("OtherStudentR") );
  shooters.emplace_back( new ShootingSprite("OtherStudentL") );
  dummies.emplace_back( new Sprite("Table") );

  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  viewport.setObjectToTrack(player->getPlayer());
  playMenu();
  std::cout << "Loading complete" << std::endl;
}

void Engine::drawMenu() const {
  bricks.draw();
  IoMod::getInstance().writeText("Press any key to begin", 220, 110);
  SDL_RenderPresent(renderer);
}

void Engine::draw() const {
  bricks.draw();
  world.draw();
  //std::cout << player->getX() << " " <<  player->getY() << std::endl;
  player->draw();
  int active = 0, inactive = 0;
  std::stringstream strmAct, strmInact;

  for (ShootingSprite* shooter: shooters) {
    shooter->draw();
    active += shooter->getNumActive();
    inactive += shooter->getNumInactive();
  }
  strmAct << active;
  strmInact << inactive;
  string strAct, strInact;
  strmAct >> strAct;
  strmInact >> strInact;

  for (const Drawable* dummie : dummies) {
    dummie->draw();
  }
  for ( const Drawable* smartie : smarties ) {
    smartie->draw();
  }

  clouds.draw();

  if (player->isDeathOn()) IoMod::getInstance().writeText("Press any key to reset", 240, 400);
  IoMod::getInstance().writeText("Press m to change strategy", 30, 30);
  IoMod::getInstance().writeText("Active: " + strAct, 30, 140);
  IoMod::getInstance().writeText("Inactive: " + strInact, 30, 170);
  strategies[currentStrategy]->draw();
  if ( collision )
    IoMod::getInstance().writeText("Oops: Collision", 30, 110);

  viewport.draw();
  //viewport.drawFPS(clock.getFps());
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  if (!player->isDeathOn()) {
    auto itr = shooters.begin();
    while ( itr != shooters.end() ) {
      auto itrBullet = (*itr)->getBulletListActive().begin();
      while (itrBullet != (*itr)->getBulletListActive().end()) {
        if ( strategies[currentStrategy]->execute(*(player->getPlayer()), **itrBullet) ) {
          collision = true;
          (*sound)[0];
          resetDelay();
          itrBullet = (*itr)->getBulletListActive().erase(itrBullet);
          player->deathOn();
        }
        else ++itrBullet;
      }
      ++itr;
    }
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

  for (ShootingSprite* shooter: shooters) {
    shooter->update(ticks);
  }
  for (Drawable* dummie : dummies) {
    dummie->update(ticks);
  }
  for ( Drawable* smartie : smarties ) {
    smartie->update( ticks );
  }
  player->update(ticks);
  theHud->update(viewport.getX(), viewport.getY());
  clouds.update();
  //std::cout << clouds.getX() << " " <<  clouds.getY() << std::endl;
  //viewport.drawFPS(clock.getFps());
  viewport.update(); // always update viewport last
}

void Engine::menu() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  //menuEngine.play();

  while (!done) {
    while (SDL_PollEvent(&event)) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type == SDL_KEYDOWN) {
        return;
      }
    }

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      delayCount++;
      clock.incrFrame();
      drawMenu();
      //update(ticks);
    }
  }
}

void Engine::playMenu() {
  menuEngine.play(viewport.getX(), viewport.getY());
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    //std::cout << player->getX() << " " <<  player->getY() << std::endl;
    //std::cout << viewport.getX() << " " <<  viewport.getY() << std::endl;
    //if (player->deathReset()) return true;
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_M] || keystate[SDL_SCANCODE_O] ) {
          clock.pause();
          menuEngine.play(viewport.getX(), viewport.getY());
          int option = menuEngine.getOptionChoice();
          std::cout << "OPTION: " << option << std::endl;
          clock.unpause();
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
          sound->toggleMusic();
        }
        if ( keystate[indexPause] && !hudOn ) {
          IoMod::getInstance().writeText("Paused", 300, 400);
          SDL_RenderPresent(renderer);
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[indexCol] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if ( keystate[indexHUD] && hudOn) {
          hudOn = false;
          clock.unpause();
        }
        else if ( keystate[indexHUD] && !hudOn) {
          hudOn = true;
          theHud->draw();
          IoMod::getInstance().writeText("Paused", 300, 400);
          SDL_RenderPresent(renderer);
          clock.pause();
        }
        if (keystate[SDL_SCANCODE_R]) {
          return true;
        }
        if (keystate[SDL_SCANCODE_M]) {
          menu();
          return true;
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
      delayCount++;
      clock.incrFrame();
      if (keystate[indexL]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[indexR]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[indexUp]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[indexDown]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return !done;
}
