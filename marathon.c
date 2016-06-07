#include "marathon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static char* ipaddr;
static char* port;

/**************************/
/*         Setup          */
/**************************/

void set_destination(char *ip, char *pt) {
  ipaddr = ip;
  port = pt;
}

/**************************/
/*       Internal         */
/**************************/

size_t write_func(char *ptr, size_t size, size_t nmemb, Response *resp)
{
  size_t write_len = size * nmemb;

  if (write_len > resp->max_len) {
    resp->message = realloc(resp->message, write_len + 1);

    if (resp->message == NULL) {
      fprintf(stderr, "realloc() failed\n");
      exit(EXIT_FAILURE);
    }
    resp->max_len = write_len;
  }
  memcpy(resp->message, ptr, write_len);
  resp->message[write_len] = '\0';
  return write_len;
}

int send_http_get(char *uri, Response *resp)
{
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

int send_http_post(char *uri, char *data, Response *resp)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist* headerlist;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    headerlist = curl_slist_append(NULL, "Content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

int send_http_put(char *uri, char *data, Response *resp) {
  CURL *curl;
  CURLcode res;
  struct curl_slist* headerlist;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    headerlist = curl_slist_append(NULL, "Content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

int send_http_delete(char *uri, Response *resp) {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

/**************************/
/*         Apps           */
/**************************/

// NB All "params" arguments must be a properly formatted string (see
// mesosphere.github.io/marathon/docs/rest-api.html for details). If no extra
// parameters are required simply pass an empty string to the function.

int create_app(char *app, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps", ipaddr, port);
  send_http_post(uri, app, resp);
  return 0;
}

int list_all_apps(char* params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps%s", ipaddr, port, params);
  send_http_get(uri, resp);
  return 0;
}

int list_app(char* aid, char *params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s%s", ipaddr, port, aid, params);
  send_http_get(uri, resp);
  return 0;
}

int list_app_versions(char* aid, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s/versions", ipaddr, port, aid);
  send_http_get(uri, resp);
  return 0;
}

int list_app_config(char* aid, char* version, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s/versions/%s", ipaddr, port, aid, version);
  send_http_get(uri, resp);
  return 0;
}

int change_app_config(char *aid, char *config, char *params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s%s", ipaddr, port, aid, params);
  send_http_put(uri, config, resp);
  return 0;
}

int restart_tasks(char *aid, char *params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s/restart%s", ipaddr, port, aid, params);
  send_http_post(uri, "", resp);
  return 0;
}

int destroy_app(char *aid, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s", ipaddr, port, aid);
  send_http_delete(uri, resp);
  return 0;
}

int list_app_tasks(char *aid, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s/tasks", ipaddr, port, aid);
  send_http_get(uri, resp);
  return 0;
}

int kill_app_tasks(char *aid, char* params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s/tasks%s", ipaddr, port, aid, params);
  send_http_delete(uri, resp);
  return 0;
}

int kill_task(char *aid, char *tid, char *params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/apps/%s/tasks/%s%s", ipaddr, port, aid, tid, params);
  send_http_delete(uri, resp);
  return 0;
}

/**************************/
/*        Groups          */
/**************************/

int list_all_groups(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/groups", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}

int list_group(char* gid, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/groups/%s", ipaddr, port, gid);
  send_http_get(uri, resp);
  return 0;
}

int create_group(char *group, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/groups", ipaddr, port);
  send_http_post(uri, group, resp);
  return 0;
}

int change_group_config(char *gid, char *config, char *params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/groups/%s%s", ipaddr, port, gid, params);
  send_http_put(uri, config, resp);
  return 0;
}

int destroy_group(char *gid, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/groups/%s", ipaddr, port, gid);
  send_http_delete(uri, resp);
  return 0;
}

/**************************/
/*         Tasks          */
/**************************/

int list_all_tasks(char* params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/tasks%s", ipaddr, port, params);
  send_http_get(uri, resp);
  return 0;
}

int kill_tasks(char *tasks, char* params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/tasks/delete%s", ipaddr, port, params);
  send_http_post(uri, tasks, resp);
  return 0;
}

/**************************/
/*      Deployments       */
/**************************/

int list_all_deployments(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/deployments", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}

int cancel_deployment(char *did, char* params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/deployments/%s%s", ipaddr, port, did, params);
  send_http_delete(uri, resp);
  return 0;
}

/**************************/
/*      Event Stream      */
/**************************/

int attach_event_stream(Response *resp)
{
  CURL *curl;
  CURLcode res;
  char uri[MAX_URI_LEN];
  struct curl_slist* headerlist;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    sprintf(uri, "%s:%s/v2/events", ipaddr, port);
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);

    headerlist = curl_slist_append(NULL, "Content-type: application/json");
    headerlist = curl_slist_append(headerlist, "Accept: text/event-stream");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

/**************************/
/*   Event Subscriptions  */
/**************************/

//NB To use these functions start Marathon with --event_subscriber http_callback

int register_callback_url(char *params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/eventSubscriptions%s", ipaddr, port, params);
  send_http_post(uri, NULL, resp);
  return 0;
}

int list_callback_urls(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/eventSubscriptions", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}

int unregister_callback_url(char* params, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/eventSubscriptions%s", params, ipaddr, port);
  send_http_delete(uri, resp);
  return 0;
}

/**************************/
/*         Queue          */
/**************************/

int show_queue_content(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/queue", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}

int reset_launch_delay(char* aid, Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/queue%s/delay", aid, ipaddr, port);
  send_http_delete(uri, resp);
  return 0;
}

/**************************/
/*      Server Info       */
/**************************/

int server_info(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/info", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}

int current_leader(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/leader", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}

int leader_abdicate(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/v2/leader", ipaddr, port);
  send_http_delete(uri, resp);
  return 0;
}

/**************************/
/*      Micellaneous      */
/**************************/

int ping(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/ping", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}

int metrics(Response *resp)
{
  char uri[MAX_URI_LEN];
  sprintf(uri, "%s:%s/metrics", ipaddr, port);
  send_http_get(uri, resp);
  return 0;
}
