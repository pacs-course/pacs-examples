
err = 0;
## create pipe
[pr, pw, err, msg] = pipe ();

if (err)
  error ("failed creating pipe")
endif


fflush (stdout); # prevent subprocesses from inheriting buffered output

err = 0;
## fork subprocess
[pid, msg] = fork ();
if (pid > 0)
  ## parent process. fork succeded.
  is_parent = true;
  fprintf (stderr, "process with pid %d created, the flag is %d\n", pid, is_parent);
  fflush (stderr);
elseif (pid == 0)
  ## child process.
  is_parent = false;
  fprintf (stderr, "I am the child process, the flag is %d \n", is_parent);
  fflush (stderr);
elseif (pid < 0)
  ## parent process. fork failed.
  error ("failed creating process");
endif

message_send = pi;
message_recv = 0;

if (is_parent)
  fprintf (stderr, "sending message '%g' to child\n", message_send);
  fflush (stderr);
  fsave (pw, message_send);
  fflush (pw);
endif

if (! is_parent)
  message_recv = fload (pr);
  fprintf (stderr, "received message %g from parent\n", message_recv);
  fflush (stderr);
endif


fclose (pr);
fclose (pw);
  
if (is_parent)
  fprintf (stderr, "waiting for child %d to exit\n", pid);
  [pid, status] = waitpid (pid);
  fflush (stderr);
  fprintf (stderr, "child %d exited\n", pid);
  fflush (stderr);
endif

if (! is_parent)
  __exit__ ();
  exit ();
endif
