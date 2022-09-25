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
Build a ModelSearchQuery object and pass it to the **search()** member function.
```c++
ModelSearchQuery query{.q=bunny,.downloadable=true};
fetcher.search(query);  //Or:
fetcher.search({.q=bunny,.downloadable=true});
```
### Search Results
There are two possible return types for search, both are wrapped in an optional if there are no results. 
#### SketchFetch Object
- The results are returned in a vector of **ModelSearchResult**. To get a **ModelSearchResult** call **.search()**, it parses the Sketchfab API return values and stores them in an object. It contains the *uri*,*uid*,*name*,*description*,the first *thumbnail* url, the number of likes and views and if its available for download.
#### Json Object
- The results are return as a **nlohmann::json**. Call **.searchJSON()** to get the JSON return value. Refer to the Sketchfab [API][2] to learn the content of the returned JSON.

## Download

There are a few ways to download data from Sketchfab using this library. You can eiher let the library handle the file access and extraction of the downloaded zip file, or get a vector containing the data. The vector has the type if **std::vector<uint8_t>**. The download and extract function begin with *download...* the other ones begin with *get...*.
You can either pass a valid *url* from Sketchfab or a **ModelSearchResult** to the download functions.

### Models
To download a model use the **downloadModel(ModelSearchResult const&)** function.
```c++
if(auto result =fetcher.search({.q=bunny,.downloadable=true}))
    fetcher.downloadModel((*result)[0]);
```
This downloads the first result for the search query "bunny" in your model folder. 
If you want to handle the file access yourself or want to use the data directly use **.getModel(ModelSearchResult const&)**
```c++
if(auto result =fetcher.search({.q=bunny,.downloadable=true}))
    auto data_vector fetcher.getModel((*result)[0]);
```
But be aware that the returned data from Sketchfab is zipped. You have to unzip the data_vector to use it.

### Thumbnails

To download a thumbnail use either **.downloadThumbnail(ModelSearchResult const&)** or **.downloadThumbnail(str::string_view url,std::string_view name)**
```c++
if(auto result =fetcher.search({.q=bunny,.downloadable=true}))
    fetcher.downloadThumbnail((*result)[0]);    //or
    fetcher.downloadThumbnail((*result)[0].thumbnail,"thumby");
```
This downloads the thumbnail of the first result for the search query "bunny" in your thumbnail folder folder. 
If you want to handle the file access yourself or want to use the data directly use **.getThumbnail(ModelSearchResult const&)** or **.getThumbnail(str::string_view url,std::string_view name)**
```c++
if(auto result =fetcher.search({.q=bunny,.downloadable=true}))
    auto data_vector fetcher.getThumbnail((*result)[0]); //or
    auto data_vector fetcher.getThumbnail((*result)[0].thumbnail,"thumby");
```


[1]: https://sketchfab.com/signup
[2]: include/SketchFetch/detail/ModelSearch.hpp
[3]: https://docs.sketchfab.com/data-api/v3/index.html#!/search/get_v3_search_type_models