/*****************************************************************************\
 *                                                                           *
 * Parser.h                                                                  *
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
 * Defines a parser class which handles parsing of gccxml output into a      *
 * set of C++ objects hashed by id.                                          *
 *                                                                           *
\*****************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <expat.h>
#include <map>
#include <string>
#include <vector>
#include <Xrtti/XrttiParsed.h>


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


// ---------------------------------------------------------------------------
// Parser
// ---------------------------------------------------------------------------
class Parser
{
public:

    enum AccessType
        {
            AccessTypePublic,
            AccessTypeProtected,
            AccessTypePrivate
        };

    enum SubType
        {
            SubTypeFundamental,
            SubTypePointer,
            SubTypeReference,
            SubTypeCvQualified,
            SubTypeFunction
        };

    // -----------------------------------------------------------------------
    // Parser::Attribute
    // -----------------------------------------------------------------------
    class Attribute
    {
    public:

        Attribute(char *pName, char *pValue);

        std::string GetName() const
        {
            return nameM;
        }

        std::string GetValue() const
        {
            return valueM;
        }
    private:

        const std::string nameM;
        const std::string valueM;
    };

    // -----------------------------------------------------------------------
    // Parser::Element
    // -----------------------------------------------------------------------
    class Element
    {
    public:

        Element(const Parser &parser, char *pElementName, char **pAttributes);

		~Element();

        std::string GetElementName() const
        {
            return elementNameM;
        }

        bool HasAttribute(const std::string &name) const
        {
            u32 count = this->GetAttributeCount();
            
            for (u32 i = 0; i < count; i++) {
                Attribute *pAttribute = this->GetAttribute(i);
                if (pAttribute->GetName() == name) {
                    return true;
                }
            }

            return false;
        }

        Attribute *GetAttribute(const std::string &name) const
        {
            u32 count = this->GetAttributeCount();
            
            for (u32 i = 0; i < count; i++) {
                Attribute *pAttribute = this->GetAttribute(i);
                if (pAttribute->GetName() == name) {
                    return pAttribute;
                }
            }

            return NULL;
        }

        std::string GetAttributeValue(const std::string &name) const
        {
            Attribute *pAttribute = this->GetAttribute(name);

            if (pAttribute == NULL) {
                return "";
            }

            return pAttribute->GetValue();
        }

        // This is the line number that the element was declared on
        u32 GetLineNumber() const
        {
            return lineNumberM;
        }

        Element *GetSubElement(const std::string &name) const
        {
            u32 count = this->GetSubElementCount();

            for (u32 i = 0; i < count; i++) {
                Element *pElement = this->GetSubElement(i);
                if (pElement->GetElementName() == name) {
                    return pElement;
                }
            }

            return NULL;
        }

        u32 GetAttributeCount() const
        {
            return attributesM.size();
        }

        Attribute *GetAttribute(u32 index) const
        {
            return attributesM[index];
        }

        u32 GetSubElementCount() const
        {
            return subElementsM.size();
        }
        
        Element *GetSubElement(u32 index) const
        {
            return subElementsM[index];
        }

    private:

        friend class Parser;

        const std::string elementNameM;
        std::vector<Attribute *> attributesM;
        std::vector<Element *> subElementsM;
        u32 lineNumberM;
    };

    // Creates a Parser object
    Parser();

	virtual ~Parser();

    // Parses the fd, which is an XML document as output by gccxml;
    // uses the given configuration settings
    void Parse(int fd);

	// Would prefer to use an iterator which does not expose the internal
	// data type used, but C++ STL is lame and doesn't support standard
	// iterators for map keps and values
	std::map<std::string, Element *>::iterator GetElementsBegin()
	{
		return htElementsM.begin();
	}

	std::map<std::string, Element *>::iterator GetElementsEnd()
	{
		return htElementsM.end();
	}

    // Get and return an element by its id
    Element *LookupElement(const std::string &id)
    {
        return htElementsM[id];
    }

private:

    // XML parsing methods
    static void ExpatStartElementHandler(void *pUserData,
                                         const XML_Char *pName,
                                         const XML_Char **pAttributes);
    static void ExpatEndElementHandler(void *pUserData,
                                       const XML_Char *pName);
    void StartElement(char *pElementName, char **pAttributes);
    void EndElement(char *pElementName);
    void XmlError(const char *pFormat, ...) const;
    u32 GetCurrentParserLineNumber() const;

    // XML parsing fields
    bool parseErrorM;
    XML_Parser xmlParserM;

    // The content which was parsed
    std::map<std::string, Element *> htElementsM;

    // The current stack of Elements
    std::vector<Element *> vCurrentElementsM;
};

}; // namespace Xrtti


#endif // PARSER_H
