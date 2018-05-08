# cppmark
A C++ Parser

>2018-05-07
Finished parsing top-level tokens, except for tables and defs.

### Todo
- [ ] top-level tokens: tables, defs
- [ ] inline tokens: checkbox, item, links, etc.
- [ ] rendering

### Notes
1. A listitem will be parsed as a block if it's a loose item, 
and as text otherwise.
2. To end a list, you need one blank line followed by a block
 token other than a listitem, or two blank lines.  