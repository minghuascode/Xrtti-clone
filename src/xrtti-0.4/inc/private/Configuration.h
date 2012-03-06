/*****************************************************************************\
 *                                                                           *
 * Configuration.h                                                           *
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
 * Defines an object which reads in and parses program variables and         *
 * configuration options for xrttigen.                                       *
 *                                                                           *
\*****************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


class Configuration
{
public:

    class Clude
    {
    public:

        enum Type
            {
                TypeIn, TypeEx
            };

        enum Match
            {
                MatchExact, MatchSubOf, MatchHas
            };

        Clude(Type type, const std::string &name);

        bool Matches(const std::string &name);

        Type type;
        Match match;
        std::string name;
        bool wildcardStartM, wildcardEndM;
    };

    Configuration(int argc, char **argv);

	virtual ~Configuration();

    u32 GetDefinitionCount() const
    {
        return vDefinitionsM.size();
    }

    std::string GetDefinition(u32 index) const
    {
        return vDefinitionsM[index];
    }

    u32 GetIncludeCount() const
    {
        return vIncludesM.size();
    }

    std::string GetInclude(u32 index) const
    {
        return vIncludesM[index];
    }

    bool GetRtti() const
    {
        return !disableRttiM;
    }

    u32 GetHeaderCount() const
    {
        return vHeadersM.size();
    }

    std::string GetHeader(u32 index) const
    {
        return vHeadersM[index];
    }

    u32 GetInputCount() const
    {
        return vInputsM.size();
    }

    std::string GetInput(u32 index) const
    {
        return vInputsM[index];
    }

    std::string GetOutputFile() const
    {
        return outFileM;
    }

    std::string GetTempFile() const
    {
        return tmpFileM;
    }

    u32 GetCludeCount() const
    {
        return vCludesM.size();
    }

    const Clude GetClude(u32 index) const
    {
        return *(vCludesM[index]);
    }

    bool ShouldInclude(const std::string &name) const;

private:

    std::vector<std::string> vDefinitionsM;
    std::vector<std::string> vIncludesM;
    std::vector<Clude *> vCludesM;
    std::vector<std::string> vHeadersM;
    bool disableRttiM;
    std::string outFileM;
    std::string tmpFileM;
    std::vector<std::string> vInputsM;
};

}; // namespace Xrtti

#endif // CONFIGURATION_H
