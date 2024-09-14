#include "assert.h"
#include "pmparser.h"

int _pmparser_parse_line(char * line, procmaps_struct * entry);
procmaps_iterator* _pmparser_parse_stream(FILE * stream);

void test_parse_line(void){
  // from man pages example:
  char line[] = "00400000-00452000 r-xp 00000000 08:02 173521      /usr/bin/dbus-daemon";
  procmaps_struct entry;

  _pmparser_parse_line(line, &entry);
  assert(entry.addr_start == (void *)0x00400000);
  assert(entry.addr_end == (void *)0x00452000);
  assert(strcmp(entry.perm, "r-xp") == 0);
  assert(entry.offset == 0);
  assert(strcmp(entry.dev, "08:02") == 0);
  assert(entry.inode == 173521);
  assert(strcmp(entry.pathname, "/usr/bin/dbus-daemon") == 0);
}

void test_parse_stream(void){

  char contents[] = "00400000-00452000 r-xp 00000000 08:02 173521      /usr/bin/dbus-daemon\n"
    "00651000-00652000 r--p 00051000 08:02 173521      /usr/bin/dbus-daemon\n";
  FILE * stream = fmemopen(contents, sizeof(contents), "r");
  procmaps_iterator *it= _pmparser_parse_stream(stream);
  procmaps_struct *entry = NULL;
  entry = pmparser_next(it);
  assert(entry->addr_start == (void *)0x00400000);
  entry = pmparser_next(it);
  assert(entry->addr_start == (void *)0x00651000);
  entry = pmparser_next(it);
  assert(entry == NULL);
  pmparser_free(it);
}

void test_process(void){
  //TODO: read maps of this process and supply check that is not null.
}

int main(void){
  test_parse_line();
  test_parse_stream();
}
