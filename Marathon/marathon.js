"use strict"

var http = require('http');

module.exports = class Marathon {

  /**************************/
  /*         Setup          */
  /**************************/

  constructor(ip, port=8080) {
    this._options = {};
    this._options.host = ip;
    this._options.port = port;
  }

  /**************************/
  /*        Internal        */
  /**************************/

  _callback(response) {
    var str = '';

    response.on('data', function (chunk) {
      str += chunk;
    });

    response.on('end', function() {
      console.log(str);
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
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  listApp(appid, params='') {
    this._options.path = '/v2/apps/' + appid + params;
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  listAppVersions(appid) {
    this._options.path = '/v2/apps/' + appid + '/versions';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  listAppConfig(appid, version) {
    this._options.path = '/v2/apps/' + appid + '/versions/' + version;
    this._options.method = 'GET';
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
    this._options.method = 'GET';
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
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  listGroup(groupid) {
    this._options.path = '/v2/groups/' + groupid;
    this._options.method = 'GET';
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

  /**************************/
  /*         Tasks          */
  /**************************/

  listAllTasks(params='') {
    this._options.path = '/v2/tasks' + params;
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  killTasks(tasks, params='') {
    this._options.path = '/v2/tasks/delete' + params;
    this._options.method = 'POST';
    var req = http.request(this._options, this._callback);
    req.write(tasks);
    req.end();
  }

  /**************************/
  /*      Deployments       */
  /**************************/

  listAllDeployments() {
    this._options.path = '/v2/deployments';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  cancelDeployment(deployid, params='') {
    this._options.path = '/v2/deployments/' + deployid + params;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  /**************************/
  /*      Event Stream      */
  /**************************/

  attachEventStream() {
    var options = {
      host: this._options.host,
      port: this._options.port,
      path: '/v2/events',
      headers: {'Accept': 'text/event-stream'}
    }
    http.request(options, this._callback).end();
  }

  /**************************/
  /*   Event Subscriptions  */
  /**************************/

  // NB To use these functions start Marathon
  // with --event_subscriber http_callback

  registerCallbackURL(params='') {
    this._options.path = '/v2/eventSubscriptions' + params;
    this._options.method = 'POST';
    http.request(this._options, this._callback).end();
  }

  listCallbackURLs() {
    this._options.path = '/v2/eventSubscriptions';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  unregisterCallbackURL(params='') {
    this._options.path = '/v2/eventSubscriptions' + params;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  /**************************/
  /*         Queue          */
  /**************************/

  showQueueContent() {
    this._options.path = '/v2/queue';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  resetLaunchDelay(appid) {
    this._options.path = '/v2/queue/' + appid + '/delay';
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  /**************************/
  /*      Server Info       */
  /**************************/

  serverInfo() {
    this._options.path = '/v2/info';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  currentLeader() {
    this._options.path = '/v2/leader';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  leaderAbdicate() {
    this._options.path = '/v2/leader';
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  /**************************/
  /*      Miscellaneous     */
  /**************************/

  ping() {
    this._options.path = '/ping';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  metrics() {
    this._options.path = '/metrics';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }
}
