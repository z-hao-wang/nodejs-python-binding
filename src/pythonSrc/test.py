import main

ret=main.callPy("{\"func\":\"init\",\"value\":3}")
print("ret1", ret)
ret=main.callPy("{\"func\":\"plus\",\"value\":2}")
print("ret2", ret)
