Summary: eXtended RunTime Type Information for C++
Name: xrtti
Version: 0.4
Release: 1
License: GPL
Group: Development/Tools
URL: http://www.ischo.com/xrtti
Source0: xrtti-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root
Buildrequires: doxygen
Buildrequires: expat-devel
Buildrequires: make
Requires: expat
Requires: gccxml

%define debug_package %{nil}

%description
The xrtti package provides the shared libraries for runtime use of the
Xrtti system.  Xrtti is a tool for generating eXtended RunTime Type
Information for C++ programs.  This is a means for adding metaprogramming
capabilities to C++ programs with little developer effort.  To incorporate
extended runtime type information into your C++ program, run the xrttigen
program on your C++ header file(s) and compile the resulting C++ source code
file into your program.  Then your program can use the Xrtti API to
introspect on the classes it defines.

%package devel
Summary: Headers and documentation for the Xrtti tool
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Header files, API documentation, static libraries, and the Xrtti tool.
This RPM is needed only if you are using Xrtti as part of your C++
development process. Programs built using the xrtti-devel package do
not need to have the xrtti-devel package installed to run; they just
need the xrtti package which includes the xrtti runtime library.

%prep
%setup -q

%build

%install
OUTPUT=$RPM_BUILD_ROOT/build DESTDIR=$RPM_BUILD_ROOT/usr make install
rm -rf $RPM_BUILD_ROOT/build

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
/usr/lib/*.so*

%files devel
%defattr(-,root,root,-)
/usr/bin/*
/usr/lib/*.a
/usr/include/*
%doc /usr/share/doc/xrtti-%{version}

%changelog
* Tue Feb 10 2009  <bryan@ischo.com> Bryan Ischo
- Converted to new build

* Wed Jul 04 2007  <bryan@ischo,com> Bryan Ischo
- 0.1 -> trunk
- Removed xrttigenwrap
- Updated summary

* Fri May 04 2007  <bryan@ischo.com> Bryan Ischo
- Removed debuginfo RPM from the build

* Wed May 02 2007  <bryan@ischo.com> Bryan Ischo
- Remove tmp directory before packaging

* Sun Apr 29 2007  <bryan@ischo.com> Bryan Ischo
- Changed essence to essential

* Sat Apr 28 2007  <bryan@ischo.com> Bryan Ischo
- Changed doc directory to include version

* Fri Apr 27 2007  <bryan@ischo.com> Bryan Ischo
- Initial build.
