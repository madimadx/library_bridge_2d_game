// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

//ImageFactory*  ImageFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   bool keepPlaying = true;
   try {
     Engine* engine = new Engine;
     engine->menu();
     keepPlaying = engine->play();
     delete engine;
     while ( keepPlaying ) {
       Engine* engine = new Engine;
       keepPlaying = engine->play();
       delete engine;
     }
     delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
