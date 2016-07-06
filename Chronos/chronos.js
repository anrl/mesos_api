"use strict"

var http = require('http');

module.exports = class Chronos {

  /**************************/
  /*         Setup          */
  /**************************/

  constructor(ip, port=4400) {
    this._options = {
      host: ip,
      port: port,
      headers: {'content-type': 'application/json'}
    };
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
      console.log(this.statusCode);
      console.log(str);
    });
  }

  /**************************/
  /*         Jobs           */
  /**************************/

  // NB All "params" arguments must be a properly formatted string (see
  // mesosphere.github.io/chronos/docs/api.html for details).

  listAllJobs() {
    this._options.path = '/scheduler/jobs';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  deleteJob(jobName) {
    this._options.path = '/scheduler/job/' + jobName;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  deleteAllTasks(jobName) {
    this._options.path = '/scheduler/task/kill/' + jobName;
    this._options.method = 'DELETE';
    http.request(this._options, this._callback).end();
  }

  startJob(jobName, params='') {
    this._options.path = '/scheduler/job/' + jobName + params;
    this._options.method = 'PUT';
    http.request(this._options, this._callback).end();
  }

  createScheduledJob(job) {
    this._options.path = '/scheduler/iso8601';
    this._options.method = 'POST';
    var req = http.request(this._options, this._callback);
    req.write(job);
    req.end();
  }

  createDependentJob(job) {
    this._options.path = '/scheduler/dependency';
    this._options.method = 'POST';
    var req = http.request(this._options, this._callback);
    req.write(job);
    req.end();
  }

  // Requires Cassandra to be present in the cluster
  updateTaskProgress(jobName, taskid, numElements) {
    this._options.path = '/scheduler/job/' + jobName + '/task/' + taskid + '/progress';
    this._options.method = 'POST';
    var req = http.request(this._options, this._callback);
    req.write('{"numAdditionalElementsProcessed":' + numElements + '}');
    req.end();
  }

  getDependencyGraph() {
    this._options.path = '/scheduler/graph/dot';
    this._options.method = 'GET';
    http.request(this._options, this._callback).end();
  }

  reportJobComplete(taskid) {
    this._options.path = '/scheduler/task/' + taskid;
    this._options.method = 'PUT';
    var req = http.request(this._options, this._callback);
    req.write('{"statusCode": 0}');
    req.end();
  }
}
