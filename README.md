# Greetings
Welcome! Thank you for considering the README file.

**Contents**
  1. What is railnet?
  2. What is the project status?
  3. Installation
  4. Using
  5. Credits
  6. Bug reports/Contact

# 1 What is railnet?
It is a project to export **OpenTTD route networks**, i.e. maps that depict
the tracks and stations of your OpenTTD map.

The project consists of three parts:
  * The **railnet driver**: This is a pseudo-video driver in OpenTTD which
    dumps a json file containing all the stations, their connections and other
    data like carried cargo. It uses the YAPF pathfinder.
  * The **railnet filter** utility: It takes the json file, filters out order
    lists (depending on command line arguments) and then outputs the modified
    data in the same json file format.
  * The **railnet2dot** utility: Converts a railnet json file into a *.dot*
    file (an input file for tools that generate PDFs, see the "Using"
    section).

Contrary to the video driver, the utilities can take parameters for
customization.

# 2 What is the project status?
It is currently a testing-only version. The code has not been approved by the
OpenTTD developers.

# 3 Installation
## Requirements
  * git (downloading only)
  * cmake
  * A C++11 compiler
  * the dot utility (called "dot" on linux)

## Installing

First, we compile OpenTTD with video driver support.

```sh
git clone https://github.com/JohannesLorenz/OpenTTD -b railnet openttd-railnet
cd openttd-railnet
# now, follow the usual
# installation instructions for OpenTTD.
# NOTE: you must compile OpenTTD with the C++11 or higher,
#       otherwise, the railnet video driver will not be compiled
mkdir build
cd build
cmake ..
make
# then, do an installation or make a bundle...
make install
```

Next, get the railnet utilities
```sh
git clone https://github.com/JohannesLorenz/railnet
mkdir railnet/build
cd railnet/build
# compile. don't forget the '..' in the end
cmake \
  -DCOMPILER=clang \
  -DCMAKE_BUILD_TYPE=Debug \
  -DOPENTTD_DIR=/path/to/where/you/just/cloned/openttd/ \
  ..
make
```

# 4 Using
If you want to do it step by step:
```sh
/where/you/installed/openttd -g your_savegame.sav -v railnet \
  > map.json 2>/dev/null
cat map.json | ./railnet-filter -c PASS > map2.json 2>/dev/null
cat map2.json | ./railnet2dot > map2.dot 2>/dev/null
cat map.dot | dot -Kneato -Tpdf > map.pdf
```
Otherwise, you can also use pipes:
```sh
/where/you/installed/openttd -g your_savegame.sav -v railnet \
  | ./railnet-filter -c PASS | ./railnet2dot | dot -Kneato -Tpdf > map.pdf
```

# 5 Credits
The author thanks:
  * The many helpful people in the OpenTTD chats/forums for helpful
    their patience and their helpful suggestions

# 6 Bug reports/Contact

Feel free to give feedback. Either to the community, or directly to the
railnet author:

```sh
printf "\x6a\x6f\x68\x61\x6e\x6e\x65\x73\x40\x6c\x6f\
\x72\x65\x6e\x7a\x2d\x68\x6f\x2e\x6d\x65\x0a"
```

If you'd like to post a feature request or a bug report, you can post it on
the project's
[github page](https://github.com/JohannesLorenz/railnet/issues/new).

