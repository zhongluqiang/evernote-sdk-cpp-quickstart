# evernote-sdk-cpp-quickstartenviroment
Evernote C++ SDK quick start demo which features simple but comprehensive utilization for Evernote C++ SDK APIs.

## Features

1. Support full and incremental sync for Evernote/印象笔记, data objects serialization and deserialization are through local files.
2. Support notes export, notes will be exported to both raw ENML file and HTML files, and the files are organized by folders of notebooks.
3. Support an index.html for all the exported HTML files, which can be easily used to serve web applications.

## Usage

Set `g_BaseUrl` to your Evernote environment and `g_AuthToken` to your developer Token in `EvernoteClient.cpp`, and run:

```
$ make distclean && make
$ ./client
```

With developer token being properly set and libs being correctly installed, you will get all the output in `evernote-data` directory.

When the program runs for the first time, a full sync is performed, all the data objects in your account will be downloaded to local disk, it will take some time if you have a lot of notes in your account. Then leave it alone,  next time if you run the program, it will perform a incremental sync, only those updated content will be downloaded.

## Requirements

thrift-0.12.0, C++11, boost

## Major issues

1. `ulimit -s unlimited`  is necessary otherwise segmentation fault occurs, this is due to C++11 regex operation, still unknown about the reason.

2. Notebook name and note title must not include "/", it will be ambiguous when create directories for notebooks and notes with a "/" in their names.
3. Unnecessary overhead in exporting notes, currently each time when the program run, it will remove all the exported notes and recreate new files, which is of extremely low efficiency.



