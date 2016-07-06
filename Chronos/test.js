var Chronos = require('./chronos.js');
var fs = require('fs');
var rs = fs.createReadStream('./myjob.json');
var myjob = '';

rs.on('data', function(data) {
    myjob = myjob.concat(data);
});

rs.on('end', function() {
  var chronos = new Chronos('192.168.33.10');
  //chronos.createJob(myjob);
  //chronos.listAllJobs();
  //chronos.deleteJob("myjob");
  chronos.getDependencyGraph();

});
