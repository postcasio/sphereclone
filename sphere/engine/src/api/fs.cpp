#include <v8.h>
#include <string>
#include <iostream>
#include <boost/format.hpp>
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
			
			fs->Set(String::New("readTextFile"), V8_FUNC(API::fs::readTextFile));
			fs->Set(String::New("path_separator"), String::New(PATH_SEPARATOR));

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
	}
}