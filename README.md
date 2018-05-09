# cppmark
A C++ Parser

>2018-05-07
Finished parsing top-level tokens, except for tables and defs.

>2018-05-08
Finished basic syntax, including parsing and rendering. Tested
very simple cases.

>2018-05-09
Added gulp-server. Scanned the code once and fixed some bugs.
Fixed unicode bugs.

### Todo
- [ ] unit tests
- [ ] mathJax
- [ ] auto build, maybe GUI
- [ ] top-level tokens: tables
- [ ] line number
- [ ] escape/unescape
- [x] encoding, chinese character
- [x] inline tokens: checkbox, item, links, etc.
- [x] rendering

### Notes
1. A listitem will be parsed as a block if it's a loose item, 
and as text otherwise.
2. To end a list, you need one blank line followed by a block
 token other than a listitem, or two blank lines.  
3. It's better not put spaces before fences

### changes relative to Moemark
1. use specific flavor
2. blockquote is terminated by blank lines
3. tasklist allows loose list item