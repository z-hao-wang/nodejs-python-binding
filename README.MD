# Summary
This project provides the binding for node to call python method directly, via json stringified args.

examples js code
```javascript
const py = require('bindings')('py');
const pyInstance = new py.PythonCode('main');

const ret1 = pyInstance.callPy(JSON.stringify({func: 'init', value: 3}));
console.log('got python return after init', ret1);
// got python return after init {"ret": 3}
const ret2 = pyInstance.callPy(JSON.stringify({func: 'plus', value: 2}));
console.log('got python return after plus', ret2);
// got python return after plus {"ret": 5}
```

python code, a counter class can keep local state.

```python
import json
class Counter:

    def __init__(self, initialValue):
        self.value = initialValue

    def plus(self, value):
        self.value = self.value + value

counterInstance = Counter(0)
def callPy(arg):
    print("call Py", arg)
    argJson = json.loads(arg)
    func = argJson["func"]
    value = argJson["value"]
    global counterInstance
    if func == 'init':
      counterInstance = Counter(value)
    if func == 'plus':
      counterInstance.plus(value)
    return json.dumps({ "ret": counterInstance.value });

```

This can enable node directly call python methods just like calling a regular javascript function. no need to do callback or async.
The json stringified args are the easiest way to pass args though. 
If performance is a big concern, can add new functions in c++ binding in order to customize the arg data type.

## build project
```
docker build -t python-binding:master .
```

## run it
```
docker run --rm -v $(pwd)/src/pythonSrc:/app/src/pythonSrc python-binding:master
```
