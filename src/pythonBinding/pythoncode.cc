#include "pythoncode.h"
#include <iostream>
#include <Python.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace Napi;
inline char* pyObjectToString(PyObject * pValue) {
  PyObject * tempBytes = PyUnicode_AsEncodedString(pValue, "UTF-8", "strict");
    if (tempBytes != NULL) {
        char* charRes = PyBytes_AS_STRING(tempBytes); // Borrowed pointer
        charRes = strdup(charRes);
        Py_DECREF(tempBytes);
        return charRes;
    } else {
        fprintf(stderr, "parse pyObjec to string failed \n");
    }
}

Napi::FunctionReference PythonCode::constructor;

Napi::Object PythonCode::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "PythonCode",
                  {
                   InstanceMethod("callPy", &PythonCode::CallPy)
                   });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
  exports.Set("PythonCode", func);
  return exports;
}

PythonCode::PythonCode(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PythonCode>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length == 0 || !info[0].IsString()) {
    Napi::TypeError::New(env, "path expected").ThrowAsJavaScriptException();
    return;
  }
  Py_UnbufferedStdioFlag = 1; // force line_buffering for _all_ I/O
  // https://stackoverflow.com/questions/49784583/numpy-import-fails-on-multiarray-extension-library-when-called-from-embedded-pyt
  void *handle = dlopen("libpython3.7m.so", RTLD_LAZY | RTLD_GLOBAL);
  if (!handle) {
    std::cout << "dlopen libpython3.7m failed!\n";
  }
  Py_Initialize();
  if (!Py_IsInitialized())
  {
      std::cout << "Python initialization failed!\n";
  }
  // ref https://docs.python.org/2/extending/embedding.html
  std::string arg1 = info[0].ToString().Utf8Value();
  const char* pythonFile = arg1.c_str();

  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));

  PyObject* pName = PyUnicode_DecodeFSDefault(pythonFile);
  PyObject* sysPath = PySys_GetObject((char*)"path");
  std::string baseCwd (cwd);
  std::string relativePath ("/src/pythonSrc");
  std::string newBasePath = baseCwd + relativePath;
  printf("init pythonFile %s %s \n", newBasePath.c_str(), pythonFile);
  // we must append the base directory to python env, otherwise it can't find our file.
  PyObject* basePathName = PyUnicode_DecodeFSDefault(newBasePath.c_str());
  PyList_Append(sysPath, basePathName);

  this->pModule = PyImport_Import(pName);
  Py_DECREF(pName);
  Py_DECREF(sysPath);
  Py_DECREF(basePathName);
  if (this->pModule == NULL) {
    PyErr_Print();
    fprintf(stderr, "Cannot find file \"%s\"\n", pythonFile);
    // Napi::TypeError::New(env, "python file not found").ThrowAsJavaScriptException();
  }
}

Napi::Value PythonCode::CallPy(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string arg1 = info[0].ToString().Utf8Value();
  PyObject *pFunc = PyObject_GetAttrString(pModule, (char*)"callPy");
  PyObject *pValue;
  if (pFunc && PyCallable_Check(pFunc)) {
    PyObject *pArgs = PyTuple_New(1);
    pValue = PyUnicode_FromString(arg1.c_str());
    PyTuple_SetItem(pArgs, 0, pValue);

    pValue = PyObject_CallObject(pFunc, pArgs);
    String ret = String::New(info.Env(), pyObjectToString(pValue));
    return ret;
    Py_DECREF(pValue);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
  } else {
    fprintf(stderr, "callPy function is not defined\n");
  }
  Array ret = Array::New(info.Env(), 0);
  return ret;
}
