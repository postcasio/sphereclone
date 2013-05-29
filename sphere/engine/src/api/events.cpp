#include <v8.h>
#include <SDL/sdl.h>

#include "helpers.h"
#include "api.h"

using namespace v8;

namespace API {
	namespace events {
		void Init(Local<Object> global) {
			Handle<Object> fs = Object::New();
			global->Set(String::New("events"), fs);
			
			fs->Set(String::New("poll"), V8_FUNC(API::events::poll));

		}
		
		Handle<Value> poll(const Arguments& args) {
			SDL_Event event;
			
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						printf("Quitting!\n");
						exit(0);
						
						break;
						
					default:
						break;
				}
			}
			
			return True();
		}
	}
}