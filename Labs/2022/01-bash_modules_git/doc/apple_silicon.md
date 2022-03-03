- [Getting `Ubuntu (x86-64)` with `mk` modules on Apple Silicon](#getting--ubuntu--x86-64---with--mk--modules-on-apple-silicon)
  * [How it works](#how-it-works)
- [Quickstart](#quickstart)
  1. [Download Docker Desktop](#1-download-docker-desktop)
  2. [Downloading the image](#2-downloading-the-image)
  3. [Mounting a folder](#3-mounting-a-folder)
  4. [Creating a container from the image](#4-creating-a-container-from-the-image)
  5. [Managing containers](#5-managing-containers)
- [Day to day](#day-to-day)

Credits: [Bendik Skundberg Waade](mailto:bendikskundberg.waade@mail.polimi.it).

# Getting *Ubuntu (x86-64)* with *mk* modules on Apple Silicon
The smoothest way of accessing a `Ubuntu (x86-64)` virtual machine (VM) is to use `Docker`.

A `Docker` image built upon [`Ubuntu`](https://hub.docker.com/_/ubuntu) (`x86-64` architecture) with [`mk`](https://github.com/elauksap/mk) installed is available [here](https://hub.docker.com/r/pasclafr/mk) and ready to use to create your VM.

First a note on the intuition of how `Docker` works for those who are not familiar. Then a quickstart guide, with a detailed description of what everything means. Finally, a day-to-day reference guide for when you have everything set up and just want a reminder.

## How it works
The two most important terms when it comes to `Docker` are **image** and **container**. Images are blueprints of VMs, and define the environment, a bit like classes in `C++`. Containers are instances of images, a bit like objects/instances in `C++`. `Docker` views a container as a sequence of changes made to its environment (the image), but the image itself never changes. So what we need to do to use `Ubuntu` is to download Prof. Africa's image and start a container on top of it. This container will then serve as our VM, and we can launch our shell in it, mount local folders to it, etc. We usually call the machine in which our container runs the **host** and the operating system running *inside* the container the **guest** (or *container* itself).


# Quickstart

## 1. Download Docker Desktop
The first step is to download [`Docker Desktop for Mac`](https://www.docker.com/products/docker-desktop). Choose the **Apple Chip** option. This should be a simple drag-and-drop thing, and after it is done copying to `Applications` you can launch it by typing `Docker Desktop` into `Spotlight` (`⌘Cmd + Space`). `Docker` is relatively heavy on RAM, and it might help to go to `Settings` and enabling the *Virtualization framework* under *Experimental features*. In any case, you can open a terminal and type `docker version` to check if it works. As long as `Docker Desktop` runs, this command should return info on both the *Client* and the *Server*. If `Docker Desktop` is not running, the command will inform you.

## 2. Downloading the image
In `Docker` this process is called *pulling*. If everything works up until now, you can download the [`mk` image](https://hub.docker.com/r/pasclafr/mk) by typing

```bash
docker pull pasclafr/mk
```

The `pull` command connects to `Docker Hub` and tries to download the specified image. This might take some time, but after it is finished it should be found by typing `docker images`. It should also show up under the *Images* section in `Docker Desktop`. This is the blueprint that we can build containers on top of and that specifies the environment of the VM.

## 3. Mounting a folder
This step is **optional**, but **recommended**. Mounting a folder means sharing a folder between the host and the container. When we run a container, we will only be able to interface with it through the shell. This means that we will only have access to command line editors such as `emacs` or `vim`. If you are comfortable with these tools, you may skip this step. If you'd rather use graphical tools like `VSCode` or `CLion`, you can use the host system to write code using these programs and store the files in the mounted folder; this allows the container to automatically have access to those files. Then you can move to a shell in the container and compile the files from the command line there.

In the following we will assume that the folder `~/pacs` exists on your host system and is to be shared with `Docker`:
```bash
mkdir -p ~/pacs
```

## 4. Running a container from the image
The following command, which in `Docker` is called *running*, creates a container with the name `my_ubuntu` from the image `pasclafr/mk` that we just pulled, and mounts the host folder `~/pacs` to the location `/root/pacs_mnt/` inside the container, and then runs an interactive `Bash` shell in the container:

```bash
docker run -it --name my_ubuntu -v ~/pacs:/root/pacs_mnt pasclafr/mk /bin/bash
```

The `-it` flag runs the shell interactively, and `-v` specifies the mount. Note that the command above **creates** the container, and should only be run once (per container).

Once the `Bash` session has been launched, a full [`mk`](https://github.com/elauksap/mk) installation is available and pre-loaded (see the `.bashrc` file inside the container). From now on, for instance, you can write code on the host system and store it in the folder `~/pacs/`, whose content is visible to the container from the mounted directory `~/pacs_mnt/`. Anything you add to this folder will be visible from both the host and the container, under the respective mountpoints. Then, you can compile code and run command-line tools and executables from the container shell, *e.g.* with
```bash
g++ main.cpp -o main
./main
```

Once you are done, type
```bash
exit
```
to return to the host shell.

## 5. Managing containers
Containers are not destroyed upon exiting. You can inspect all the existing (both running and stopped) containers with

```bash
docker ps -a
```

To log back into a shell of an existing container, for example `my_ubuntu`, use

```bash
docker exec -it my_ubuntu /bin/bash
```

That is, do **not** use the `run` command when the container already exists. Use `exec`, instead.

It is good not to keep `Docker Desktop` running all the time, as it consumes quite a lot of resources. It is perfectly safe to exit `Docker Desktop`, just remember to `exit` to your host shell and `stop` the container afterwards:

```bash
exit # Return to the host shell.
docker stop my_ubuntu
```

You can check the status of a container with the `docker ps -a` command. Once it has been succesfully stopped, you can safely quit from `Docker Desktop`.

When you want to go back to your container, launch `Docker Desktop`, check your containers with `docker ps -a` and, if the container (for example `my_ubuntu`) is not already running, start it with `docker start my_ubuntu`. Finally, log into it with `docker exec -it my_ubuntu /bin/bash`. You can repeat the whole procedure as many times as you want!


# Day to day
This section provides a quick reminder of the workflow if you have already gone through all the steps above.

Write code in your favorite editor and store the files in a shared folder (for example `~/pacs`).

Launch `Docker Desktop` and open a terminal. Check your container status with

```bash
docker ps -a
```

If the container (for example `my_ubuntu`) is not already running, start it with

```bash
docker start ubuntu
```

Log into a shell of the container just started with

```bash
docker exec -it my_ubuntu /bin/bash
```

and locate the shared folder (mounted under `~/pacs_mnt/` in the guide).

When you are done working, exit and stop the container with

```bash
exit
docker stop my_ubuntu
```

Now you can exit `Docker Desktop`.
