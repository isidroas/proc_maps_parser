#include "assert.h"
#include "pmparser.h"

int _pmparser_parse_line(char * line, procmaps_struct * entry);

int main(void){
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
