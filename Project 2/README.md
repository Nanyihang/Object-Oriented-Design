- Shall use Visual Studio 2017 and its C++ Windows Console Projects, as provided in the ECS computer labs.
- Shall use the standard streams libraries for all I/O and operators new and delete for memory management.
- (2) Shall provide an Executive package that accepts, from its command line, a path containing files to be converted.
- (2) Shall also provide Loader, Converter, Dependencies, and Display packages.
- (3) The Loader package shall support finding and loading all the files for conversion, starting at a path specified by the Executive.
- (3) The Converter package shall support wrapping code in an html document, and translating the "<" and ">" characters into their corresponding escape sequences.
- (5) The Dependencies package shall support insertion, in each analyzed file, links to all the files in the directory tree, on which it depends.
- (6) The Display package5 shall support wrapping selected parts of the web page in blocks that can be displayed or hidden6. The parts to be selected are: Comment blocks at the begining of each source code file, if they exist Function bodies, including the opening and closing braces Class bodies, including the opening and closing braces
- The display package shall provide the means to specify each of these parts independently. That means that the user can display none of the parts, any one or more of the parts, or all of the parts.
- (4) Shall include an automated test that demonstrates you meet all the functional requirements above. Note that this is just a set of tests that show clearly the results of each step of processing you've used to implement this project.
