#ifndef LIB_MARATHON_H
#define LIB_MARATHON_H

#include <stddef.h>

#define MAX_URI_LEN 128

typedef struct {
  char* message;
  size_t max_len;
} Response;

/**************************/
/*         Setup          */
/**************************/

void set_destination(char *ip, char *pt);

/**************************/
/*         Apps           */
/**************************/

int create_app(char *app, Response *resp);

int list_all_apps(char *params, Response *resp);

int list_app(char *aid, char *params, Response *resp);

int list_app_versions(char* aid, Response *resp);

int list_app_config(char *aid, char *version, Response *resp);

int change_app_config(char *aid, char *config, char *params, Response *resp);

int restart_tasks(char *aid, char *params, Response *resp);

int destroy_app(char *aid, Response *resp);

int list_app_tasks(char *aid, Response *resp);

int kill_app_tasks(char *aid, char *params, Response *resp);

int kill_task(char *aid, char *tid, char *params, Response *resp);

/**************************/
/*        Groups          */
/**************************/

int list_all_groups(Response *resp);

int list_group(char *gid, Response *resp);

int create_group(char *group, Response *resp);

int change_group_config(char *gid, char *config, char* params, Response *resp);

int destroy_group(char *gid, Response *resp);

/**************************/
/*         Tasks          */
/**************************/

int list_all_tasks(char *params, Response *resp);

int kill_tasks(char *tasks, char *params, Response *resp);

/**************************/
/*      Deployments       */
/**************************/

int list_all_deployments(Response *resp);

int cancel_deployment(char *did, char *params, Response *resp);

/**************************/
/*      Event Stream      */
/**************************/

int attach_event_stream(Response *resp);

/**************************/
/*   Event Subscriptions  */
/**************************/

int register_callback_url(char *params, Response *resp);

int list_callback_urls(Response *resp);

int unregister_callback_url(char* params, Response *resp);

/**************************/
/*         Queue          */
/**************************/

int show_queue_content(Response *resp);

int reset_launch_delay(char* aid, Response *resp);

/**************************/
/*      Server Info       */
/**************************/

int server_info(Response *resp);

int current_leader(Response *resp);

int abdicate_leader(Response *resp);

/**************************/
/*      Micellaneous      */
/**************************/

int ping(Response *resp);

int metrics(Response *resp);

#endif
