# Blog Post for Logging in C++
author: Aman Dhruva Thamminana

## Introduction: What is Logging?
Whether you are one is just staring of in their programming journey or a seasoned programmer, 
you would have used print statements to debug your code. Logging is very similar to print states of code but with more tools
to help better understand the state of the program. Logging can be a very useful tool to level up one;'s programming skills.

In simple terms logging is the act of  recording events, actions, or states that occur during the execution of a program.

Let's look into understanding the basics of logging in C++ and looking into spdlog and glog libraries.

## Back to Basics:  Why is Logging Important?
Logging helps us understand what is happening to a program under the hood. This can be very crutial
especially at scale like when working on a large code base or when running high and long computing programs.
Understading this information allows us to create error free, efficient and robust code. 

Logging has become an indispensable standard in the software industry. In real-world many applications in  ios, android, and web and software platforms themselves use logging to record information about their operations. 
. These detailed records enable developers to understand the context of issues, replicate scenarios,
and create fixes.

### How Logging Differs from Print Statements?

Print statements can be used as a simpler version of logging allowing the programer to display several states of the program.
Unlike print statements logging provides several syntatical levels of capturing information. 

Although it is upto the library to provide these levels, some of the common levels are:
- Debug: Information that is critical to debug an application.
- Info: Informational messages that highlight the progress of the application.
- Warning: Indicates a potential problem that is not terminal to the application.
- Error: Indicates a serious error that may prevent the application from continuing to run.

Logging also provides the ability to log to different outputs like files, console, etc. This can be useful when running a program on a remote locations.

### spdlog 💨 💨


### glog (🔵🔴🟠🟢)
