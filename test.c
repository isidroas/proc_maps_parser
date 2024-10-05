#include "assert.h"
#include "pmparser.h"

int _pmparser_parse_line(char *line, procmaps_struct *entry);
procmaps_iterator *_pmparser_parse_stream(FILE *stream);

void test_parse_line(void) {
  // from man pages example:
  char line[] =
      "00400000-00452000 r-xp 00000000 08:02 173521      /usr/bin/dbus-daemon";
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

void test_next(void) {
  // not sure if include in pr.
  procmaps_struct entry1, entry2, entry3;
  entry1.addr_start = (void *)1;
  entry2.addr_start = (void *)2;
  entry3.addr_start = (void *)3;
  entry1.next = &entry2;
  entry2.next = &entry3;
  entry3.next = NULL;
  /* procmaps_iterator it = {.head=&entry1, .current=NULL}; */
  procmaps_iterator it = {.head = &entry1, .current = &entry1};
  assert(pmparser_next(&it) == &entry1);
  assert(pmparser_next(&it) == &entry2);
  assert(pmparser_next(&it) == &entry3);
  assert(pmparser_next(&it) == NULL);
}

void test_parse_stream(void) {

  char contents[] =
      "00400000-00452000 r-xp 00000000 08:02 173521      /usr/bin/dbus-daemon\n"
      "00651000-00652000 r--p 00051000 08:02 173521      "
      "/usr/bin/dbus-daemon\n";
  FILE *stream = fmemopen(contents, sizeof(contents), "r");
  procmaps_iterator *it = _pmparser_parse_stream(stream);
  procmaps_struct *entry = NULL;
  entry = pmparser_next(it);
  assert(entry->addr_start == (void *)0x00400000);
  entry = pmparser_next(it);
  assert(entry->addr_start == (void *)0x00651000);
  entry = pmparser_next(it);
  assert(entry == NULL);
  pmparser_free(it);
}

// TODO: Innecesario, ya lo hace el example que lo puede ejecutar la ci
void test_process(void) {
  // TODO: read maps of this process and supply check that is not null.
  procmaps_iterator *it = pmparser_parse(-1);
  assert(it!=NULL);
  int size;
  procmaps_struct *map=NULL;
  map=pmparser_next(it);
  for (size=0;map!=NULL;map=pmparser_next(it)){
    pmparser_print(map,1);
    size++;
  }
  assert(size>1);
}

int main(void) {
  test_parse_line();
  test_next();
  test_parse_stream();
  test_process();
}
