const py = require('bindings')('py');
const pyInstance = new py.PythonCode('main');

const ret1 = pyInstance.callPy(JSON.stringify({func: 'init', value: 3}));
console.log('got python return after init', ret1);

const ret2 = pyInstance.callPy(JSON.stringify({func: 'plus', value: 2}));
console.log('got python return after plus', ret2);
