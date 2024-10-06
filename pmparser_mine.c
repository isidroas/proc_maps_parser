#include "pmparser.h"

// for easier testing
int _pmparser_parse_line(char *line, procmaps_struct *entry) {

  // endofline is exclude autopatically since %s stops in whitespace
  // TODO: obtain max string by sizeof(entry->..)
  int ret = sscanf(line, "%p-%p %4s %ld %11s %d %599s", &(entry->addr_start),
                   &(entry->addr_end), entry->perm, &(entry->offset),
                   entry->dev, &(entry->inode), entry->pathname);
  if (ret < -1) {
    perror("failed scanf");
    return -1;
  }
  else if(ret == 6){
    // missing last argument (pathname) is optional
    entry->pathname[0]='\0';
  }
  else if(ret !=7){
    // missing arguments
    return -2;
  }
  return 0;
}

#include <assert.h>
// for easier testing
procmaps_iterator *_pmparser_parse_stream(FILE *stream) {

  char line[PROCMAPS_LINE_MAX_LENGTH];

  procmaps_iterator *it = malloc(sizeof(procmaps_iterator));
  memset(it, 0, sizeof(*it));
  procmaps_struct *entry = NULL, *prev = NULL;

  while (fgets(line, sizeof(line), stream) != NULL) {

    // TODO: handle errors

    entry = malloc(sizeof(procmaps_struct));
    assert(entry != NULL);
    if (_pmparser_parse_line(line, entry)) {
      abort();
      // TODO: free all struct and return null
    }
    if (prev != NULL) // unless first iteration
      prev->next = entry;
    if (it->head == NULL) {
      // first iteration
      it->head = entry;
      it->current = entry;
    }
    prev = entry;
  }
  entry->next = NULL;
  return it;
}

procmaps_iterator *pmparser_parse(int pid) {

  char maps_path[50];
  // TODO: if pid< 0 -> %s self
  if (pid < 0)
    pid = getpid();
  snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);
  FILE *stream = fopen(maps_path, "r");
  if (stream == NULL) {
    return NULL;
  }

  return _pmparser_parse_stream(stream);
}

procmaps_struct *pmparser_next(procmaps_iterator *p_procmaps_it) {
  // consumo aun más bajo de memoria seria guardar el stream y hacer parse_line
  // aquí. Esa es la ventaja de los iterators con respecto a los arrays. De lo
  // contrario (por ejemplo si se quiere acceder a todos los elementos a la
  // vez), sería mejor un array y usar realloc.
  // TODO: mirar implementación de getifaddr o alguna otra API que hace uso de
  // linked lists.
  procmaps_struct *res;
  res = p_procmaps_it->current;
  if (res != NULL)
    p_procmaps_it->current = res->next;
  return res;
}

void pmparser_free(procmaps_iterator *p_procmaps_it) {

  procmaps_struct *next;
  procmaps_struct *current = p_procmaps_it->head;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  };

  free(p_procmaps_it);
}

void pmparser_print(procmaps_struct *map, int order){
  printf("start: %p\n", map->addr_start);
}
