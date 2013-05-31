#ifndef API_H_
#define API_H_

#include <v8.h>
#include <SDL/sdl.h>
#include <fstream>
#include "helpers.h"

#define PATH_SEPARATOR "/"

namespace API {
	namespace fs {
		extern v8::Handle<v8::FunctionTemplate> File;
		extern v8::Handle<v8::ObjectTemplate> FileInstanceTemplate;
		extern v8::Handle<v8::ObjectTemplate> FileProto;
		extern v8::Handle<v8::FunctionTemplate> FileConstructor;
		
		extern v8::Handle<v8::FunctionTemplate> ByteArray;
		extern v8::Handle<v8::ObjectTemplate> ByteArrayInstanceTemplate;
		extern v8::Handle<v8::ObjectTemplate> ByteArrayProto;
		extern v8::Handle<v8::FunctionTemplate> ByteArrayConstructor;
		
		void Init(v8::Local<v8::Object> global);
		
		v8::Handle<v8::Value> readTextFile(const v8::Arguments& args);
		
		class ALIGNED SphereFile {
			public:
				SphereFile(const char* path);
				~SphereFile();
				
				const char* path;
				std::ifstream file;
				
				int read(uint8_t* dest, int size);
		};
		
		class ALIGNED SphereByteArray {
			public:
				SphereByteArray(uint8_t* data, int length);
				~SphereByteArray();
				
				uint8_t* data;
				int length;
		};
		
		v8::Handle<v8::Value> File_constructor(const v8::Arguments& args);
		v8::Handle<v8::Value> File_read(const v8::Arguments& args);
		v8::Handle<v8::Value> ByteArray_index(uint32_t index, const v8::AccessorInfo& info);
		v8::Handle<v8::Value> ByteArray_toString(const v8::Arguments& args);
		v8::Handle<v8::Value> ByteArray_toSurface(const v8::Arguments& args);
	}
	namespace engine {
		void Init(v8::Local<v8::Object> global);
		
		v8::Handle<v8::Value> abort(const v8::Arguments& args);
		v8::Handle<v8::Value> exit(const v8::Arguments& args);
		v8::Handle<v8::Value> restart(const v8::Arguments& args);
		v8::Handle<v8::Value> restartGame(const v8::Arguments& args);
		v8::Handle<v8::Value> evalInContext(const v8::Arguments& args);
		v8::Handle<v8::Value> evalScript(const v8::Arguments& args);
		v8::Handle<v8::Value> setWindowTitle(const v8::Arguments& args);
		v8::Handle<v8::Value> log(const v8::Arguments& args);
	}
	namespace graphics {
		extern v8::Handle<v8::FunctionTemplate> Surface;
		extern v8::Handle<v8::ObjectTemplate> SurfaceInstanceTemplate;
		extern v8::Handle<v8::ObjectTemplate> SurfaceProto;
		
		extern v8::Handle<v8::FunctionTemplate> Color;
		extern v8::Handle<v8::ObjectTemplate> ColorInstanceTemplate;
		extern v8::Handle<v8::ObjectTemplate> ColorProto;
		extern v8::Handle<v8::FunctionTemplate> ColorConstructor;
		
		void Init(v8::Local<v8::Object> global);
		
		class ALIGNED SphereColor {
			public:
				SphereColor(uint8_t, uint8_t, uint8_t, uint8_t);
				~SphereColor();
				
				uint32_t ToUint32(SDL_PixelFormat* surface);
				
				uint8_t red;
				uint8_t green;
				uint8_t blue;
				uint8_t alpha;
		};
		
		v8::Handle<v8::Value> setVideoMode(const v8::Arguments& args);
		v8::Handle<v8::Value> flipScreen(const v8::Arguments& args);
		v8::Handle<v8::Value> Surface_rectangle(const v8::Arguments& args);
		v8::Handle<v8::Value> Surface_blitSurface(const v8::Arguments& args);
		v8::Handle<v8::Value> Color_constructor(const v8::Arguments& args);
		v8::Handle<v8::Value> Color_getRed(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		void Color_setRed(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		v8::Handle<v8::Value> Color_getGreen(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		void Color_setGreen(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		v8::Handle<v8::Value> Color_getBlue(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		void Color_setBlue(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	}
	namespace events {
		void Init(v8::Local<v8::Object> global);
		
		v8::Handle<v8::Value> poll(const v8::Arguments& args);
	}
}

#endif