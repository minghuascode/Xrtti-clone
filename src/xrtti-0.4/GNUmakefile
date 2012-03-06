# -*- mode: makefile -*- ---------------------------------------------------
#
# xrtti make file
#
# --------------------------------------------------------------------------
# Copyright (C) 2007-2009 Bryan Ischo <bryan@ischo.com>
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License Version 2 as published
# by the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to:
# The Free Software Foundation, Inc.
# 51 Franklin Street, Fifth Floor
# Boston, MA 02110-1301, USA.
# --------------------------------------------------------------------------
#
# --------------------------------------------------------------------------


# I tried to use the autoconf/automake/autolocal/etc (i.e. autohell) tools
# but I just couldn't stomach them.  Since this is a Makefile for POSIX
# systems, I will simply do away with autohell completely and use a GNU
# Makefile.  GNU make ought to be available pretty much everywhere, so I
# don't see this being a significant issue for portability.

# All commands assume a GNU compiler.  For systems which do not use a GNU
# compiler, write scripts with the same names as these commands, and taking
# the same arguments, and translate the arguments and commands into the
# appropriate non-POSIX ones as needed.  xrtti assumes a GNU toolchain as
# the most portable way to build software possible.  Non-POSIX, non-GNU
# systems can do the work of supporting this build infrastructure.


# --------------------------------------------------------------------------
# Set xrtti version number

XRTTI_VER_MAJOR := 0
XRTTI_VER_MINOR := 4
XRTTI_VER := $(XRTTI_VER_MAJOR).$(XRTTI_VER_MINOR)


# -----------------------------------------------------------------------------
# Determine verbosity.  VERBOSE_SHOW should be prepended to every command which
# should only be displayed if VERBOSE is set.  QUIET_ECHO may be used to
# echo text only if VERBOSE is not set.  Typically, a VERBOSE_SHOW command will
# be paired with a QUIET_ECHO command, to provide a command which is displayed
# in VERBOSE mode, along with text which is displayed in non-VERBOSE mode to
# describe the command.
#
# No matter what VERBOSE is defined to, it ends up as true if it's defined.
# This will be weird if you defined VERBOSE=false in the environment, and we
# switch it to true here; but the meaning of VERBOSE is, "if it's defined to
# any value, then verbosity is turned on".  So don't define VERBOSE if you
# don't want verbosity in the build process.
# -----------------------------------------------------------------------------

ifdef VERBOSE
        VERBOSE = true
        VERBOSE_ECHO = @ echo
        VERBOSE_SHOW =
        QUIET_ECHO = @ echo > /dev/null
        DOXYGEN_EXTRA =
else
        VERBOSE = false
        VERBOSE_ECHO = @ echo > /dev/null
        VERBOSE_SHOW = @
        QUIET_ECHO = @ echo
        DOXYGEN_EXTRA = >/dev/null
endif


# --------------------------------------------------------------------------
# OUTPUT directory
ifndef OUTPUT
    ifdef DEBUG
        OUTPUT := build-debug
    else
        OUTPUT := build
    endif
endif


# --------------------------------------------------------------------------
# DESTDIR directory
ifndef DESTDIR
    DESTDIR := /usr
endif


# --------------------------------------------------------------------------
# Acquire configuration information for libraries that xrtti depends upon

ifndef EXPAT_LIBS
    EXPAT_LIBS := -lexpat
endif

ifndef EXPAT_CFLAGS
    EXPAT_CFLAGS :=
endif


# --------------------------------------------------------------------------
# These CFLAGS assume a GNU compiler.  For other compilers, write a script
# which converts these arguments into their equivalent for that particular
# compiler.

ifndef CFLAGS
    ifdef DEBUG
        CFLAGS := -g
    else
        CFLAGS := -O3
    endif
endif

CFLAGS += -Wall -Werror -Wshadow -Wextra -Wno-invalid-offsetof -Iinc \
          $(EXPAT_CFLAGS) \
          -DXRTTI_VER_MAJOR=\"$(XRTTI_VER_MAJOR)\" \
          -DXRTTI_VER_MINOR=\"$(XRTTI_VER_MINOR)\" \
          -DXRTTI_VER=\"$(XRTTI_VER)\" \
          -D__STRICT_ANSI__ \
          -D_ISOC99_SOURCE \
          -D_POSIX_C_SOURCE=200112L


# --------------------------------------------------------------------------
# Default targets are everything

.PHONY: all
all: exported tests


# --------------------------------------------------------------------------
# Exported targets are the library and driver program

.PHONY: exported
exported: libxrtti libxrttiparsed xrttigen headers


# --------------------------------------------------------------------------
# API documentation target

.PHONY: apidocs
apidocs:
	$(QUIET_ECHO) $(OUTPUT)/share/doc/xrtti-$(XRTTI_VER): Generating API docs
	@ mkdir -p $(OUTPUT)/share/doc/xrtti-$(XRTTI_VER)
	$(VERBOSE_SHOW) OUTPUT=$(OUTPUT) XRTTI_VER=$(XRTTI_VER) \
        doxygen doc/doxyfile $(DOXYGEN_EXTRA)


# --------------------------------------------------------------------------
# Install target

.PHONY: install
install: exported apidocs
	$(QUIET_ECHO) $(DESTDIR)/bin/xrttigen: Installing executable
	$(VERBOSE_SHOW) install -Dps -m u+rwx,go+rx $(OUTPUT)/bin/xrttigen \
                    $(DESTDIR)/bin/xrttigen
	$(QUIET_ECHO) $(DESTDIR)/include/Xrtti/Xrtti.h: Installing header
	$(VERBOSE_SHOW) install -Dp -m u+rw,go+r $(OUTPUT)/include/Xrtti/Xrtti.h \
                    $(DESTDIR)/include/Xrtti/Xrtti.h
	$(QUIET_ECHO) $(DESTDIR)/include/Xrtti/XrttiParsed.h: Installing header
	$(VERBOSE_SHOW) install -Dp -m u+rw,go+r \
                    $(OUTPUT)/include/Xrtti/XrttiParsed.h \
                    $(DESTDIR)/include/Xrtti/XrttiParsed.h
	$(QUIET_ECHO) $(DESTDIR)/include/Xrtti/XrttiPrivate.h: Installing header
	$(VERBOSE_SHOW) install -Dp -m u+rw,go+r \
                    $(OUTPUT)/include/Xrtti/XrttiPrivate.h \
                    $(DESTDIR)/include/Xrtti/XrttiPrivate.h
	$(QUIET_ECHO) $(DESTDIR)/lib/libxrtti.a: Installing static library
	$(VERBOSE_SHOW) install -Dp -m u+rw,go+r $(OUTPUT)/lib/libxrtti.a \
                    $(DESTDIR)/lib/libxrtti.a
	$(QUIET_ECHO) $(DESTDIR)/lib/libxrttiparsed.a: Installing static library
	$(VERBOSE_SHOW) install -Dp -m u+rw,go+r $(OUTPUT)/lib/libxrttiparsed.a \
                    $(DESTDIR)/lib/libxrttiparsed.a
	$(QUIET_ECHO) \
    $(DESTDIR)/lib/libxrtti.so.$(XRTTI_VER): Installing shared library
	$(VERBOSE_SHOW) install -Dps -m u+rw,go+r \
                    $(OUTPUT)/lib/libxrtti.so.$(XRTTI_VER_MAJOR) \
                    $(DESTDIR)/lib/libxrtti.so.$(XRTTI_VER)
	$(QUIET_ECHO) \
    $(DESTDIR)/lib/libxrtti.so.$(XRTTI_VER_MAJOR): Linking shared library
	$(VERBOSE_SHOW) ln -sf libxrtti.so.$(XRTTI_VER) \
                    $(DESTDIR)/lib/libxrtti.so.$(XRTTI_VER_MAJOR)
	$(QUIET_ECHO) $(DESTDIR)/lib/libxrtti.so: Linking shared library
	$(VERBOSE_SHOW) ln -sf libxrtti.so.$(XRTTI_VER_MAJOR) \
                    $(DESTDIR)/lib/libxrtti.so
	$(QUIET_ECHO) \
    $(DESTDIR)/lib/libxrttiparsed.so.$(XRTTI_VER): Installing shared library
	$(VERBOSE_SHOW) install -Dps -m u+rw,go+r \
                    $(OUTPUT)/lib/libxrttiparsed.so.$(XRTTI_VER_MAJOR) \
                    $(DESTDIR)/lib/libxrttiparsed.so.$(XRTTI_VER)
	$(QUIET_ECHO) \
    $(DESTDIR)/lib/libxrttiparsed.so.$(XRTTI_VER_MAJOR): Linking shared library
	$(VERBOSE_SHOW) ln -sf libxrttiparsed.so.$(XRTTI_VER) \
                    $(DESTDIR)/lib/libxrttiparsed.so.$(XRTTI_VER_MAJOR)
	$(QUIET_ECHO) $(DESTDIR)/lib/libxrttiparsed.so: Linking shared library
	$(VERBOSE_SHOW) ln -sf libxrttiparsed.so.$(XRTTI_VER_MAJOR) \
                    $(DESTDIR)/lib/libxrttiparsed.so
	$(QUIET_ECHO) $(DESTDIR)/share/doc/xrtti-$(XRTTI_VER): Installing API docs
	@ mkdir -p $(DESTDIR)/share/doc
	$(VERBOSE_SHOW) cp -r $(OUTPUT)/share/doc/xrtti-$(XRTTI_VER) \
                    $(DESTDIR)/share/doc


# --------------------------------------------------------------------------
# Uninstall target

.PHONY: uninstall
uninstall:
	$(QUIET_ECHO) Installed files: Uninstalling
	$(VERBOSE_SHOW) \
          rm -rf $(DESTDIR)/bin/xrttigen \
                 $(DESTDIR)/include/Xrtti/Xrtti.h \
                 $(DESTDIR)/include/Xrtti/XrttiParsed.h \
                 $(DESTDIR)/include/Xrtti/XrttiPrivate.h \
                 $(DESTDIR)/lib/libxrtti.so \
                 $(DESTDIR)/lib/libxrtti.so.$(XRTTI_VER_MAJOR) \
                 $(DESTDIR)/lib/libxrtti.so.$(XRTTI_VER) \
                 $(DESTDIR)/lib/libxrtti.a \
                 $(DESTDIR)/lib/libxrttiparsed.so \
                 $(DESTDIR)/lib/libxrttiparsed.so.$(XRTTI_VER_MAJOR) \
                 $(DESTDIR)/lib/libxrttiparsed.so.$(XRTTI_VER) \
                 $(DESTDIR)/lib/libxrttiparsed.a \
                 $(DESTDIR)/share/doc/xrtti-$(XRTTI_VER)


# --------------------------------------------------------------------------
# Compile target patterns

$(OUTPUT)/obj/%.o: src/%.cpp
	$(QUIET_ECHO) $@: Compiling object
	@ mkdir -p $(dir $(OUTPUT)/dep/$<)
	@ g++ $(CFLAGS) -M -MG -MQ $@ -DCOMPILINGDEPENDENCIES \
        -o $(OUTPUT)/dep/$(<:%.cpp=%.d) -c $<
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ $(CFLAGS) -o $@ -c $<

$(OUTPUT)/obj/%.do: src/%.cpp
	$(QUIET_ECHO) $@: Compiling dynamic object
	@ mkdir -p $(dir $(OUTPUT)/dep/$<)
	@ g++ $(CFLAGS) -M -MG -MQ $@ -DCOMPILINGDEPENDENCIES \
        -o $(OUTPUT)/dep/$(<:%.cpp=%.dd) -c $<
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ $(CFLAGS) -fpic -fPIC -o $@ -c $< 


# --------------------------------------------------------------------------
# libxrtti targets

LIBXRTTI_STATIC = $(OUTPUT)/lib/libxrtti.a
LIBXRTTI_SHARED = $(OUTPUT)/lib/libxrtti.so.$(XRTTI_VER_MAJOR)

.PHONY: libxrtti
libxrtti: $(LIBXRTTI_STATIC) $(LIBXRTTI_SHARED)

LIBXRTTI_SOURCES := Xrtti/Argument.cpp \
                    Xrtti/Array.cpp \
                    Xrtti/Base.cpp \
                    Xrtti/Compiled.cpp \
                    Xrtti/CompiledContextSet.cpp \
                    Xrtti/Constructor.cpp \
                    Xrtti/ConstructorSignature.cpp \
                    Xrtti/Context.cpp \
                    Xrtti/Destructor.cpp \
                    Xrtti/DestructorSignature.cpp \
                    Xrtti/Enumeration.cpp \
                    Xrtti/EnumerationValue.cpp \
                    Xrtti/Field.cpp \
                    Xrtti/Member.cpp \
                    Xrtti/Method.cpp \
                    Xrtti/MethodSignature.cpp \
                    Xrtti/Pointer.cpp \
                    Xrtti/StringUtils.cpp \
                    Xrtti/Struct.cpp \
                    Xrtti/Structure.cpp \
                    Xrtti/Type.cpp \
                    Xrtti/TypeEnumeration.cpp \
                    Xrtti/TypeFunction.cpp \
                    Xrtti/TypeStructure.cpp \
                    Xrtti/Xrtti.cpp

ALL_SOURCES := $(ALL_SOURCES) $(LIBXRTTI_SOURCES)

LIBXRTTI_OBJECTS = $(LIBXRTTI_SOURCES:%.cpp=$(OUTPUT)/obj/%.o)

LIBXRTTI_FPIC_OBJECTS = $(LIBXRTTI_SOURCES:%.cpp=$(OUTPUT)/obj/%.do)

$(LIBXRTTI_STATIC): $(LIBXRTTI_OBJECTS)
	$(QUIET_ECHO) $@: Building static library
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) $(AR) cr $@ $^

$(LIBXRTTI_SHARED): $(LIBXRTTI_FPIC_OBJECTS)
	$(QUIET_ECHO) $@: Building shared library
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) gcc -shared -Wl,-soname,libxrtti.so.$(XRTTI_VER_MAJOR) \
        -o $@ $^


# --------------------------------------------------------------------------
# libxrttiparsed targets

LIBXRTTIPARSED_STATIC = $(OUTPUT)/lib/libxrttiparsed.a
LIBXRTTIPARSED_SHARED = $(OUTPUT)/lib/libxrttiparsed.so.$(XRTTI_VER_MAJOR)

.PHONY: libxrttiparsed
libxrttiparsed: $(LIBXRTTIPARSED_SHARED) $(LIBXRTTIPARSED_STATIC)

LIBXRTTIPARSED_SOURCES = Xrtti/Parsed.cpp \
                         Xrtti/ParsedArgument.cpp \
                         Xrtti/ParsedBase.cpp \
                         Xrtti/ParsedConstructor.cpp \
                         Xrtti/ParsedConstructorSignature.cpp \
                         Xrtti/ParsedContext.cpp \
                         Xrtti/ParsedContextSet.cpp \
                         Xrtti/ParsedDestructor.cpp \
                         Xrtti/ParsedDestructorSignature.cpp \
                         Xrtti/ParsedEnumeration.cpp \
                         Xrtti/ParsedEnumerationValue.cpp \
                         Xrtti/ParsedField.cpp \
                         Xrtti/ParsedMember.cpp \
                         Xrtti/ParsedMethod.cpp \
                         Xrtti/ParsedMethodSignature.cpp \
                         Xrtti/ParsedStruct.cpp \
                         Xrtti/ParsedStructure.cpp \
                         Xrtti/ParsedType.cpp \
                         Xrtti/ParsedTypeEnumeration.cpp \
                         Xrtti/ParsedTypeFunction.cpp \
                         Xrtti/ParsedTypeStructure.cpp \
                         Xrtti/Parser.cpp

ALL_SOURCES := $(ALL_SOURCES) $(LIBXRTTIPARSED_SOURCES)

LIBXRTTIPARSED_OBJECTS = $(LIBXRTTIPARSED_SOURCES:%.cpp=$(OUTPUT)/obj/%.o)

LIBXRTTIPARSED_FPIC_OBJECTS = $(LIBXRTTIPARSED_SOURCES:%.cpp=$(OUTPUT)/obj/%.do)

$(LIBXRTTIPARSED_STATIC): $(LIBXRTTIPARSED_OBJECTS)
	$(QUIET_ECHO) $@: Building static library
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) $(AR) cr $@ $^

$(LIBXRTTIPARSED_SHARED): $(LIBXRTTIPARSED_FPIC_OBJECTS)
	$(QUIET_ECHO) $@: Building shared library
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) gcc -shared \
        -Wl,-soname,libxrttiparsed.so.$(XRTTI_VER_MAJOR) -o $@ $^


# --------------------------------------------------------------------------
# xrttigen targets

XRTTIGEN = $(OUTPUT)/bin/xrttigen

.PHONY: xrttigen
xrttigen: $(XRTTIGEN)

XRTTIGEN_SOURCES = xrttigen/Configuration.cpp \
                   xrttigen/Generator.cpp \
                   xrttigen/GeneratorArgument.cpp \
                   xrttigen/GeneratorArray.cpp \
                   xrttigen/GeneratorBase.cpp \
                   xrttigen/GeneratorConstructor.cpp \
                   xrttigen/GeneratorConstructorSignature.cpp \
                   xrttigen/GeneratorContext.cpp \
                   xrttigen/GeneratorDestructor.cpp \
                   xrttigen/GeneratorDestructorSignature.cpp \
                   xrttigen/GeneratorEnumeration.cpp \
                   xrttigen/GeneratorEnumerationValue.cpp \
                   xrttigen/GeneratorField.cpp \
                   xrttigen/GeneratorMember.cpp \
                   xrttigen/GeneratorMethod.cpp \
                   xrttigen/GeneratorMethodSignature.cpp \
                   xrttigen/GeneratorObject.cpp \
                   xrttigen/GeneratorPointer.cpp \
                   xrttigen/GeneratorStruct.cpp \
                   xrttigen/GeneratorStructure.cpp \
                   xrttigen/GeneratorType.cpp \
                   xrttigen/GeneratorTypeEnumeration.cpp \
                   xrttigen/GeneratorTypeFunction.cpp \
                   xrttigen/GeneratorTypeStructure.cpp \
                   xrttigen/xrttigen.cpp

ALL_SOURCES := $(ALL_SOURCES) $(XRTTIGEN_SOURCES)

XRTTIGEN_OBJECTS = $(XRTTIGEN_SOURCES:%.cpp=$(OUTPUT)/obj/%.o)

$(XRTTIGEN): $(XRTTIGEN_OBJECTS) $(LIBXRTTI_SHARED) $(LIBXRTTIPARSED_SHARED)
	$(QUIET_ECHO) $@: Building executable
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ -o $@ $^ -L$(OUTPUT)/lib $(EXPAT_LIBS)


# --------------------------------------------------------------------------
# xrtti header targets

.PHONY: headers
headers: $(OUTPUT)/include/Xrtti/Xrtti.h $(OUTPUT)/include/Xrtti/XrttiParsed.h \
         $(OUTPUT)/include/Xrtti/XrttiPrivate.h

$(OUTPUT)/include/Xrtti/Xrtti.h: inc/Xrtti/Xrtti.h
	$(QUIET_ECHO) $@: Linking header
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) ln -sf $(abspath $<) $@

$(OUTPUT)/include/Xrtti/XrttiParsed.h: inc/Xrtti/XrttiParsed.h
	$(QUIET_ECHO) $@: Linking header
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) ln -sf $(abspath $<) $@

$(OUTPUT)/include/Xrtti/XrttiPrivate.h: inc/Xrtti/XrttiPrivate.h
	$(QUIET_ECHO) $@: Linking header
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) ln -sf $(abspath $<) $@


# --------------------------------------------------------------------------
# Test targets

TESTCLASSES = $(OUTPUT)/bin/TestClasses
TESTCOMPILED = $(OUTPUT)/bin/TestCompiled
TESTMETHODS = $(OUTPUT)/bin/TestMethods
TESTPARSED = $(OUTPUT)/bin/TestParsed

.PHONY: TestClasses
TestClasses: $(TESTCLASSES)

.PHONY: TestCompiled
TestCompiled: $(TESTCOMPILED)

.PHONY: TestMethods
TestMethods: $(TESTMETHODS)

.PHONY: TestParsed
TestParsed: $(TESTPARSED)

.PHONY: tests
tests: $(TESTCLASSES) $(TESTCOMPILED) $(TESTMETHODS) $(TESTPARSED)

vpath %.cpp $(OUTPUT) $(shell mkdir -p $(OUTPUT))

TESTCLASSES_SOURCES = test/TestClasses.cpp \
                      test/Util.cpp \
                      TestClasses_Generated.cpp

ALL_SOURCES := $(ALL_SOURCES) test/TestClasses.cpp test/Util.cpp

$(OUTPUT)/src/TestClasses_Generated.cpp: inc/test/TestClasses.h $(XRTTIGEN)
	$(QUIET_ECHO) $@: Generating xrtti
	@ mkdir -p $(dir $@)
	@ mkdir -p $(OUTPUT)/tmp
	$(VERBOSE_SHOW) LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(OUTPUT)/lib \
        $(XRTTIGEN) -I inc -h "test/TestClasses.h" -o $@ \
        -t $(OUTPUT)/tmp/$(notdir $(*:.cpp=.xml)) $<


$(TESTCLASSES): $(TESTCLASSES_SOURCES:%.cpp=$(OUTPUT)/obj/%.o) \
                $(LIBXRTTI_SHARED)
	$(QUIET_ECHO) $@: Building executable
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ -o $@ $^ -L$(OUTPUT)/lib


TESTCOMPILED_SOURCES = test/TestCompiled.cpp \
                       test/XrttiToCpp.cpp \
                       TestCompiled_Generated.cpp

ALL_SOURCES := $(ALL_SOURCES) test/TestCompiled.cpp test/XrttiToCpp.cpp

$(OUTPUT)/src/TestCompiled_Generated.cpp: inc/test/TestCoverage.h $(XRTTIGEN)
	$(QUIET_ECHO) $@: Generating xrtti
	@ mkdir -p $(dir $@)
	@ mkdir -p $(OUTPUT)/tmp
	$(VERBOSE_SHOW) LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(OUTPUT)/lib \
        $(XRTTIGEN) -I inc -h "test/TestCoverage.h" -o $@ \
        -t $(OUTPUT)/tmp/$(notdir $(*:.cpp=.xml)) $<

$(TESTCOMPILED): $(TESTCOMPILED_SOURCES:%.cpp=$(OUTPUT)/obj/%.o) \
                $(LIBXRTTI_SHARED)
	$(QUIET_ECHO) $@: Building executable
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ -o $@ $^ -L$(OUTPUT)/lib


TESTMETHODS_SOURCES = test/TestMethods.cpp \
                      TestMethods_Generated.cpp

ALL_SOURCES := $(ALL_SOURCES) test/TestMethods.cpp

$(OUTPUT)/src/TestMethods_Generated.cpp: inc/test/TestMethods.h $(XRTTIGEN)
	$(QUIET_ECHO) $@: Generating xrtti
	@ mkdir -p $(dir $@)
	@ mkdir -p $(OUTPUT)/tmp
	$(VERBOSE_SHOW) LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(OUTPUT)/lib \
        $(XRTTIGEN) -I inc -h "test/TestMethods.h" -o $@ \
        -t $(OUTPUT)/tmp/$(notdir $(*:.cpp=.xml)) -e '*' -i TestMethods $<

$(TESTMETHODS): $(TESTMETHODS_SOURCES:%.cpp=$(OUTPUT)/obj/%.o) \
                $(LIBXRTTI_SHARED)
	$(QUIET_ECHO) $@: Building executable
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ -o $@ $^ -L$(OUTPUT)/lib


TESTPARSED_SOURCES = test/TestParsed.cpp \
                     test/XrttiToCpp.cpp \
                     xrttigen/Configuration.cpp

ALL_SOURCES := $(ALL_SOURCES) test/TestParsed.cpp

$(TESTPARSED): $(TESTPARSED_SOURCES:%.cpp=$(OUTPUT)/obj/%.o) \
               $(LIBXRTTIPARSED_SHARED) $(LIBXRTTI_SHARED)
	$(QUIET_ECHO) $@: Building executable
	@ mkdir -p $(dir $@)
	$(VERBOSE_SHOW) g++ -o $@ $^ -L$(OUTPUT)/lib $(EXPAT_LIBS)


# --------------------------------------------------------------------------
# Clean target

.PHONY: clean
clean:
	$(QUIET_ECHO) $@: Cleaning
	$(VERBOSE_SHOW) rm -rf $(OUTPUT)


# --------------------------------------------------------------------------
# Clean dependencies target

.PHONY: cleandeps
cleandeps:
	$(QUIET_ECHO) $@: Cleaning dependencies
	$(VERBOSE_SHOW) rm -rf $(OUTPUT)/dep


# --------------------------------------------------------------------------
# Dependencies

$(foreach i, $(ALL_SOURCES), $(eval -include $(OUTPUT)/dep/src/$(i:%.cpp=%.d)))
$(foreach i, $(ALL_SOURCES), $(eval -include $(OUTPUT)/dep/src/$(i:%.cpp=%.dd)))


# --------------------------------------------------------------------------
# Debian package target

DEBPKG = $(OUTPUT)/pkg/xrtti_$(XRTTI_VER).deb
DEBDEVPKG = $(OUTPUT)/pkg/xrtti-dev_$(XRTTI_VER).deb

.PHONY: deb
deb: $(DEBPKG) $(DEBDEVPKG)

$(DEBPKG): DEBARCH = $(shell dpkg-architecture | grep ^DEB_BUILD_ARCH= | \
                       cut -d '=' -f 2)
$(DEBPKG): exported $(OUTPUT)/deb/DEBIAN/control $(OUTPUT)/deb/DEBIAN/shlibs \
           $(OUTPUT)/deb/DEBIAN/postinst \
           $(OUTPUT)/deb/usr/share/doc/xrtti/changelog.gz \
           $(OUTPUT)/deb/usr/share/doc/xrtti/changelog.Debian.gz \
           $(OUTPUT)/deb/usr/share/doc/xrtti/copyright
	DESTDIR=$(OUTPUT)/deb/usr $(MAKE) install
	rm -rf $(OUTPUT)/deb/usr/bin
	rm -rf $(OUTPUT)/deb/usr/include
	rm -rf $(OUTPUT)/deb/usr/share
	rm -f $(OUTPUT)/deb/usr/lib/*.a
	@ mkdir -p $(dir $@)
	fakeroot dpkg-deb -b $(OUTPUT)/deb $@
	mv $@ $(OUTPUT)/pkg/xrtti_$(XRTTI_VER)_$(DEBARCH).deb

$(DEBDEVPKG): DEBARCH = $(shell dpkg-architecture | grep ^DEB_BUILD_ARCH= | \
                          cut -d '=' -f 2)
$(DEBDEVPKG): exported $(OUTPUT)/deb-dev/DEBIAN/control \
           $(OUTPUT)/deb-dev/usr/share/doc/xrtti-dev/changelog.gz \
           $(OUTPUT)/deb-dev/usr/share/doc/xrtti-dev/changelog.Debian.gz \
           $(OUTPUT)/deb-dev/usr/share/doc/xrtti-dev/copyright
	DESTDIR=$(OUTPUT)/deb-dev/usr $(MAKE) install
	rm -f $(OUTPUT)/deb-dev/usr/lib/*.so*
	@ mkdir -p $(dir $@)
	fakeroot dpkg-deb -b $(OUTPUT)/deb-dev $@
	mv $@ $(OUTPUT)/pkg/xrtti-dev_$(XRTTI_VER)_$(DEBARCH).deb

$(OUTPUT)/deb/DEBIAN/control: debian/control
	@mkdir -p $(dir $@)
	echo -n "Depends: " > $@
	dpkg-shlibdeps -O $(OUTPUT)/lib/libxrtti.so.$(XRTTI_VER_MAJOR) \
            $(OUTPUT)/lib/libxrttiparsed.so.$(XRTTI_VER_MAJOR) | \
            cut -d '=' -f 2- >> $@
	sed -e 's/XRTTI_VERSION/$(XRTTI_VER)/' \
            < $< | sed -e 's/DEBIAN_ARCHITECTURE/$(DEBARCH)/' | \
            grep -v ^Source: >> $@

$(OUTPUT)/deb-dev/DEBIAN/control: debian/control.dev
	@mkdir -p $(dir $@)
	sed -e 's/XRTTI_VERSION/$(XRTTI_VER)/' \
            < $< | sed -e 's/DEBIAN_ARCHITECTURE/$(DEBARCH)/' > $@

$(OUTPUT)/deb/DEBIAN/shlibs:
	echo -n "xrtti $(XRTTI_VER_MAJOR) xrtti " > $@
	echo "(>= $(XRTTI_VER))" >> $@

$(OUTPUT)/deb/DEBIAN/postinst: debian/postinst
	@mkdir -p $(dir $@)
	cp $< $@

$(OUTPUT)/deb/usr/share/doc/xrtti/copyright: LICENSE.TXT
	@mkdir -p $(dir $@)
	cp $< $@
	@echo >> $@
	@echo -n "An alternate location for the GNU General Public " >> $@
	@echo "License version 3 on Debian" >> $@
	@echo "systems is /usr/share/common-licenses/GPL-3." >> $@

$(OUTPUT)/deb-dev/usr/share/doc/xrtti-dev/copyright: LICENSE.TXT
	@mkdir -p $(dir $@)
	cp $< $@
	@echo >> $@
	@echo -n "An alternate location for the GNU General Public " >> $@
	@echo "License version 3 on Debian" >> $@
	@echo "systems is /usr/share/common-licenses/GPL-3." >> $@

$(OUTPUT)/deb/usr/share/doc/xrtti/changelog.gz: debian/changelog
	@mkdir -p $(dir $@)
	gzip --best -c $< > $@

$(OUTPUT)/deb-dev/usr/share/doc/xrtti-dev/changelog.gz: debian/changelog
	@mkdir -p $(dir $@)
	gzip --best -c $< > $@

$(OUTPUT)/deb/usr/share/doc/xrtti/changelog.Debian.gz: debian/changelog.Debian
	@mkdir -p $(dir $@)
	gzip --best -c $< > $@

$(OUTPUT)/deb-dev/usr/share/doc/xrtti-dev/changelog.Debian.gz: \
    debian/changelog.Debian
	@mkdir -p $(dir $@)
	gzip --best -c $< > $@
