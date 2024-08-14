# Necessary only if you use parallel algorithms
LDLIBS+=-L$(mkTbbLib) -ltbb
