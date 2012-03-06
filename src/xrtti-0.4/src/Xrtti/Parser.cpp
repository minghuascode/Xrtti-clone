/*****************************************************************************\
 *                                                                           *
 * Parser.cpp                                                                *
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

#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <private/Parser.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif

// --------------------------------------------------------------------------
// Parser::Parser(const Configuration &config, InputStream &instream)
// --------------------------------------------------------------------------
Parser::Parser()
    : parseErrorM(false), xmlParserM(NULL)
{
}


// --------------------------------------------------------------------------
// Parser::~Parser()
// --------------------------------------------------------------------------
Parser::~Parser()
{
	for (map<string, Element *>::iterator iter = htElementsM.
			 begin(); iter != htElementsM.end(); iter++) {
		delete iter->second;
	}
}


// --------------------------------------------------------------------------
// Parser::Parse(int fd)
// --------------------------------------------------------------------------
void Parser::Parse(int fd)
{
    // Set the expat parser
    xmlParserM = XML_ParserCreate("utf-8");
    XML_SetStartElementHandler(xmlParserM, ExpatStartElementHandler);
    XML_SetEndElementHandler(xmlParserM, ExpatEndElementHandler);
    XML_SetUserData(xmlParserM, this);
    
    while (true) {
        unsigned char inbuf[1024];
        u32 bytesRead = read(fd, inbuf, sizeof(inbuf));

        bool fReadSuccess = (bytesRead > 0) && (bytesRead < (u32) -1);
        
        if (!fReadSuccess) {
            if (!XML_Parse(xmlParserM, (const char *) inbuf, 0, true) || 
                parseErrorM) {
                this->XmlError("XML parse failed\n");
            }
            break;
        }

        if (!XML_Parse(xmlParserM, (const char *) inbuf, bytesRead, false) || 
            parseErrorM) {
            this->XmlError("XML parse failed\n");
        }
    }

    XML_ParserFree(xmlParserM);

    xmlParserM = NULL;
}


// --------------------------------------------------------------------------
// Parser::ExpatStartElementHandler(void *pData,
//                                  const XML_Char *pName,
//                                  const XML_Char **pAttributes)
// --------------------------------------------------------------------------
/* static */
void Parser::ExpatStartElementHandler(void *pData,
                                      const XML_Char *pName,
                                      const XML_Char **pAttributes)
{
    ((Parser *) pData)->StartElement((char *) pName, (char **) pAttributes);
}


// --------------------------------------------------------------------------
// Parser::ExpatEndElementHandler(void *pData, const XML_Char *pName)
// --------------------------------------------------------------------------
/* static */
void Parser::ExpatEndElementHandler(void *pData, const XML_Char *pName)
{
    ((Parser *) pData)->EndElement((char *) pName);
}


// --------------------------------------------------------------------------
// Parser::StartElement(char *pElementName, char **pAttributes)
// --------------------------------------------------------------------------
void Parser::StartElement(char *pElementName, char **pAttributes)
{
    // Ignore GCC-XML element
    if (!strcmp(pElementName, "GCC_XML")) {
        return;
    }

    // Create a new Element for this XML element
    Element *pElement = new Element(*this, pElementName, pAttributes);

    // See if this XML element is a child of another XML element, which
    // will already have been "pushed" on the current elements "stack"

    // If there is no super element, then this is a "root" element
    if (vCurrentElementsM.empty()) {
        // root elements are expected to have ids
        string id = pElement->GetAttributeValue("id");
        if (id == "") {
            this->XmlError("missing id");
        }
        htElementsM[id] = pElement;
    }
    // If there is a super element, then add this element to it
    else {
        vCurrentElementsM.back()->subElementsM.push_back(pElement);
    }
             
    // "Push" the current element on the element "stack"
    vCurrentElementsM.push_back(pElement);
}


// --------------------------------------------------------------------------
// Parser::EndElement(char *pElementName)
// --------------------------------------------------------------------------
void Parser::EndElement(char *pElementName)
{
    // Ignore GCC-XML element
    if (!strcmp(pElementName, "GCC_XML")) {
        return;
    }

    // "pop" the current element off of the element "stack"
    Element *pElement = vCurrentElementsM.back();

    // there had better have been one there
    if (pElement == NULL) {
        this->XmlError("Mismatched XML elements");
    }

    vCurrentElementsM.pop_back();
}


// --------------------------------------------------------------------------
// Parser::XmlError(const char *pFormat, ...) const
// --------------------------------------------------------------------------
void Parser::XmlError(const char *pFormat, ...) const
{
    va_list valist;

    va_start(valist, pFormat);

    fprintf(stderr, "Line %lu: ", 
            (unsigned long) this->GetCurrentParserLineNumber());

    fprintf(stderr, pFormat, valist);

    va_end(valist);

    fprintf(stderr, "\n");

    exit(-1);
}


// --------------------------------------------------------------------------
// Parser::GetCurrentParserLineNumber() const
// --------------------------------------------------------------------------
u32 Parser::GetCurrentParserLineNumber() const
{
    return (u32) XML_GetCurrentLineNumber(xmlParserM);
}


// --------------------------------------------------------------------------
// Parser::Attribute::Attribute(char *pName, char *pValue)
// --------------------------------------------------------------------------
Parser::Attribute::Attribute(char *pName, char *pValue)
    : nameM(pName), valueM(pValue)
{
}


// --------------------------------------------------------------------------
// Parser::Element::Element(const Parser &parser, char **pAttributes)
// --------------------------------------------------------------------------
Parser::Element::Element(const Parser &parser, char *pElementName,
                         char **pAttributes)
    : elementNameM(pElementName)
{
    // Get current line number
    lineNumberM = parser.GetCurrentParserLineNumber();
    // Create attributes
    while (*pAttributes) {
        char *pAttributeName = *pAttributes++;
        char *pAttributeValue = *pAttributes++;
        // Skip attributes with no value
        if (!pAttributeValue || !*pAttributeValue) {
            continue;
        }
        Parser::Attribute *pAttribute = 
            new Parser::Attribute(pAttributeName, pAttributeValue);
        attributesM.push_back(pAttribute);
    }
}


// --------------------------------------------------------------------------
// Parser::Element::~Element()
// --------------------------------------------------------------------------
Parser::Element::~Element()
{
	for (u32 i = 0; i < attributesM.size(); i++) {
		delete attributesM[i];
	}
	
	for (u32 i = 0; i < subElementsM.size(); i++) {
		delete subElementsM[i];
	}
}

}

