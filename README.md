# Visualization of A* Path Finding in C
This application allows you to select an image(map) and generate a path between two points in that map.

### Prerequisites
* C compiler (tested on gcc 5.4.0, Ubuntu 16.04)
* [gtk+](https://www.gtk.org)

### Screenshot
![screenshot](https://raw.githubusercontent.com/akhilmd/ds-sem3/master/img/screenshots/as2.png)

### How to build
###### Building for Ubuntu:
```sh 
$ make clean
$ make
```

###### Running:
```sh 
$ ./start
Enter Path to Map: ./img/map-kora-8.png

PLEASE SELECT A START POINT
START POINT SELECTED AT (100, 169)
PLEASE SELECT AN END POINT
END POINT SELECTED AT (168, 364)
CLICK ANYWHERE ON THE MAP TO START PLOTTING
PLOTTING ROUTE
ROUTE HAS BEEN FOUND IN GREEN
$
```
