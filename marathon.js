"use strict"

var http = require('http');

module.exports = class Marathon {

  /**************************/
  /*         Setup          */
  /**************************/

  constructor(ip, port=8080) {
    this._options = {}
    this._options.host = ip;
    this._options.port = port;
  }

  /**************************/
  /*        Internal        */
  /**************************/

  _callback(response) {
    var msg = ''

    response.on('data', function (chunk) {
      msg += chunk;
    });

    response.on('end', function() {
      console.log(msg);
    });
  }

  /**************************/
  /*         Apps           */
  /**************************/

  // NB All "params" arguments must be a properly formatted string (see
  // mesosphere.github.io/marathon/docs/rest-api.html for details).

  createApp(app) {
    this._options.path = '/v2/apps';
    this._options.method = 'POST';
    var req = http.request(this._options, this._callback);
    req.write(app);
    req.end();
  }

  listAllApps(params='') {
    this._options.path = '/v2/apps' + params;
    http.request(this._options, this._callback).end();
  }

  listApp(appid, params='') {
    this._options.path = '/v2/apps/' + appid + params;
    http.request(this._options, this._callback).end();
  }

  listAppVersions(appid) {
    this._options.path = '/v2/apps/' + appid + '/versions';
    http.request(this._options, this._callback).end();
  }

  listAppConfig(appid, version) {
    this._options.path = '/v2/apps/' + appid + '/versions/' + version;
    http.request(this._options, this._callback).end();
  }

  changeAppConfig(appid, config, params='') {
    this._options.path = '/v2/apps/' + appid + params;
    this._options.method = 'PUT';
    var req = http.request(this._options, this._callback);
    req.write(config);
    req.end();
  }

  restartTasks(appid, params='') {
    this._options.path = '/v2/apps/' + appid + '/restart' + params;
    this._options.method = 'POST';
    http.request(this._options, this._callback).end();
  }

  destroyApp(appid) {
    this._options.path = '/v2/apps/' + appid;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  listAppTasks(appid) {
    this._options.path = '/v2/apps/' + appid + '/tasks';
    http.request(this._options, this._callback).end();
  }

  killAppTasks(appid, params='') {
    this._options.path = '/v2/apps/' + appid + '/tasks' + params;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  killTask(appid, taskid, params='') {
    this._options.path = '/v2/apps/' + appid + '/tasks/' + taskid + params;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  /**************************/
  /*        Groups          */
  /**************************/

  listAllGroups() {
    this._options.path = '/v2/groups';
    http.request(this._options, this._callback).end();
  }

  listGroup(groupid) {
    this._options.path = '/v2/groups/' + groupid;
    http.request(this._options, this._callback).end();
  }

  createGroup(group) {
    this._options.path = '/v2/groups';
    this._options.method = 'POST';
    var req = http.request(this._options, this._callback);
    req.write(group);
    req.end();
  }

  changeGroupConfig(groupid, config, params='') {
    this._options.path = '/v2/groups/' + groupid + params;
    this._options.method = 'PUT';
    var req = http.request(this._options, this._callback);
    req.write(config);
    req.end();
  }

  destroyGroup(groupid) {
    this._options.path = '/v2/groups/' + groupid;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

}
