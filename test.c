#include "libmarathon.h"

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static Response resp;

char *read_file(char *filename);

int main(int argc, char **argv)
{
  set_destination("192.168.33.10", "8080");

  char* myapp = read_file("myapp.json");
  create_app(myapp, &resp);
  // list_all_apps("", &resp);
  // list_app("myapp", "", &resp);
  // list_app_versions("myapp", &resp);
  // list_app_config("myapp", "2016-03-25T19:53:46.939Z", &resp);
  // change_app_config("myapp", "{\"instances\":4}", "", &resp);
  // restart_tasks("myapp", "", &resp);
  // destroy_app("myapp", &resp);
  // list_app_tasks("myapp", &resp);
  // kill_app_tasks("myapp", "", &resp);
  // kill_task("myapp", "myapp.94e700cf-f2fe-11e5-a0a5-0800272e2a42", "?scale=true", &resp);

  //list_all_groups(&resp);
  //list_group("/", &resp);
  //create_group("{\"id\":\"002\"}", &resp);
  //change_group_config("002", "{\"id\":\"003\"}", &resp);
  //destroy_group("002", &resp);

  //list_all_tasks(&resp);
  //kill_tasks("{\"ids\":[\"myapp.94e6d9be-f2fe-11e5-a0a5-0800272e2a42\"]}", "?scale=true",&resp);

  //list_all_deployments(&resp);
  //cancel_deployment("64e66b2d-31c6-40c0-9680-661b3d070fbe", "", &resp);

  //attach_event_stream(&resp);

  printf("%s\n", resp.message);
  return EXIT_SUCCESS;
}

char *read_file(char *filename)
{
  long size;
  char *fbuffer;
  FILE *fp = fopen(filename, "rb");

  if (!fp)
    fprintf(stderr,"Error opening file\n");

  fseek(fp , 0L , SEEK_END);
  size = ftell(fp);
  fbuffer = calloc(1, size+1);
  rewind(fp);

  if (!fbuffer) {
    fclose(fp);
    fprintf(stderr,"Memory allocation failed\n");
  }
  if (1 != fread(fbuffer, size, 1 , fp) ) {
    fclose(fp);
    free(fbuffer);
    fprintf(stderr,"Error reading file\n");
  }
  fclose(fp);
  return fbuffer;
}
