# `SketchFetch` - a C++ Sketchfab Wrapper
![Sketchfab](https://static.sketchfab.com/img/press/logos/sketchfab-logo-text-white-mono.png)

`SketchFetch` is a C++ Wrapper for Sketchfab. It can: 
- Search for models using every Parameter provided by Sketchfab
- Handle authentication
- Download models and extract them

# Simple example
To start a simple search, a SketchFetch object is created. An optional result type is returned. You can iterate over the results. 
``` c++
 SketchFetch fetcher{};
 if(auto results = fetcher.search({.q = "Bunny"})){
    for(auto &res: *results){
        fmt::print("{}\n",res);     //Print the whole result object
        fmt::print("{}\n",res.name) //Print only the name
    }
 }
```
More detailed examples are in the  [PROGRAMMING](PROGRAMMING.md) document.


# Building and installing

See the [BUILDING](BUILDING.md) document.

# Licensing

SketchFetch uses the MIT License ![GitHub](https://img.shields.io/github/license/SpiRelli90/SketchFetch?style=flat-square)