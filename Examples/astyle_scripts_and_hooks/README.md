# astyle_precommit_hook
AStyle Git precommit hook for code beautification enforcement

Features:

* deletes trailing whitespaces by default
* Creates a backup in a folder with date/time
* Two styles defined: Linux kernel, and User space.

Prerequisites:
* Astyle (Artistic style) installed and added to PATH.

Installation:
* copy the file to .git/hooks in desired repository.
* Add execution mode (chmod +x)
* Optional: Installation for every created repository.
`sudo cp ./pre-commit /usr/share/git-core/templates/hooks/`

Usefull notes:
* To bypass the hook, just add -n to the git commit command ( git commit -n)


