# Success Story

This document outlines the intentions of the application and gives context to the work that has been done. The text can be easily misunderstood as bragging I suppose, but I can assure you that it is far from that. It's more a journal to be reevaluated at a later time when most of the details have been lost already.

## Context

### Global pandemy

Around March 2020, a highly infectuous and, at the time very dangerous, global corona pandemic broke out. To mediate the risk of infection, the government issued a limitation on how many people can access a certain building of certain size. For zuehlke austria, this limitation was calculated as 30. The challange was to actually track the number of people in the office at certain times.

### Presence Bot

A colleague of mine and I were goofing around at work about a service that connects to our access system, counts the number of employees and provides that number via an official company channel. Over night my colleague built a one-file python script that does just that:
1. Reverse engineers the GET requests for accessing the list of employees (via an authenticated user of course)
1. Parse that HTML list while excluding external users (security guard, cleaning staff)
1. Count the number of unique individuals and save that number to a file that is exposed via a simple HTTP GET endpoint

The prototype was sound enough for me to continue the work and created a service from that. For sentimental reasons (and curiousity) the service was kept in python.
* A `presence-bot service` periodically polls the `presence-api service` and tracks whether or not it should trigger a notification via the `messaging-service`

> At the time I was trying to get experiences with microservices and what better way to learn is to experiment with a low-risk and very simple project like the presence-bot.

Each of the services would communicate over REST in a pull based fashion.

> Refactoring to a cleaner pull-based / stream-based solution were planned but never gotten around to doing so unfortunately.

### Presence API

A dedicated service that deals exclusively with accessing our companies access system via configurable credentials stored as docker-secrets. It would authenticate to the service, retrieve a list of employees, count the set of employees and return that number as a single value 200 response.

### Messaging Service

The number returned from the presence-api is then packed into a message format and sent to a "notification service" that is capable of reaching out to other employees.

#### Notification Service

We tried to negotiate with our company to get a dedicated service user with enough rights to send out messages to employees. Unfortunately, company policies restricted us from that, so we had to find a way to anonymously transfer a message to our colleagues.

##### Approach 1: Microsoft Teams

We decided on using a combination of a public Microsoft Form which, on message receival, triggers a Power Automate Flow that comments on a Teams post automatically created every day at midnight.

1. The notification service was responsible for understanding that complexity, hide it from other services, build and publish a microsoft teams form request.
1. The second reason for having a dedicated notification service was that eventually we would replace this hacky logic with something a little more "sophisticated", should we find resources for that.

##### Approach 2: Power Automate

After a year we reached a certain microsoft partner level and found the possibility to use premium power automate features such as "REST endpoints". This allowed us to simplify the MS Forms madness into a single HTTP POST request.

#### Power Automate

The Power Automate flows are responsible for multiple things:
1. Post a teams message every day at midnight
1. Create an anonymous POST endpoint for receiving an employee count
1. Parse that employee count (senml packet)
1. Build a message based on certain rules (if threshold passed, mark as red)
1. Create a teams comment on the post issued at midnight

This flow would happen every 30 seconds (lower values would get ourselfes throttled by the access system).

#### Docker

These services would somehow need to be orchestrated. The first version from my colleague ran as linux systemd service but this wasn't as easy now with 3 interdependent services. For that I packaged the services into 3 docker images to be orchestrated by docker-compose. This was great as it provided
1. a way to easily restart the services if something failed
1. it added an additional layer of resiliency as it would automatically restart on error
1. docker provides means of having secrets which was perfect for credential handling
1. docker-compose allows the usage of 2 compose-files where 1 would override parts of the other, perfect for dev/prod environments



## Presence Cube

### Context

A few months before corona, I got the chance of experimenting with embedded development in the context of trying to create an automated green house (hydroponics bay). It was an inhouse project where only I got 50% of the time per week to study up on the topics needed to build one. Progress was quite slow as I haven't had any contact with C, C++, embedded development, edge computing or any form of gardening at all. So eventually I had to move on and rejoin productive society with a new project.

So, corona was raging on, we had years worth of lockdowns, fear and panic (the situation here in austria was not pretty). At the time I was quite depressed as corona basically eliminated every single hobby I loved to do and of course kept me from visiting my friends or go out in vienna (rules in vienna were extra strict, nobody was allowed to go outside with police patroling the city). My project situation was increasingly bad as I was forced to leave a project I loved and had to do one at a customer where every developer already gave up on good development practices. So eventually depression got to me.

Eventually I figured to make the best of it and use the knowledge gathered the year before to create something useful with it with goals in mind:
1. Use the tools researched for the indoor garden and turn it into something useful
1. Gain on-hand experience with C, C++ and embedded developing
1. Gain experience and solid understanding of electronic engineering
    * This also was to evaluate whether or not a university course in mechatronics was an interesting topic or not

The plan was to create a cube that would sit right at the entrance (glass door) and would shine red, green and orange depending on the amount of employees currently present at the office. The lid of the cube would contain a small display that would display the companies runlevel and the employee count.

The jouney to that cube was quite intense as it would cover a broad range of trainings, disciplines, trial and error and most of all mistakes.

TDB TDB TDB TDB
TDB TDB TDB TDB
TDB TDB TDB TDB
TDB TDB TDB TDB
TDB TDB TDB TDB

* This section could be expanded by
- trainings in electrodynamics and mechatronics (mechrev course)
- working with mainflux
- learning to understand C
- finding out how to debug code on embedded systems
- finding out that platform IO makes things a lot easier 
- getting into software design
- how to test code on embedded systems
- building electronic circuits
- mechatronic lifecycle
- building a frame for the cube
- refactoring the app into modular parts 
- MQTT => REST
- Flutter UI for setting the colors (Flutteranden)
- Scripted test scripts
- Attending a course to learning PCB design using KiCAD
- Using EInk displays
- Trying to use embedded UI frameworks for drawing (unsuccessfully due to heavy resource constraints)
- Trying to create my own tiny UI framework
    * The idea and design was sound and even worked yet was still unsuccessful due to heavy resource constraints and too little understanding of C++ to write code that is tiny enough)
- Learning how to measure resource usage with valgrind
- Setup a CI/CD pipeline that would test and measure the embedded code

TDB TDB TDB TDB
TDB TDB TDB TDB
TDB TDB TDB TDB
TDB TDB TDB TDB
TDB TDB TDB TDB
