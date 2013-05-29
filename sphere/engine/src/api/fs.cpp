#include <v8.h>
#include "helpers.h"
#include "api.h"
#include "files.h"

using namespace v8;
using namespace std;

namespace API {
	namespace fs {
		void Init(Local<Object> global) {
			Handle<Object> fs = Object::New();
			global->Set(String::New("fs"), fs);
			
			fs->Set(String::New("readTextFile"), V8_FUNC(API::fs::readTextFile
														 ));

		}
		
		Handle<Value> readTextFile(const Arguments& args) {
			String::Utf8Value utf8_value(args[0]);
			const char* filename = string(*utf8_value).c_str();
			const char* text = Files::readTextFile(filename);
			
			return String::New(text);
		}
	}
}