# Alpha wrap

A small CLI wrappr around Alpha Wrap in CGAL, that can be easily called from the command line
on a variety of meshes.

# Compiling

It will be necessary to install dependencies, this can be done through a package manager such as
`brew` (`brew install cgal`), or `apt` (`sudo apt-get install libcgal-dev`).

```
cmake .
make

OR

./compile.sh
```

# Usage

`./main <input mesh> <output file>`


## Credit

This is just a small wrapper, the actual algorithm implementation is
explained [here](https://www.cgal.org/2022/05/18/alpha_wrap/).
