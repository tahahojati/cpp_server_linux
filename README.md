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
  
  Wrap the http-parser class. 
  
TODO:
  How to compile the lib with c and project with cpp
  