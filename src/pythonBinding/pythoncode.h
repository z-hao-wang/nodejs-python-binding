#ifndef PYTHON_CODE_H
#define PYTHON_CODE_H

#include <napi.h>
#include <Python.h>

class PythonCode : public Napi::ObjectWrap<PythonCode> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  PythonCode(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value CallPy(const Napi::CallbackInfo& info);

  PyObject *pModule;
};

#endif
