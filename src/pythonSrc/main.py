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
