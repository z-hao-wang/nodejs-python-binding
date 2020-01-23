#include <napi.h>
#include "pythonBinding/pythoncode.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return PythonCode::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
