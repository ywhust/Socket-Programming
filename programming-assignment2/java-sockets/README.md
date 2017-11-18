# Incomplete Guide to Java Sockets

In the subdirectory, we provide you several examples of Java Sockets. In particular, there are two examples: Greeting server/client, and Echo server/client. From their names we believe that you can easily tell their functionalities. If you cannot, please refer to [A Guide to Java Sockets](http://www.baeldung.com/a-guide-to-java-sockets). Note that, all of these codes were directly copied and modified from [A Guide to Java Sockets](http://www.baeldung.com/a-guide-to-java-sockets). 

One modification that you might need to pay attention, if you decide to go through [A Guide to Java Sockets](http://www.baeldung.com/a-guide-to-java-sockets). 
+ For the `EchoMultiServer`, we have changed `new EchoClientHandler(serverSocket.accept()).run();` to `new EchoClientHandler(serverSocket.accept()).start();` because it seems the one with `run` does not work correctly.

## Usage 

+ Compile: `make clean` and then `make` (not work on windows)
+ Run: `java classname`. For example, `java EchoMultiServer` (pay attention, no `.class` or `.java`)


## Notice

These codes were tested under `JDK 9` and `JDK 8`.