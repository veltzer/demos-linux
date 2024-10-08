# Exercise 6

* Write a Connector and Acceptor programs that will communicate by send/receive data via
    socket.
* Make it modular enough that you will be able to specify the port externally.
* Allow the connector to receive a machine name as an external input, thus you may send data
    from another machine if needed. (For testing you may use your hostname or localhost without
    using another machine)
* Use timeout in the Connector and issue a message when connection is not made soon enough.
    Yet, do not exit when timeout occurs.
* Print the received data by the Acceptor. (Printout of the original data by the connector
    may help in debugging).
* Finally, use the input data in order to terminate the connection. The connector will close
    the opened port when no data is available. Then, the Acceptor will close the opened port
    and will terminate gracefully.
    Note: If you close the Connector too early then the Acceptor may fail in reading data.
    Use sleep function with reasonable value in order to prevent such problem.
