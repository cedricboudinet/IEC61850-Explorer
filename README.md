# IEC61850-Explorer
------
IEC61850-Explorer is a basic explorer for exploring, reading and writing values in IED servers.

The application is cross-platform (Linux and Windows) and licensed under GPLv3.

Prebuilt package for Windows are available in the [Releases](../../releases) section.

### Dependencies

- cmake
- C++ compiler (gcc for Linux, MinGW or MSVC for Windows)
- libiec61850 (get it from http://libiec61850.com/)
- Qt5

### Building

#### Compiling from sources
```sh
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

In order to build in Release mode for Windows with MSVC, the building command is:
```sh
cmake --build . --config Release
```

#### Package generation

Under Windows, users might install NSIS (http://nsis.sourceforge.net/Main\_Page).
An installation package can be generated with :
```sh
$ cpack -G <generator>
```
Under Linux generator can be DEB, RPM, TGZ ... depending on the wanted package format.
Under Windows generator can be NSIS or ZIP.

### Testing

A simple test server (iec61850\_testsrv) is supplied for testing purposes.

Under linux, to allow the debug server to listen on port 102, users may install authbind and run:
```sh
$ sudo touch /etc/authbind/byport/102
$ sudo chown root:dialout /etc/authbind/byport/102
$ sudo chmod 711 /etc/authbind/byport/102
```

then you can run the server by launching :
```sh
$ authbind ./src/iec61850_testsrv
```

### Roadmap

- Improve shell version
- Improve look and feel

### Work in progress

- Allow all objects types modification

