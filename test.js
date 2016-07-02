var Marathon = require('./marathon.js');
var fs = require('fs');
var rs = fs.createReadStream('myapp.json');
var myapp = '';

rs.on('data', function(data) {
    myapp = myapp.concat(data);
});

rs.on('end', function() {
  var marathon = new Marathon('192.168.33.10');
  //marathon.createApp(myapp);
  //marathon.listAllApps();
  //marathon.listApp('myapp');
  //marathon.listAppVersions('myapp');
  //marathon.listAppConfig('myapp', '2016-07-01T04:44:39.318Z');
  //marathon.changeAppConfig('myapp', '{\"instances\":4}');
  //marathon.restartTasks('myapp');
  //marathon.destroyApp('myapp');
  //marathon.listAppTasks('myapp');
  //marathon.killAppTasks('myapp', '?scale=true');
  //marathon.killTask('myapp', 'myapp.38618152-3f4b-11e6-a681-08002760a44c');
  //marathon.listAllGroups();
  //marathon.listGroup('/');
  //marathon.createGroup('{\"id\":\"/1\"}');
  //marathon.changeGroupConfig('/1', '{\"id\":\"2\"}');
  //marathon.destroyGroup('/1/2');
  //marathon.listAllTasks();
  //marathon.killTasks('{\"ids\":[\"myapp.38615a41-3f4b-11e6-a681-08002760a44c\"]}');
  //marathon.listAllDeployments();
  //marathon.cancelDeployment('30e7bc33-aee4-48b9-a2a2-2e7900b8bfa8');
  //marathon.attachEventStream();
  //marathon.registerCallbackURL();
  //marathon.listCallbackURLs();
  marathon.unregisterCallbackURL();
});
