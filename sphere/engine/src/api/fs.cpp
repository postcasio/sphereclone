#include <v8.h>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <SDL/sdl.h>

#include "helpers.h"
#include "api.h"
#include "files.h"

using namespace v8;
using namespace std;

namespace API {
	namespace fs {
	
		v8::Handle<v8::FunctionTemplate> File;
		v8::Handle<v8::ObjectTemplate> FileInstanceTemplate;
		v8::Handle<v8::ObjectTemplate> FileProto;
		v8::Handle<v8::FunctionTemplate> FileConstructor;
		
		v8::Handle<v8::FunctionTemplate> ByteArray;
		v8::Handle<v8::ObjectTemplate> ByteArrayInstanceTemplate;
		v8::Handle<v8::ObjectTemplate> ByteArrayProto;
		v8::Handle<v8::FunctionTemplate> ByteArrayConstructor;
	
		void Init(Local<Object> global) {
			Handle<Object> fs = Object::New();
			global->Set(String::New("fs"), fs);
			
			fs->Set(String::New("readTextFile"), V8_FUNC(API::fs::readTextFile));
			fs->Set(String::New("path_separator"), String::New(PATH_SEPARATOR));
			
			File = FunctionTemplate::New();
			FileInstanceTemplate = File->InstanceTemplate();
			FileProto = File->PrototypeTemplate();
			FileProto->Set(String::New("read"), V8_FUNC(File_read));
			FileInstanceTemplate->SetInternalFieldCount(1);
			
			FileConstructor = FunctionTemplate::New(File_constructor);
			fs->Set(String::New("File"), FileConstructor->GetFunction());
			
			ByteArray = FunctionTemplate::New();
			ByteArrayInstanceTemplate = ByteArray->InstanceTemplate();
			ByteArrayProto = ByteArray->PrototypeTemplate();
			ByteArrayInstanceTemplate->SetInternalFieldCount(1);
			ByteArrayInstanceTemplate->SetIndexedPropertyHandler(ByteArray_index);
			ByteArrayProto->Set(String::New("toString"), V8_FUNC(ByteArray_toString));
			ByteArrayProto->Set(String::New("toSurface"), V8_FUNC(ByteArray_toSurface));
		}
		
		Handle<Value> readTextFile(const Arguments& args) {
			String::Utf8Value utf8_value(args[0]);
			
			string filename(*utf8_value);
			const char* contents = Files::readTextFile(filename.c_str());
			
			if (contents == NULL) {
				boost::format msg_fmt("Error reading file %s");
				string msg = str(msg_fmt % filename);
				
				return ThrowException(String::New(msg.c_str()));
			}

			return String::New(contents);
		}
		
		SphereFile::SphereFile(const char* path) {
			this->path = path;
			this->file.open(path, ios::in | ios::binary);
		}
		
		SphereFile::~SphereFile() {
			this->file.close();
		}
		
		int SphereFile::read(uint8_t* data, int length) {
			this->file.read((char*)data, length);
			return 1;
		}
		
		SphereByteArray::SphereByteArray(uint8_t* data, int length) {
			this->data = data;
			this->length = length;
		}
		
		SphereByteArray::~SphereByteArray() {
			free(this->data);
		}
		
		Handle<Value> File_constructor(const Arguments &args) {
			if (!args.IsConstructCall()) {
				return ThrowException(String::New("You must use new to create a file."));
			}
		 
			HandleScope scope;
		 
			String::Utf8Value path(args[0]);
			SphereFile* spherefile = new SphereFile(*path);
		
			Local<Object> file = FileInstanceTemplate->NewInstance();
			
			file->SetAlignedPointerInInternalField(0, spherefile);

			return scope.Close(file);
		
		}
		
		Handle<Value> File_read(const Arguments &args) {
			HandleScope scope;
			
			SphereFile* file = static_cast<SphereFile*>(args.Holder()->GetAlignedPointerFromInternalField(0));
			
			int length = args[0]->Int32Value();
			
			uint8_t* data = (uint8_t*)malloc(length);
			
			file->read(data, length);
			
			Local<Object> bytearray = ByteArrayInstanceTemplate->NewInstance();
			
			SphereByteArray* ba = new SphereByteArray(data, length);
			
			bytearray->SetAlignedPointerInInternalField(0, ba);
			
			return scope.Close(bytearray);
		}

		Handle<Value> ByteArray_index(uint32_t index, const v8::AccessorInfo& info) {
			SphereByteArray* array = static_cast<SphereByteArray*>(info.Holder()->GetAlignedPointerFromInternalField(0));
			return Number::New(array->data[index]);
		}
		
		Handle<Value> ByteArray_toString(const Arguments &args) {
			SphereByteArray* array = static_cast<SphereByteArray*>(args.Holder()->GetAlignedPointerFromInternalField(0));
			
			char* result = (char*)malloc(array->length + 1);
			strncpy(result, (char*)	array->data, array->length + 1);
			return String::New(result);
		}
		
		Handle<Value> ByteArray_toSurface(const Arguments& args) {
			HandleScope scope;
			int width = args[0]->Int32Value();
			int height = args[1]->Int32Value();
			
			SphereByteArray* array = static_cast<SphereByteArray*>(args.Holder()->GetAlignedPointerFromInternalField(0));
			
			if (array->length < width * height * 4) {
				return ThrowException(String::New("Tried to create a surface with insufficient pixel data."));
			}
			
			
			SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, width, height, 32,
				RGBA_RMASK, RGBA_GMASK, RGBA_BMASK, RGBA_AMASK);
			
			SDL_LockSurface(surface);
			unsigned char* pixels = (unsigned char*)surface->pixels;
			
			for (long i = 0; i < width * height * 4; i++) {
				pixels[i] = array->data[i];
			}
			
			SDL_UnlockSurface(surface);
			
			Local<Object> surface_obj = API::graphics::SurfaceInstanceTemplate->NewInstance();
			
			surface_obj->SetAlignedPointerInInternalField(0, surface);

			return scope.Close(surface_obj);
		}
	}
}