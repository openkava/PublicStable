
for (var j=0; j<40; j++) {
  console.log('mem: ' + ( process.memoryUsage().rss / 1024.0 / 1024.0 ).toFixed( 2 ) + 'MB');
  console.log('starting loop ' + j + '...');
  c = require('Fabric').createClient();
  for (var i=0; i<10000; i++) {
    c.DG.createNode("foo"+i);
  }
  console.log('flushing...');
  c.flush();
  console.log('closing...');
  c.close();
  delete c;
}

