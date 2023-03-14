#!/bin/bash
git pull --recurse-submodules
(cd Examples; /bin/bash setup.sh)
