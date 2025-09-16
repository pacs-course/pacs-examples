# A very simple example of the use of shared libraries #

The command `exec.sh` compiles and runs the example.

This is just an example that creates two "releases" of a shared library and sets up the technique used for versioning in Linux. 
By changing some links you may use the old or new release of the library, without recompiling the main program. That's what happens when you install a new release of a library in your PC.

We recall that the shared library is a collection of object files that are linked together in a single file. The library is loaded in memory when the program is run, and the objects are extracted from the library and linked to the program. We can distingush three type of names of a shared library:

- the **link** name (e.g. `libmylib.so`), which is used when you compile the program;
- the **soname** (e.g. `libmylib.so.1`), which is used when you run the program and contains	 the information on the version of the library;
- the **real** name (e.g. `libmylib.so.1.1.0`), which is the actual file that contains the library, and it normally contains also information on the release number.

Usually, files with link and soname are just symbolic links to the real file.

The complexity of the handling of versions and releases is of course necessary in professional code. If the library is used only locally you can avoid the complexity of using the three names, and in this case link, soname and real name coincide   (e.g. `libmylib.so`).

In `DynamicLoading` we show another characteristic of shared libraries: the fact that you can dynamically load them, and extract objects contained in the library run time.

## Note ##
If you use versioning, you need to store in the library the information on the version of the library. This is done with the `-Wl,-soname` option of the linker. The option is used as follows:
```bash
    g++ -shared -Wl,-soname,libmylib.so.1 -o libmylib.so.1.1.0 mylib.o
```

It indicates that `libmylib.so.1` is the soname of the library, while the real name of the library is `libmylib.so.1.1.0`. The soname is used by the dynamic linker to find the library when the program is run. The real name is used to store the library in the file system.

The option `-Wl,-soname` is used to pass the option `-soname` to the linker. The option `-soname` is used as follows:
```bash
    -Wl,-soname=NAME
```
where `NAME` is the soname of the library.
or
```bash
    -Wl,-soname,NAME
```
When you link your executable with a shared library, you normally use the link name, which is normally just a symbolic link to the soname, which is itself a link to the real name.
If the linked library has the soname information, the loader (also called dynamic linker) will use the soname to find the library, and not the link name. This is useful when you have to update the library, and you want to use the new version without recompiling the program.

Here is the synopsis of the option `-soname` of the linker:

```bash
-soname=name
           When creating an ELF shared object, set the internal DT_SONAME field to the specified name.  When an
           executable is linked with a shared object which has a DT_SONAME field, then when the executable is run
           the dynamic linker will attempt to load the shared object specified by the DT_SONAME field rather than
           using the file name given to the linker.
```

## Linking and loading a dynamic library ##
Linking a code with a dynamic library is done with the usual command -l:
```bash
	g++ -o myprog myprog.o -L. -lmylib
```	
where `-L.` indicates that the library is in the current directory, and `-lmylib` indicates that the library is called `libmylib.so`. The `-L` option is not needed if the library is in a standard directory for libraries. 

The loader is the tool that loads your code, and all the dynamic libraries it is linked to, at the moment of the launch. It will look for the library in the directories specified by the environment variable `LD_LIBRARY_PATH`, and in the standard directories.

When creating an executable, or a shared library, you can instruct the linker to put in the code the indication on where to look for the library, in case you plan to put it in a special place. The command is `-Wl,-rpath,/path/to/library`. For example
```bash
	g++ -o myprog myprog.o -L. -lmylib -Wl,-rpath,/home/user/lib
```
This will put in the executable the information that the library is in `/home/user/lib`. The loader will look for the library in this directory, and if it does not find it, it will look in the directories specified by `LD_LIBRARY_PATH`, and in the standard directories.

# A note #
The versioning mechanism is important only for libraries that are distributed to the general public and constantly maintained. For libraries used only for internal use you may forget about versioning and use only the link name. In this case, the link name, soname and real name coincide.

# What do I learn here? #
- Some basic concepts of shared libraries. 