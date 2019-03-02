

%% write/read one number
name = tmpnam ();
fid = fopen (name, 'wb');

msg_write = pi;
fprintf ("writing message '%g' to file '%s'\n", msg_write, name)
fsave (fid, msg_write);
fclose (fid);

fid = fopen (name, 'rb');
msg_read = fload (fid);
fclose (fid);
fprintf ("read message '%g' from file '%s'\n", msg_write, name)
delete (name)

%% write/read a small sparse matrix
name = tmpnam ();
fid = fopen (name, 'wb');

msg_write = sprandn (2, 2, .5);
fprintf ("writing the following matrix to file '%s':\n", name)
disp (full (msg_write))
fsave (fid, msg_write);
fclose (fid);

fid = fopen (name, 'rb');
msg_read = fload (fid);
fclose (fid);
fprintf ("read the following matrix to file '%s':\n", name)
disp (full (msg_read))
delete (name)

%% write/read a large sparse matrix
name = tmpnam ();
fid = fopen (name, 'wb');

msg_write = sprandn (2000, 2000, .1);
fprintf ("writing a matrix to file '%s':\n", name)
fsave (fid, msg_write);
fclose (fid);

fid = fopen (name, 'rb');
msg_read = fload (fid);
fclose (fid);
fprintf ("read a matrix from file '%s':\n", name)

delete (name)
assert (msg_write, msg_read, eps)



%% write/read a large sparse matrix
name = tmpnam ();
fid = fopen (name, 'wb');

msg_write = num2cell (randn (1, 200));
fprintf ("writing a cell array to file '%s':\n", name)
fsave (fid, msg_write);
fclose (fid);

fid = fopen (name, 'rb');
msg_read = fload (fid);
fclose (fid);
fprintf ("read a cell array from file '%s':\n", name)

delete (name)
assert (msg_write, msg_read, eps)
