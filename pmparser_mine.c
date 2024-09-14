#include "pmparser.h"

// for easier testing
procmaps_iterator* _pmparser_parse_stream(FILE * stream){
  
  char line[PROCMAPS_LINE_MAX_LENGTH];


  while (fgets(line, sizeof(line), stream) !=NULL){

    procmaps_struct * entry = malloc(sizeof(procmaps_struct));

    // TODO: exclude endofline
    int ret = sscanf(line, "%p-%p %s %ld %s %d %s", &(entry->addr_start), &(entry->addr_end), entry->perm, &(entry->offset), entry->dev, &(entry->inode), entry->pathname);
    if (ret<7){
      perror("failed scanf");
      return NULL;
    }
  }

}

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

