#include <v8.h>
#include <string>
#include <SDL/sdl.h>

#include "engine.h"
#include "helpers.h"
#include "api.h"
#include "files.h"

using namespace v8;
using namespace std;

Isolate* isolate;

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	V8::Initialize();
	isolate = Isolate::GetCurrent();
	HandleScope handle_scope(isolate);
	
	Persistent<Context> context = Context::New();
    Context::Scope context_scope(context);

	Local<Object> global = context->Global();
	
	Local<Object> sphere = Object::New();
	
	global->Set(String::New("sphere"), sphere);
	
	API::fs::Init(sphere);
	API::engine::Init(sphere);
	API::graphics::Init(sphere);
	
	//sphere->Get(String::New("engine"))->ToObject()->Set(String::New("activeGame"), String::New(argv[1]));
	
	const char* gameScript = Files::readTextFile("system/sphere.js");
	
	if (gameScript == NULL) {
		debug("Error loading bootstrap script.\n");
		exit(1);
	}
	
	TryCatch trycatch;
	Local<Script> compiled = Script::Compile(String::New(gameScript), String::New("system/sphere.js"));
	
	if (compiled.IsEmpty()) {
		Handle<Value> exception = trycatch.Exception();
		String::Utf8Value exception_str(exception);
		printf("Exception: %s\n", *exception_str);
		exit(1);
	}
	
	Handle<Value> value = compiled->Run();
	
	if (value.IsEmpty()) {
		Handle<Value> exception = trycatch.Exception();
		String::Utf8Value exception_str(exception);
		printf("Exception: %s\n", *exception_str);
		exit(1);
	}
	
	return 0;
}