#include <v8.h>
#include <SDL/sdl.h>
#include "engine.h"
#include "helpers.h"
#include "api.h"
#include "files.h"

using namespace v8;
using namespace std;

namespace API {
	namespace engine {
		void Init(Local<Object> global) {
			Handle<Object> engine = Object::New();
			global->Set(String::New("engine"), engine);
			
			engine->Set(String::New("abort"), V8_FUNC(API::engine::abort));
			engine->Set(String::New("exit"), V8_FUNC(API::engine::exit));
			engine->Set(String::New("restart"), V8_FUNC(API::engine::restart));
			engine->Set(String::New("restartGame"), V8_FUNC(API::engine::restartGame));
			engine->Set(String::New("evalInContext"), V8_FUNC(API::engine::evalInContext));
			engine->Set(String::New("evalScript"), V8_FUNC(API::engine::evalScript));
			engine->Set(String::New("version"), String::New(ENGINE_VERSION));
			engine->Set(String::New("setWindowTitle"), V8_FUNC(API::engine::setWindowTitle));
		}
		
		Handle<Value> abort(const Arguments& args) {
			return ThrowException(args[0]->ToString());
		}
		
		Handle<Value> exit(const Arguments& args) {
			return ThrowException(String::New("Not implemented."));
		}
		
		Handle<Value> restart(const Arguments& args) {
			return ThrowException(String::New("Not implemented."));
		}
		
		Handle<Value> restartGame(const Arguments& args) {
			return ThrowException(String::New("Not implemented."));
		}
		
		Handle<Value> evalScript(const Arguments& args) {
			if (args.Length() < 2) {
				return ThrowException(Exception::TypeError(String::New("evalScript takes 2 arguments.")));
			}

			TryCatch tc;
			Local<Script> compiled = Script::Compile(args[0]->ToString(), args[1]->ToString());
			
			if (compiled.IsEmpty()) {
				return tc.ReThrow();
			}
			
			Handle<Value> value = compiled->Run();
			
			if (value.IsEmpty()) {
				return tc.ReThrow();
			}
			
			return value;
		
		}
		
		Handle<Value> evalInContext(const Arguments& args) {
			if (args.Length() < 3) {
				return ThrowException(Exception::TypeError(String::New("evalInContext takes 3 arguments.")));
			}
			
			if (!args[1]->IsObject()) {
				return ThrowException(Exception::TypeError(String::New("evalInContext expects an object as second argument.")));
			}
			
			HandleScope scope;
			
			Persistent<Context> context = Context::New();
			Context::Scope context_scope(context);

			Local<Object> global = context->Global();

			Local<Object> jscontext = args[1]->ToObject()->Clone();
			Local<Array> names = jscontext->GetOwnPropertyNames();
			for (int i = 0; i < names->Length(); i++) {
				global->Set(names->Get(i), jscontext->Get(names->Get(i)));
			}

			TryCatch tc;
			
			Local<Script> compiled = Script::Compile(args[0]->ToString(), args[2]->ToString());
			
			if (compiled.IsEmpty()) {
				return tc.ReThrow();
			}
			
			Handle<Value> value = compiled->Run();
			
			if (value.IsEmpty()) {
				return tc.ReThrow();
			}

			
			return scope.Close(value);
		}
		
		Handle<Value> setWindowTitle(const Arguments& args) {
			String::Utf8Value utf8_value(args[0]);
			SDL_WM_SetCaption(*utf8_value, NULL);
			
			return Undefined();
		}
	}
}