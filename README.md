# cpp_server_linux


Ok we want to do process management, threading, memory.
// read and write to file. 
// several classes
// member initializer lists
// abstract classes
// composition 
// thread pooling
// templates
// promise future
// mutex
// condition_variable


// Ok the server will have to accept multiple requests. each of which has to take a bunch of time
// we have to store some data. 
// in order to have true concurrency, we have to have another worker thread.
// perhaps the database can run on a different thread. and we can have multiple databases. 

//philosophy
  we want very modular code. The code must be put in several top directories.  Now when we have https-parser where do we
  put it?  It is clearly an important class. 
  
  we have several modules though like main web server, web code, database, logger, etc.  So I think the .h files should in general
  be put next to the .cpp files. Ok I like that idea. Now how do we write the cmake? Well let's first code then we figure that out. 
  
  So
  We have multiple modules. Each running on their own threads and having their own resources etc.
  ForFramework:
    FrontController,
    Router,
    Request, 
    Response,
    ThreadPooling,
    Qing,
    Tasks, 
    No response decorators, etc.. 
    callbacks, 
    settings document,
    param_supply
    error handling. 
    error separation between threads. //i.e. server cannot go down. 
    Classes to extend in our application.
    caching of database
    html renderer
    CPU monitoring
   
    
  
TODO:
  How to compile the lib with c and project with cpp
  get the routing done
  write the main website
  
  404 handling: abstract controller must have defined all the methods such that they report method missing to the
   application. Application must then dispatch the correct 
  
Application will have a router, a list of controllers, each controller handles a single path. So the application will
 create a router with all the controllers, and numbers.  When a route matches, application will create a new
  controller and call its post/get/etc method.   