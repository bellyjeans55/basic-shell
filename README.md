# basic-shell

This is a simple *nix shell. It interprets most standard commands. This project illustrates the use of fork and exec to create child process, wherein the commands themselves execute.

TODO:
-IO redirection (piping in particular)
-Shell bound commands (e.g. cd)
-Check if file is located in path without relying on system error
  -Currently encountering serious difficulties with strtok(). In my check attempts, strtok breaks
  my previous tokenized arguments.
