/*****************************************************************************\
 *                                                                           *
 * Configuration.cpp                                                         *
 *                                                                           *
 * ------------------------------------------------------------------------- *
 * Copyright (C) 2007 Bryan Ischo <bryan@ischo.com>                          *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License Version 2 as published  *
 * by the Free Software Foundation.                                          *
 *                                                                           *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General *
 * Public License for more details.                                          *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to:                                *
 * The Free Software Foundation, Inc.                                        *
 * 51 Franklin Street, Fifth Floor                                           *
 * Boston, MA 02110-1301, USA.                                               *
 * ------------------------------------------------------------------------- *
 *                                                                           *
\*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <private/StringUtils.h>
#include <private/Configuration.h>

using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


static const char *usageMessageG = 
    "Usage: xrttigen [-D <definition>]... [-I <include_directory>]...\n"
    "                [-e <exclude_spec>]... [-h <header_file>]\n"
    "                [-i <include_spec>]... [-n] [-o <output_file>]\n"
    "                [-t <tmp file>] input_header_file...\n\n"
    "  -D:   Defines a preprocessor macro to be used when processing all "
    "input\n        header fles.\n"
    "  -I:   Specifies a directory to search for header files included by "
    "input\n        header files.\n"
    "  -e:   Gives a specification of a class or a set of classes to exclude "
    "from\n        generation.  Includes and excludes are evaluated in the "
    "order that\n        they appear on the command line, with later includes "
    "and excludes\n        taking precedence.\n"
    "  -h:   Adds a header file to the set of header files to include in the\n"
    "        generated output.  This is typically the same set of header "
    "files\n        used in the generation of the input gccxml output.\n"
    "  -i:   Gives a specification of a class or a set of classes to "
    "include in\n        generation.  Includes and excludes are evaluated in "
    "the order that\n        they appear on the command line, with later "
    "includes and excludes\n        taking precedence.\n"
    "  -n:   If present disables C++ RTTI support in the generated Xrtti "
    "code\n        this flag would be specified if your C++ code is "
    "being built without\n        C++ rtti support.\n"
    "  -o:   Names the output file to write the generated source to.  A "
    "value of\n        dash (-) indicates stdout.  Default is stdout.\n"
    "  -t:   Names the temporary file which should be used to store gccxml "
    "xml\n        output; this file will be created and then deleted during "
    "the run of\n        xrttigen.  Defaults to gccxml.out in the current "
    "directory.\n\n"
    "  input_header_file: Specifies a header files to process.\n\n"
    "  All names used with the -e and -i arguments may use simple "
    "wildcarding\n"
    "  in which either the first or last character of the name is an "
    "asterisk.\n\n";


static void UsageExit(bool stdout)
{
    if (stdout) {
        printf("%s", usageMessageG);
    }
    else {
        fprintf(stderr, "%s", usageMessageG);
    }

    exit(-1);
}


static inline bool IsIdentifierBeginning(const char &uc)
{
    return (((uc >= 'a') && (uc <= 'z')) || ((uc >= 'A') && (uc <= 'Z')) ||
            (uc == '_'));
}


static inline bool IsIdentifierPostBeginning(const char &uc)
{
    return (IsIdentifierBeginning(uc) || ((uc >= '0') && (uc <= '9')));
}


static bool IsOption(char *arg, const char *shortopt, const char *longopt)
{
	return (!strcmp(arg, shortopt) || !strcmp(arg, longopt));
}


Configuration::Configuration(int argc, char **argv)
	: disableRttiM(false), outFileM("-"), tmpFileM("gccxml.out")
{
	int i;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		}

		if (IsOption(argv[i], "-?", "--help")) {
			UsageExit(true);
		}
		else if (IsOption(argv[i], "-D", "--define")) {
			if (++i == argc) {
				UsageExit(false);
			}
			vDefinitionsM.push_back(argv[i]);
		}
		else if (IsOption(argv[i], "-I", "--incdir")) {
			if (++i == argc) {
				UsageExit(false);
			}
			vIncludesM.push_back(argv[i]);
		}
		else if (IsOption(argv[i], "-h", "--header")) {
			if (++i == argc) {
				UsageExit(false);
			}
			vHeadersM.push_back(argv[i]);
		}
		else if (IsOption(argv[i], "-n", "no-rtti")) {
			disableRttiM = true;
		}
		else if (IsOption(argv[i], "-o", "output")) {
			if (++i == argc) {
				UsageExit(false);
			}
			outFileM = argv[i];
		}
		else if (IsOption(argv[i], "-t", "tmpfile")) {
			if (++i == argc) {
				UsageExit(false);
			}
			tmpFileM = argv[i];
		}
		else if (IsOption(argv[i], "-i", "include")) {
			if (++i == argc) {
				UsageExit(false);
			}
            vCludesM.push_back(new Clude(Clude::TypeIn, argv[i]));
		}
		else if (IsOption(argv[i], "-e", "exclude")) {
			if (++i == argc) {
				UsageExit(false);
			}
            vCludesM.push_back(new Clude(Clude::TypeEx, argv[i]));
		}
		else {
			UsageExit(false);
		}
	}

	if (i == argc) {
        fprintf(stderr, "\nERROR: No input header files specified (at least "
                "one is required).\n\n");
        UsageExit(false);
	}

	for ( ; i < argc; i++) {
		vInputsM.push_back(argv[i]);
    }
}


Configuration::~Configuration()
{
	for (u32 i = 0; i < vCludesM.size(); i++) {
		delete vCludesM[i];
	}
}


bool Configuration::ShouldInclude(const string &name) const
{
    // Walk the list of "cludes" backwards, since each later one takes
    // precedence over prior ones
    u32 cludeCount = vCludesM.size();

    for (u32 i = cludeCount; i > 0; i--) {
        Clude *pClude = vCludesM[i - 1];

        // See if it's in there
        if (!pClude->Matches(name)) {
            // Nope, go to the next clude
            continue;
        }

        // OK, it matches - what does that mean?
        return (pClude->type == Clude::TypeIn) ? true : false;
    }

    // Default is to include everything
    return true;
}


Configuration::Clude::Clude(Clude::Type theType, const string &theName)
{
    type = theType;

    if (StringUtils::StartsWith(theName, "subof:")) {
        match = MatchSubOf;
        name = string(theName, 0, 6);
    }
    else if (StringUtils::StartsWith(theName, "has:")) {
        match = MatchHas;
        name = string(theName, 0, 4);
    }
    else {
        match = MatchExact;
        name = theName;
    }

    if (StringUtils::StartsWith(name, "*")) {
        wildcardStartM = true;
        wildcardEndM = false;
        name = string(name, 1, name.length() - 1);
    }
    else if (StringUtils::EndsWith(name, "*")) {
        wildcardStartM = false;
        wildcardEndM = true;
        name = string(name, 0, name.length() - 1);
    }
    else {
        wildcardStartM = wildcardEndM = false;
    }
}


bool Configuration::Clude::Matches(const string &otherName)
{
    if (wildcardStartM) {
        return StringUtils::EndsWith(otherName, name);
    }
    else if (wildcardEndM) {
        return StringUtils::StartsWith(otherName, name);
    }
    else {
        return (otherName == name);
    }
}


}; // namespace Xrtti
