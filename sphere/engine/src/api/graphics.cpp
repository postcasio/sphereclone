#include <v8.h>
#include <SDL/sdl.h>

#include "helpers.h"
#include "api.h"
#include "files.h"

using namespace v8;
using namespace std;

namespace API {
	namespace graphics {
	
		Handle<FunctionTemplate> Surface;
		Handle<ObjectTemplate> SurfaceInstanceTemplate;
		Handle<ObjectTemplate> SurfaceProto;
		
		Handle<FunctionTemplate> Color;
		Handle<ObjectTemplate> ColorInstanceTemplate;
		Handle<ObjectTemplate> ColorProto;
		Handle<FunctionTemplate> ColorConstructor;
		
		SDL_Surface* screen;
		
		void Init(Local<Object> global) {
			Handle<Object> graphics = Object::New();
			global->Set(String::New("graphics"), graphics);
			
			graphics->Set(String::New("setVideoMode"), V8_FUNC(API::graphics::setVideoMode));
			graphics->Set(String::New("flipScreen"), V8_FUNC(API::graphics::flipScreen));
			
			Surface = FunctionTemplate::New();
			SurfaceInstanceTemplate = Surface->InstanceTemplate();
			SurfaceProto = Surface->PrototypeTemplate();
			SurfaceProto->Set(String::New("rectangle"), V8_FUNC(Surface_rectangle));
			SurfaceInstanceTemplate->SetInternalFieldCount(1);
			
			Color = FunctionTemplate::New();
			ColorInstanceTemplate = Color->InstanceTemplate();
			ColorProto = Color->PrototypeTemplate();
			
			ColorInstanceTemplate->SetAccessor(String::New("red"), Color_getRed, Color_setRed);
			ColorInstanceTemplate->SetAccessor(String::New("green"), Color_getGreen, Color_setGreen);
			ColorInstanceTemplate->SetAccessor(String::New("blue"), Color_getBlue, Color_setBlue);
			
			ColorInstanceTemplate->SetInternalFieldCount(1);
			
			ColorConstructor = FunctionTemplate::New(Color_constructor);
			graphics->Set(String::New("Color"), ColorConstructor->GetFunction());
		}
		
		Handle<Value> setVideoMode(const Arguments& args) {
			HandleScope scope;
			
			if (screen != NULL) {
				return ThrowException(String::New("Screen already initialized."));
			}
			
			if (args.Length() < 2) {
				return ThrowException(Exception::TypeError(String::New("SetVideoMode expects 2 arguments.")));
			}
			
			screen = SDL_SetVideoMode(args[0]->Uint32Value(), args[1]->Uint32Value(), 32, SDL_HWSURFACE);
			
			Local<Object> surface = SurfaceInstanceTemplate->NewInstance();
			
			surface->SetAlignedPointerInInternalField(0, screen);

			return scope.Close(surface);
		}
		
		Handle<Value> flipScreen(const Arguments& args) {
			if (screen == NULL) {
				return ThrowException(String::New("Screen has not been initialized."));
			}
			
			SDL_Flip(screen);
			
			return Undefined();
		}
		
		Handle<Value> Surface_rectangle(const Arguments& args) {
			SDL_Surface* surface = static_cast<SDL_Surface*>(args.Holder()->GetAlignedPointerFromInternalField(0));
			
			int x = args[0]->Int32Value();
			int y = args[1]->Int32Value();
			int w = args[2]->Int32Value();
			int h = args[3]->Int32Value();
			
			SDL_Rect rect = { x, y, w, h };
			SphereColor* color = static_cast<SphereColor*>(args[4]->ToObject()->GetAlignedPointerFromInternalField(0));
			
			SDL_FillRect(surface, &rect, color->ToUint32(surface->format));
			
			return True();
		}
		
		SphereColor::SphereColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
			this->red = red;
			this->green = green;
			this->blue = blue;
			this->alpha = alpha;
		}
		
		SphereColor::~SphereColor() {
		
		}
		
		uint32_t SphereColor::ToUint32(SDL_PixelFormat* pixel) {
			return SDL_MapRGBA(pixel, this->red, this->green, this->blue, this->alpha);
		}
		
		Handle<Value> Color_constructor(const Arguments &args) {
			if (!args.IsConstructCall()) {
				return ThrowException(String::New("You must use new to create a color."));
			}
		 
			HandleScope scope;
			
			uint8_t r = args[0]->Int32Value();
			uint8_t g = args[1]->Int32Value();
			uint8_t b = args[2]->Int32Value();
			uint8_t a = args[3]->Int32Value();
		 
			SphereColor* spherecolor = new SphereColor(r, g, b, a);
		
			Local<Object> color = ColorInstanceTemplate->NewInstance();
			
			color->SetAlignedPointerInInternalField(0, spherecolor);

			return scope.Close(color);
		
		}
		
		Handle<Value> Color_getRed(Local<String> property, const AccessorInfo &info) {
			SphereColor* color = static_cast<SphereColor*>(info.Holder()->GetAlignedPointerFromInternalField(0));
			return Integer::New(color->red);
		}
		
		void Color_setRed(Local<String> property, Local<Value> value, const AccessorInfo& info) {
			SphereColor* color = static_cast<SphereColor*>(info.Holder()->GetAlignedPointerFromInternalField(0));
			color->red = value->Int32Value();
		}
		
		Handle<Value> Color_getGreen(Local<String> property, const AccessorInfo &info) {
			SphereColor* color = static_cast<SphereColor*>(info.Holder()->GetAlignedPointerFromInternalField(0));
			return Integer::New(color->green);
		}
		
		void Color_setGreen(Local<String> property, Local<Value> value, const AccessorInfo& info) {
			SphereColor* color = static_cast<SphereColor*>(info.Holder()->GetAlignedPointerFromInternalField(0));
			color->green = value->Int32Value();
		}
		
		Handle<Value> Color_getBlue(Local<String> property, const AccessorInfo &info) {
			SphereColor* color = static_cast<SphereColor*>(info.Holder()->GetAlignedPointerFromInternalField(0));
			return Integer::New(color->blue);
		}
		
		void Color_setBlue(Local<String> property, Local<Value> value, const AccessorInfo& info) {
			SphereColor* color = static_cast<SphereColor*>(info.Holder()->GetAlignedPointerFromInternalField(0));
			color->blue = value->Int32Value();
		}
	}
}