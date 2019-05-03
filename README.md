# Object-Oriented-Design
CSE687 Assignment 
https://ecs.syr.edu/faculty/fawcett/handouts/webpages/cse687.htm
Covers design and implementation of object oriented programs using the C++ language. This is an intensive course covering: design 
strategies used in object oriented design, essentially all of the Standard C++ Programming Lanaguage, and its Standard Library. A typical
class will introduce some part of the C++ language, present a design principle that relates in some way to that part, and illustrate both
with code examples.

Four design and implementation projects are required that are progressively more challenging. A lot of work outside of class is expected.
Students that complete this course with good grades will have the skills to work in almost any commercial or industrial organization in 
this country.

Requirements:
Your Remote Code Publisher Solution:

1.Shall use Visual Studio 2017 and its C++ Windows Console Projects, as provided in the ECS computer labs.

2.Shall use Windows Presentation Foundation (WPF) for the Client's user display.

3.Shall assemble working parts from Projects #1, #2, and #3 into a Client-Server configuration. Unlike conventional Client-Server
  architectures, the Client need not wait for a reply from the Publisher server for a request before sending additional requests.
  
4.Shall provide a Graphical User Interface (GUI) for the client that supports navigating remote directories to find a project for
  conversion, and supports displaying the conversion results in a way that meets Project #3 requirements.
  
5.Shall provide message designs appropriate for this application. All messages are instances of the same Message class, but have a
  specified set of attributes and body contents suited for the intended task.
  
6.Shall support converting source code in the server and, with a separate request, transferring one or more converted files back to the
  local client, using the communication channel.
  
7.Shall demonstrate correct operations for two or more concurrent clients.

8.Shall include an automated test that accepts the server url and remote path3 to your project directory on its command line, invokes
  the Code Publisher, through its interface, to convert all the project files matching a pattern that accepts *.h and *.cpp files, and       then opens the Client GUI's Display view.
  
