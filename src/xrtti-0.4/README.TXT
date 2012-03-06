Xrtti - Extended Runtime Type Information for C++
-------------------------------------------------

Xrtti is a tool and accompanying C++ library which extends the
standard runtime type system of C++ to provide a much richer set of
reflection information about classes and methods to manipulate these
classes and their members.


INSTALLING
----------

Prerequisites
-------------

The Xrtti compilation process requires the following software packages:

- expat
- doxygen (if you wish to build the API documentation)
- gccxml (if you wish to build the test programs or examples)


Compiling
---------

Once you have all prerequisites installed, you can compile Xrtti by
entering the xrtti top level source directory (where you will find the
Makefile file) and typing:

make exported

This will build the Xrtti system into:

build

If you would like to build the output into a different location, you can
set the OUTPUT environment variable and Xrtti will be built into that
directory instead.

The directory structure under the output directory typically looks like:

bin/       -- binaries are built into this directory
obj/       -- object files are built into this directory
include/   -- exported header files are copied into this directory
lib/       -- libraries are built into this directory
share/doc  -- documentation is built into this directory
src/       -- generated source code is built into this directory
tmp/       -- temporary files are built into this directory


Installing
----------

To install the xrttigen program, the Xrtti libraries, and the Xrtti header
files, type:

make install

This will install to the default root path of /usr.  If you would like to
install into a different directory, you can set the DESTDIR environment
variable before running make install.



USING
-----

This section gives a brief tutorial on using Xrtti.

Once your classes have had Xrtti classes generated for them, you can
use the API documented below to, at runtime, introspect on your class
definitions. Just about every aspect of your classes will be available
to you via the Xrtti API, including methods for creating instances of
your class, calling methods on such instances, accessing and changing
fields (via the get method), and destroying instances created in this way.

The basic steps for using Xrtti with your classes are:

   1. Run the xrttigen program on the header file(s) defining your classes.

   2. Compile the resulting C++ source file, and link this source file
      into your program.

   3. Use the Xrtti API to reflect on your classes.

Run xrttigen

The Xrtti system is currently based on the gccxml tool, which is what
actually reads in and parses your C++ header files. Because of this,
many of the options and operations involved in this step are strongly
related to running the g++ compiler on your code.

If you already use the g++ compiler, then you will find this step
easier, since running xrttigen is very similar to running the g++
compiler. If not, you will have to construct flags to xrttigen that are
unlike those you are passing to your compiler.

Also, if your code compiles under g++, there is a very good chance it
will work without any issues with the xrttigen program.

xrttigen needs to know the following information:

    * Where to find header files included by the input header
      file. These are specified using a -I <directory> (or multiple
      -I <directory1> -I <directory2> etc.) arguments.

    * Which classes from the header file you are using as input to
      xrttigen should have Xrtti classes generated for them. To
      save space in the generated code, you will normally want to limit
      the xrttigen tool to processing only the classes that you define
      and that you care about.  xrttigen will also automatically include
      classes that your classes reference too, so all you really need to
      specify are the classes that xrttigen should start with, and it
      will figure out the entire set of classes necessary to produce
      complete reflection information for those classes.

      You specify which classes to include and exclude using the -i
      and -e options to xrttigen. Each subsequent -i and -e argument
      further refines the set of classes which will be processed.

      Normally, you'd start with "-e '*'" to tell xrttigen to not include
      any classes except the ones that you name in later -i options. This
      is important because the default behavior of xrttigen is to include
      all classes, and without "-e '*'" your subsequent -i options would be
      meaningless.

      Next, include an -i option for each class you want to process. You
      can use simple wildcarding: "*foo" will match all classes with names
      ending in "foo" and "foo*" will match all classes with names
      beginning with "foo". So if all of your classes are in the same
      namespace, for example, "MyNamespace", then you can simply use 
      "-i 'MyNamespace::*'; or if all of your classes begin with the same
      prefix, for example "MyLib_", you can use "-i 'MyLib_*'.

    * Where to write the generated C++ code. For example,
      '-o path/to/generated.cpp'.

    * What header file(s) to read in to get your class file definitions.
      These are the last arguments to xrttigen.

Additionally, you may optionally tell xrttigen:

    * The paths to the header files included by the input header file,
      as they would appear in code that you would write. For example,
      if you're processing a file
      '/home/me/project/inc/Utilities/MyClasses.h' and you normally
      include it in your code via '#include "Utilities/MyClasses.h"',
      then you would pass this to xrttigen:

      -h "Utilities/MyClasses.h"

      Each header file named via "-h xxx" will result in a "#include
      xxx" line being emitted by xrttigen into the generated source file.
      Thus, you can use this option to cause the generated output to 
      reference header files as source code in your project would normally
      reference them (imagine if instead xrttigen just used the path to
      the header file itself - you'd end up with
      '#include "/home/me/project/inc/Utilities/MyClasses.h' - usually you
      don't want explicit paths in your code, and -h allows you to specify
      the header files that the generated code depends upon using relative
      paths).

    * Whether or not the xrttigen should generate associations with C++
      rtti information for your classes.   Associations with C++ rtti are
      enabled by default.  Normally you would want to leave this enabled,
      but if you are compiling your C++ code without C++ rtti, then then
      you will want to use the -n option to disable associations with
      C++ rtti information.

    * An intermediate file is used to save the output of gccxml; it is
      normally named gccxml.out, but you can change the name of this file
      via the -t parameter

xrttigen Example

Here is an example use of xrttigen, and some comments about what each
argument does:

             // Add /home/me/project/inc to the header file search path
xrttigen     -I /home/me/project/inc
             // Add /home/me/project/inc/posix to the header file search path
             -I /home/me/project/inc/posix
             // Define DEBUG when processing header files
             -D DEBUG
             // Put '#include "inc/Util.h"' into the generated source
             -h inc/Util.h
             // Put '"#include "inc/MyClasses.h"' into the generated source
             -h inc/MyClasses.h
             // Don't generate associations between C++ rtti info and Xrtti
             -n
             // Exclude all classes from processing by default
             -e '*'
             // But include the class named MessageManager
             -i MessageManager
             // And also include every class in the MyNamespace namespace
             -i "MyNamespace::*"
             // Except for MyNamespace::Garbage
             -e MyNamespace::Garbage
             // Write the generated source here
             -o /home/me/project/MyNamespace_generated.cpp
             // And process this file to find the class definitions
             /home/me/project/inc/MyClasses.h

Compile the generated code

Treat the generated source code just like any other source file in
your project.  Compile it and link it into your program.

Use the Xrtti API

You must include the Xrtti.h header file in your code to use the Xrtti API.
API.

Now your program can call any of the Xrtti API methods to introspect
on the classes you processed. For example, you could:

    * Iterate over all classes you processed using
      Xrtti::GetContextCount() and Xrtti::GetContext().

    * Create an instance of one of your classes using the Create()
      method of the corresponding Xrtti::Class.

    * Iterate over all of the methods of that class using the
      GetMethodCount() and GetMethod() methods of the Xrtti::Class.

    * Call one of these methods using its Invoke() method.


ACCESS TO PROTECTED AND PRIVATE CLASSES AND MEMBERS
---------------------------------------------------

The xrttigen tool can only generate "complete" access to public classes
and members.  This means that for protected and private classes and class
members, the following will be true:

- Field::HasOffsetof() will return false
- Field::IsAccessible() will return false
- Structure::IsCreatable() will return false
- Structure::IsDeletable() will return false
- Destructor::/Constructor::/Method::IsInvokeable() will return false

In essence, you will be unable to call any of the Xrtti methods which
require compile-time support, since code to support this cannot be
generated by the xrttigen tool because the classes/members in question
are protected or private.

You can enable Xrtti access to protected and private members of classes
by declaring the following:

class XrttiAccess; // somewhere at the top of your header file, to identify
                   // XrttiAccess as a globally-scoped class

friend class ::XrttiAccess; // inside the class that you want to give the Xrtti
                            // system access to

For example:

class XrttiAccess;

class Foo
{
public:

    void DoSomething();

private:

    friend class ::XrttiAccess;

    Foo();
    ~Foo();

    void DoSomethingElse();

    int a;
};


With the above friend declaration, Foo is now completely accessible via
the Xrtti system and will be constructable via Structure::Create(), and
all of its fields and methods accessible as well.


EXAMPLES
--------

See the example source code in doc/examples for examples of using Xrtti.


LICENSE
-------

Xrtti is copyrighted by its author, Bryan Ischo, and released under
the GNU General Public License Version 2. This means that you may not
redistribute this software in source or binary form unless you adhere
to the terms of the GPLv2. The full text of the GPLv2 can be found
in the file LICENSE.TXT.
