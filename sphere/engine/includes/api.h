#ifndef API_H_
#define API_H_

#include <v8.h>
namespace API {
	namespace fs {
		void Init(v8::Local<v8::Object> global);
		
		v8::Handle<v8::Value> readTextFile(const v8::Arguments& args);
	}
	namespace engine {
		void Init(v8::Local<v8::Object> global);
		
		v8::Handle<v8::Value> abort(const v8::Arguments& args);
		v8::Handle<v8::Value> exit(const v8::Arguments& args);
		v8::Handle<v8::Value> restart(const v8::Arguments& args);
		v8::Handle<v8::Value> restartGame(const v8::Arguments& args);
		v8::Handle<v8::Value> evalInContext(const v8::Arguments& args);
		v8::Handle<v8::Value> evalScript(const v8::Arguments& args);
	}
	namespace graphics {
		void Init(v8::Local<v8::Object> global);
		
		class SphereColor {
			public:
				SphereColor(uint8_t, uint8_t, uint8_t, uint8_t);
				~SphereColor();
				
				uint32_t ToUint32();
				
				uint8_t red;
				uint8_t green;
				uint8_t blue;
				uint8_t alpha;
		};
		
		v8::Handle<v8::Value> setVideoMode(const v8::Arguments& args);
		v8::Handle<v8::Value> flipScreen(const v8::Arguments& args);
		v8::Handle<v8::Value> Surface_rectangle(const v8::Arguments& args);
		v8::Handle<v8::Value> Color_constructor(const v8::Arguments& args);
		v8::Handle<v8::Value> Color_getRed(v8::Local<v8::String> property, const v8::AccessorInfo &info);
		void Color_setRed(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	}	
}
#endif