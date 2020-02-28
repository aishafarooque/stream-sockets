# stream-sockets
Using stream sockets to write a simple server/client pair.

1. Client program will connect with the server to send requests for current low and high prices for particular cars.
2. Server program will wait for connection requests from the client and reply with the respective prices for that car.

To run this file:
1. Open two Linux terminal windows
2. In window #1 (server window):
  a. "cd" into source code folder<br/>
  b. "gcc aisha_farooque_server.cpp"<br/>
  c. "./a.out"<br/>
  d. Enter "input1.txt" for "Enter the name of file:"
  e. Enter any four digit port number that is valid. 
  -- If connection is refused, then port number is not valid, try another number.
  -- Else, try starting back from step b.
 
 3. In window #2:
  a. "cd" into source code folder
  b. "gcc aisha_farooque_client.cpp"
  c. "./a.out"
  d. Enter "localhost" as the "host name"
  c. Enter same port number as Step 2-e for "server port number"
  e. Search for any model and make as the cars in the input1.txt database.

![media](demo/media.gif)
