The PHP Interpreter
===================

This is the github mirror of the official PHP repository located at
https://git.php.net.

[![Build Status](https://secure.travis-ci.org/php/php-src.svg?branch=master)](http://travis-ci.org/php/php-src)
[![Build status](https://ci.appveyor.com/api/projects/status/meyur6fviaxgdwdy?svg=true)](https://ci.appveyor.com/project/php/php-src)

Pull Requests
=============
PHP accepts pull requests via github. Discussions are done on github, but
depending on the topic can also be relayed to the official PHP developer
mailing list internals@lists.php.net.

New features require an RFC and must be accepted by the developers.
See https://wiki.php.net/rfc and https://wiki.php.net/rfc/voting for more
information on the process.

Bug fixes **do not** require an RFC, but require a bugtracker ticket. Always
open a ticket at https://bugs.php.net and reference the bug id using #NNNNNN.

    Fix #55371: get_magic_quotes_gpc() throws deprecation warning

    After removing magic quotes, the get_magic_quotes_gpc function caused
    a deprecate warning. get_magic_quotes_gpc can be used to detected
    the magic_quotes behavior and therefore should not raise a warning at any
    time. The patch removes this warning

We do not merge pull requests directly on github. All PRs will be
pulled and pushed through https://git.php.net.


Guidelines for contributors
===========================
- [CODING_STANDARDS](/CODING_STANDARDS)
- [README.GIT-RULES](/README.GIT-RULES)
- [README.MAILINGLIST_RULES](/README.MAILINGLIST_RULES)
- [README.RELEASE_PROCESS](/README.RELEASE_PROCESS)



root@star-A:/www/server/php/74/bin# ./phpdbg  --help

phpdbg is a lightweight, powerful and easy to use debugging platform for
PHP5.4+
It supports the following commands:

Information
  list      list PHP source
  info      displays information on the debug session
  print     show opcodes
  frame     select a stack frame and print a stack frame summary
  generator show active generators or select a generator frame
  back      shows the current backtrace
  help      provide help on a topic

Starting and Stopping Execution
  exec      set execution context
  stdin     set executing script from stdin
  run       attempt execution
  step      continue execution until other line is reached
  continue  continue execution
  until     continue execution up to the given location
  next      continue execution up to the given location and halt on the first
line after it
  finish    continue up to end of the current execution frame
---Type <return> to continue or q <return> to quit---
  leave     continue up to end of the current execution frame and halt after
the calling instruction
  break     set a breakpoint at the specified target
  watch     set a watchpoint on $variable
  clear     clear one or all breakpoints
  clean     clean the execution environment

Miscellaneous
  set       set the phpdbg configuration
  source    execute a phpdbginit script
  register  register a phpdbginit function as a command alias
  sh        shell a command
  ev        evaluate some code
  quit      exit phpdbg

Type help <command> or (help alias) to get detailed help on any of the above
commands, for example help list or h l.  Note that help will also match
partial commands if unique (and list out options if not unique), so help exp
will give help on the export command, but help ex will list the summary for
exec and export.

Type help aliases to show a full alias list, including any registered phpdginit
functions
Type help syntax for a general introduction to the command syntax.
---Type <return> to continue or q <return> to quit---
Type help options for a list of phpdbg command line options.
Type help phpdbginit to show how to customise the debugger environment.


安装了vld扩展后需要关闭swoole扩展 否则冲突
           
