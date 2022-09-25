# Programming Guide

To use `SketchFetch`  you need to initialize a `SketchFetch` Object. You can use the 
default constructor and change settings later or provide a username and password.
```c++
SketchFetch fetcher{};
SketchFetch fetcher{"username","passord"};
```
You can also choose if the folders to store models and thumbnails should be created or provided by you. As a default it will use the folder the Project is running in as a 'workingFolder'. The *modelFolder* and *thumbnailFolder* are *workingFolder/models* and *workingFolder/thumbnails* 
```c++
SketchFetch fetcher{"username","passord",SketchFlag::CreateFolder }; //or SketchFlag::DontCreateFolder
```
You can set different folder the following setters. If you want `SketchFetch` to create the folders for you pass true as the second argument. The function will throw if the folder doesn't exists.
```c++
fetcher.setWorkingFolder(std::filesystem path& folder,bool create);
fetcher.setModelFolder(std::filesystem path& folder,bool create);
fetcher.setThumbnailFolder(std::filesystem path& folder,bool create);
```



## Authentication
You don't need to authenticate with Sketchfab if you only want to use the search function. If you want to download thumbnails or models, you have to authenticate using your password and username from Sketchfab.
You can create an account [here][1] if you don't have one.

You can either log in when creating the `SketchFetch` Object or at a later time as needed.
```c++
SketchFetch fetcher{"username","passord"}; //or
fetcher.setAccess("username","password");
```
The authentication functions throw an exception on error. You can see what went wrong by looking at the exception message
```c++
try{
    fetcher.setAccess("username","wrong_password");
}catch(authentication_error const& ae){
    fmt::print("{}\n",ae.what());
}
```
Sketchfab uses a token with an expiration date. The token is stored in your *workingFolder* as *sketchfetch.auth*. You can just call authenticate without arguments if you already have a token. Or check if you are authenticated without starting authentication.
```c++
bool authenticated = fetcher.authenticate();
bool authenticated = fetcher.getAuthenticated();
```
## Search
`SketchFetch` uses an **ModelSearchQuery** object to describe the search requests. Every argument is optional but you have to declare them in the given order. Refer to [ModelSearch][2] for the available options and there order. Find more information about the search queries in the Sketchfab API [documentation][3].

### Search Results

#### SketchFetch Object

#### Json Object

## Download

### Models

### Thumbnails

[1]: https://sketchfab.com/signup
[2]: include/SketchFetch/detail/ModelSearch.hpp
[3]: https://docs.sketchfab.com/data-api/v3/index.html#!/search/get_v3_search_type_models