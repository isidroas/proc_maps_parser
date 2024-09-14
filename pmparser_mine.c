#include "pmparser.h"

procmaps_iterator* pmparser_parse(int pid){

  char maps_path[50];
  // TODO: if pid< 0 -> %s self
  if(pid<0)
    pid = getpid();
  snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid );
  FILE * stream = fopen(maps_path, "r");
  if (stream == NULL){
    return NULL;
  }

  return _pmparser_parse_stream(stream);
   
}

// for easier testing
procmaps_iterator* _pmparser_parse_stream(FILE * stream){
  
  char line[PROCMAPS_LINE_MAX_LENGTH];

  while (fgets(line, sizeof(line), stream) !=NULL){
    // TODO: exclude endofline
    int ret = sscanf(line, "%d-%d %s %d %d %d %s", &begin, &end, perms, &offset, &dev, &inode, &pathname);
    if (ret<7){
      perror("failed scanf");
      return NULL;
    }
  }
}
