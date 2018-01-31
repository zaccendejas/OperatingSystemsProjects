CSE.30341.FA17: Project 03
==========================

This is the documentation for [Project 03] of [CSE.30341.FA17].

Members
-------

1. Zac Cendejas	(zcendeja@nd.edu)
2. Noelle Rosa	(nrosa@nd.edu)(rip)
3. Will Fritz	(wfritz@nd.edu)

Design
------

> 1. The client library needs to provide a `Client` class.
>
>   - What does the `Client` class need to keep track of?
>	~ The client class needs to keep track of a concurrent queue for outgoing messages, a concurrent queue for incoming messages, a concurrent map holding refernces to Callback objects, and three Thread objects to handle, sending, receiving, and parsing.
>
>   - How will the `Client` connect to the server?
>	~ It will connect at the start of the run() function using the port number and looking for the host given to it when its constructed.
>
>   - How will it implement concurrent publishing, retrieval, and processing of
>     messages?
>	~ A concurrent queue is used to store messages that need to be published, so that one thread can continuously pop from this queue and send the message to the server while other functions push to the queue. Retrieval is handled by a separate concurrent queue and thread. Retrieval continuously pushes server responses to its concurrent queue. Processing messages is done by another thread that pops from the queue populated by retrieval.
>
>   - How will it pass messages between different threads?
>	~ Messages will be stored in concurrent data structures that each thread has access to. This is where the use of locks  becomes important to eliminate the potential for race conditions. 
>
>   - What data members need to have access synchronized? What primitives will
>     you use to implement these?
>	~ Queues used for outgoing/incoming messages and a callback table need to be synchronized, because you don't want any sections of code to try accessing their data at the same time. This is handled by using mutexes inside the data structures member functions so that only one thread can access data at a time. You also need to use condition variables when one thread may be waiting on another thread to do something first (such as add the first element to a queue) before continuing. 
>
>   - How will threads know when to block or to quit?
>	~ Threads know when to block because the data structures they are accessing have locks built in to each member function, so if it tries to call a member function on a shared data structure while another thread is also in the midst of running a similar member function the use of locks will cause the first function to "block" until the other is finished. Quit is handled based on a event loop that runs based on a boolean that is changed to false when a disconnect response from the server is parsed.
>
>   - How will the `Client` determine which `Callbacks` belong to which topics?
>	~ When a subscribe is called the callback reference passed into it is stored in a multimap where the topic is a key and the reference is a value. This way after parsing a server response you can loop through the table to find matching callback references for the topic in the server response.


> 2. The client library needs to provide a `Callback` class.
>
>   - What does the `Callback` class need to keep track of?
>	~ The callback class is in charge of processing messages so the Callback class needs to keep track of the messages struct and needs to handle functionality for any subclasses such as EchoCallback. EchoCallback will be used to print the messages but other potential Callbacks we could implement for our user app could include a StoreCallback that would store a message in a file or something of a similar nature. 
>
>   - How will applications use this `Callback` class?
>	~ Callbacks allow you to pass a function into a function. For example you can have a functions that loops through the elements of a vector and performs a function on each element. You can can pass this a function that might manipulate the elements, print them, save them in a file, ect. The EchoCallback method that we need to implements will likely loop through the mesages to be able to print the contents to stdout. 


> 3. The client library needs to provide a `Message` struct.
>
>   - What does the `Message` struct need to keep track of?
>	~ This keeps track of the Message contents, type, topic, and the sender.
>
>   - What methods would be useful to have in processing `Messages`?
>	~ It would be useful to process Messages based on their topic, or based on their sender. It would also be useful to look at the type in order to determine what other functions to call.


> 4. The client library needs to provide a `Thread` class.
>
>   - What does the `Thread` class need to keep track of?
>	~ This keeps track of a single pthread_t and does stuff to it.
>
>   - What POSIX thread functions will need to utilize?
>	~ pthread_create(), pthread_mutext_init(), pthread_cond_init(), pthread_mutex_lock(), pthread_mutext_unlock(), pthread_cond_wait(), pthread_cond_signal(), pthread_join(), pthread_detach()



> 5. You will need to perform testing on your client library.
>
>   - How will you test your client library?
>	~ We can build a Google unit test for each function to test that it returns what we expect or changes a queue/table in the way we expect.
>
>   - What will `echo_test` tell you?
>	~ It will tell us what messages are getting correctly passed to echo's callback object.
>
>   - How will you use the Google Test framework?
>	~ We will read Google's instructions on github for how to use the Google Test framework and try to follow their examples.
>
>   - How will you incorporate testing at every stage of development?
>	~ Make a unit test for a function once it is ready to be tested.


> 6. You will need to create an user application that takes advantage of the
>    pub/sub system.
>
>   - What distributed and parallel application do you plan on building?
>	~ A project management notification system, where devs can sub to a team and manages can send notifications to their team.
>
>   - How will utilize the client library?
>	~ Devs will call sub on their team while managers will publish messages to their team.
>
>   - What topics will you need?
>	~ One for every team the user app is meant to represent. 
>
>   - What callbacks will you need?
>	~ Each dev should pass a callback that prints out the body of each message struct and the name of the sender in case there are multiple managers.
>
>   - What additional threads will you need?
>	~ None because that client handles that stuff.

Response.

Demonstration
-------------

> Place a link to your demonstration slides on [Google Drive].
https://docs.google.com/a/nd.edu/presentation/d/1w3opjZYdTH8YspsYGsBv7IkQFCyZEoWP0lRVsm89BjU/edit?usp=sharing


Errata
------

> Describe any known errors, bugs, or deviations from the requirements.
~ I can't get a single google test to compile

Extra Credit
------------

> Describe what extra credit (if any) that you implemented.

[Project 03]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project03.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://drive.google.com
