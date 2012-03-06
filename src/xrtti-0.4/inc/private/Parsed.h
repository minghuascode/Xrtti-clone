/*****************************************************************************\
 *                                                                           *
 * Parsed.h                                                                  *
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

#ifndef PARSED_H
#define PARSED_H

#include <map>
#include <string>
#include <vector>
#include <Xrtti/XrttiParsed.h>
#include <private/Parser.h>

// The Parsed* classes are an implementation of the Xrtti abstract base
// classes which can be created from parsed gccxml output.  The "Parser"
// helper class is used to parse the gccxml XML file, and the resulting
// elements and attributes are passed to the initializer methods of these
// Parsed classes to construct a full Xrtti representation of the gccxml
// output.
//
// Each Parsed class has a constructor (that does essentially nothing except
// initialize data mambers so that if the destructor is immediately called, it
// works correctly), an initialize method (which actually sets up the parsed
// object and any sub-objects), a destructor (which can destroy any parsed
// object, even if it failed its initialize method), and most have a
// "MergeContents" method, which is used to merge the object into a
// ParsedContextSet.  Aside from these methods, each parsed object implements
// methods to satisfy the abstract base class from which the object derives;
// methods "at the layer" of the parsed object are implemented using member
// variables of the parsed object, and methods "above the layer" of the parsed
// object are directed to an embedded "base class" parsed instance.
//
// When a new gccxml file is read into a ParsedContextSet, it is first read
// into a temporary empty ParsedContextSet, and then the results are "merged"
// (if possible - merging is not possible if there are conflicts between the
// newly read Xrtti objects and the existing ones) into the final
// ParsedContextSet.

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


class ParsedArgument;
class ParsedArray;
class ParsedBase;
class ParsedClass;
class ParsedConstructor;
class ParsedConstructorSignature;
class ParsedContext;
class ParsedContextSet;
class ParsedDestructor;
class ParsedDestructorSignature;
class ParsedEnumeration;
class ParsedEnumerationValue;
class ParsedField;
class ParsedImplementation;
class ParsedMethod;
class ParsedMethodSignature;
class ParsedNamespace;
class ParsedPointer;
class ParsedStructure;
class ParsedStruct;
class ParsedType;
class ParsedTypeEnumeration;
class ParsedTypeFunction;
class ParsedTypeStructure;
class ParsedTypeUnion;
class ParsedUnion;


// **********************************************************************
// Utility functions
// **********************************************************************
void split(const std::string &, std::vector<std::string> &);
std::string get_name(const Parser::Element *pElement, bool anonymous);
AccessType get_access_type(const Parser::Element *pElement);
bool get_member_elements(Parser &parser, const Parser::Element *pElement, 
                         std::vector<Parser::Element *> &vMembers,
                         std::string &error);

class ParsedContext : public Context
{
public:

    ParsedContext();

    virtual ~ParsedContext();

    bool Initialize(ParsedContextSet &set, Parser &parser, 
                    const Parser::Element *pElement, std::string &error);
    
    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    // Context methods --------------------------------------------------------
    
    virtual Type GetType() const
    {
        return (Type) 0xFFFFFFFF;
    }

    virtual const char *GetName() const
    {
        return nameM.c_str();
    }

    virtual const char *GetFullName() const
    {
        return fullNameM.c_str();
    }

    virtual const Context *GetContext() const
    {
        return pContextM;
    }

private:

    std::string nameM;

    std::string fullNameM;

    Context *pContextM;
};


class ParsedNamespace : public Namespace
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser, 
                    const Parser::Element *pElement, std::string &error)
    {
        return superM.Initialize(set, parser, pElement, error);
    }

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from)
    {
        superM.MergeContents(to, from);
    }

    // Context methods --------------------------------------------------------

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual const char *GetFullName() const
    {
        return superM.GetFullName();
    }

    virtual const Context *GetContext() const
    {
        return superM.GetContext();
    }

private:

    ParsedContext superM;
};


class ParsedStructure : public Structure
{
public:

    ParsedStructure();

    virtual ~ParsedStructure();

    bool Initialize(ParsedContextSet &set, Parser &parser, 
                    const Parser::Element *pElement, std::string &error);

    void AddFriend(Structure *pStructure)
    {
        vFriendsM.push_back(pStructure);
    }

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Context methods --------------------------------------------------------

    virtual Type GetType() const
    {
        return (Type) 0xFFFFFFFF;
    }

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual const char *GetFullName() const
    {
        return superM.GetFullName();
    }

    virtual const Context *GetContext() const
    {
        return superM.GetContext();
    }

    // Structure methods ------------------------------------------------------

    virtual bool IsIncomplete() const
    {
        return isIncompleteM;
    }
    
    virtual bool HasSizeof() const
    {
        return false;
    }

    virtual u32 GetSizeof() const
    {
        return 0;
    }

    virtual bool HasStructureName() const
    {
        return hasStructureNameM;
    }

    virtual AccessType GetAccessType() const
    {
        return accessTypeM;
    }

    virtual const std::type_info *GetTypeInfo() const
    {
        return pTypeInfoM;
    }

    virtual u32 GetBaseCount() const
    {
        return vBasesM.size();
    }

    virtual const Base &GetBase(u32 index) const;

    virtual u32 GetFriendCount() const
    {
        return vFriendsM.size();
    }

    virtual const Structure &GetFriend(u32 index) const;

    virtual u32 GetFieldCount() const
    {
        return vFieldsM.size();
    }

    virtual const Field &GetField(u32 index) const;

    virtual bool IsAnonymous() const
    {
        return isAnonymousM;
    }

    virtual u32 GetConstructorCount() const
    {
        return vConstructorsM.size();
    }

    virtual const Constructor &GetConstructor(u32 index) const;

    virtual bool HasDestructor() const
    {
        return pDestructorM;
    }

    virtual const Destructor &GetDestructor() const;

    virtual bool IsCreatable() const
    {
        return false;
    }

    virtual void *Create() const
    {
        return 0;
    }

    virtual void *CreateArray(u32 /* count */) const
    {
        return 0;
    }

    virtual bool IsDeletable() const
    {
        return false;
    }

    virtual void Delete(void * /* pInstance */) const
    {
    }

    virtual void DeleteArray(void * /* pInstanceArray */) const
    {
    }

private:

    ParsedContext superM;

    bool isIncompleteM;

    bool hasStructureNameM;

    AccessType accessTypeM;

    const std::type_info *pTypeInfoM;

    std::vector<ParsedBase> vBasesM;

    std::vector<Structure *> vFriendsM;

    std::vector<ParsedField> vFieldsM;

    bool isAnonymousM;

    std::vector<ParsedConstructor> vConstructorsM;

    ParsedDestructor *pDestructorM;
};


class ParsedBase : public Base
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser, 
                    const std::string &id, AccessType accessType, 
                    bool isVirtual, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    // Base methods -----------------------------------------------------------

    virtual AccessType GetAccessType() const
    {
        return accessTypeM;
    }

    virtual bool IsVirtual() const
    {
        return isVirtualM;
    }

    virtual const Structure &GetStructure() const
    {
        return *pStructureM;
    }
    
    virtual bool IsCastable() const
    {
        return false;
    }

    virtual void *CastSubclass(void * /* pObject */) const
    {
        return 0;
    }

private:

    Structure *pStructureM;

    AccessType accessTypeM;

    bool isVirtualM;
};


class ParsedUnion : public Union
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser, 
                    const Parser::Element *pElement, std::string &error)
    {
        return superM.Initialize(set, parser, pElement, error);
    }
    
    void MergeContents(ParsedContextSet &to, ParsedContextSet &from)
    {
        superM.MergeContents(to, from);
    }
    
    void ReplaceContext(Context *pOld, Context *pNew)
    {
        superM.ReplaceContext(pOld, pNew);
    }

    // Context methods --------------------------------------------------------

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual const char *GetFullName() const
    {
        return superM.GetFullName();
    }

    virtual const Context *GetContext() const
    {
        return superM.GetContext();
    }

    // Structure methods ------------------------------------------------------

    virtual bool IsIncomplete() const
    {
        return superM.IsIncomplete();
    }

    virtual bool HasSizeof() const
    {
        return superM.HasSizeof();
    }

    virtual u32 GetSizeof() const
    {
        return superM.GetSizeof();
    }

    virtual bool HasStructureName() const
    {
        return superM.HasStructureName();
    }

    virtual AccessType GetAccessType() const
    {
        return superM.GetAccessType();
    }

    virtual const std::type_info *GetTypeInfo() const
    {
        return superM.GetTypeInfo();
    }

    virtual u32 GetBaseCount() const
    {
        return superM.GetBaseCount();
    }

    virtual const Base &GetBase(u32 index) const
    {
        return superM.GetBase(index);
    }

    virtual u32 GetFriendCount() const
    {
        return superM.GetFriendCount();
    }

    virtual const Structure &GetFriend(u32 index) const
    {
        return superM.GetFriend(index);
    }

    virtual u32 GetFieldCount() const
    {
        return superM.GetFieldCount();
    }

    virtual const Field &GetField(u32 index) const
    {
        return superM.GetField(index);
    }

    virtual bool IsAnonymous() const
    {
        return superM.IsAnonymous();
    }

    virtual u32 GetConstructorCount() const
    {
        return superM.GetConstructorCount();
    }

    virtual const Constructor &GetConstructor(u32 index) const
    {
        return superM.GetConstructor(index);
    }

    virtual bool HasDestructor() const
    {
        return superM.HasDestructor();
    }

    virtual const Destructor &GetDestructor() const
    {
        return superM.GetDestructor();
    }

    virtual bool IsCreatable() const
    {
        return superM.IsCreatable();
    }

    virtual void *Create() const
    {
        return superM.Create();
    }

    virtual void *CreateArray(u32 count) const
    {
        return superM.CreateArray(count);
    }

    virtual bool IsDeletable() const
    {
        return superM.IsDeletable();
    }

    virtual void Delete(void *pInstance) const
    {
        return superM.Delete(pInstance);
    }

    virtual void DeleteArray(void *pInstanceArray) const
    {
        return superM.DeleteArray(pInstanceArray);
    }

private:

    ParsedStructure superM;
};


class ParsedStruct : public Struct
{
public:

    ParsedStruct();

    virtual ~ParsedStruct();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    const Parser::Element *pElement, std::string &error);

    void AddFriend(Structure *pFriend)
    {
        superM.AddFriend(pFriend);
    }

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Context methods --------------------------------------------------------

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual const char *GetFullName() const
    {
        return superM.GetFullName();
    }

    virtual const Context *GetContext() const
    {
        return superM.GetContext();
    }

    // Structure methods ------------------------------------------------------

    virtual bool IsIncomplete() const
    {
        return superM.IsIncomplete();
    }

    virtual bool HasSizeof() const
    {
        return superM.HasSizeof();
    }

    virtual u32 GetSizeof() const
    {
        return superM.GetSizeof();
    }

    virtual bool HasStructureName() const
    {
        return superM.HasStructureName();
    }

    virtual AccessType GetAccessType() const
    {
        return superM.GetAccessType();
    }

    virtual const std::type_info *GetTypeInfo() const
    {
        return superM.GetTypeInfo();
    }

    virtual u32 GetBaseCount() const
    {
        return superM.GetBaseCount();
    }

    virtual const Base &GetBase(u32 index) const
    {
        return superM.GetBase(index);
    }

    virtual u32 GetFriendCount() const
    {
        return superM.GetFriendCount();
    }

    virtual const Structure &GetFriend(u32 index) const
    {
        return superM.GetFriend(index);
    }

    virtual u32 GetFieldCount() const
    {
        return superM.GetFieldCount();
    }

    virtual const Field &GetField(u32 index) const
    {
        return superM.GetField(index);
    }

    virtual bool IsAnonymous() const
    {
        return superM.IsAnonymous();
    }

    virtual u32 GetConstructorCount() const
    {
        return superM.GetConstructorCount();
    }

    virtual const Constructor &GetConstructor(u32 index) const
    {
        return superM.GetConstructor(index);
    }

    virtual bool HasDestructor() const
    {
        return superM.HasDestructor();
    }

    virtual const Destructor &GetDestructor() const
    {
        return superM.GetDestructor();
    }

    virtual bool IsCreatable() const
    {
        return superM.IsCreatable();
    }

    virtual void *Create() const
    {
        return superM.Create();
    }

    virtual void *CreateArray(u32 count) const
    {
        return superM.CreateArray(count);
    }

    virtual bool IsDeletable() const
    {
        return superM.IsDeletable();
    }

    virtual void Delete(void *pInstance) const
    {
        return superM.Delete(pInstance);
    }

    virtual void DeleteArray(void *pInstanceArray) const
    {
        return superM.DeleteArray(pInstanceArray);
    }

    // Struct methods ---------------------------------------------------------

    virtual bool IsAbstract() const
    {
        return isAbstractM;
    }

    virtual u32 GetMethodCount() const
    {
        return vMethodsM.size();
    }

    virtual const Method &GetMethod(u32 index) const;

private:

    ParsedStructure superM;

    bool isAbstractM;

    std::vector<ParsedMethod> vMethodsM;
};


class ParsedClass : public Class
{
public:
    
    bool Initialize(ParsedContextSet &set, Parser &parser,
                    const Parser::Element *pElement, std::string &error)
    {
        return superM.Initialize(set, parser, pElement, error);
    }

    void AddFriend(Structure *pFriend)
    {
        superM.AddFriend(pFriend);
    }

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from)
    {
        superM.MergeContents(to, from);
    }

    void ReplaceContext(Context *pOld, Context *pNew)
    {
        superM.ReplaceContext(pOld, pNew);
    }

    // Context methods --------------------------------------------------------

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual const char *GetFullName() const
    {
        return superM.GetFullName();
    }

    virtual const Context *GetContext() const
    {
        return superM.GetContext();
    }

    // Structure methods ------------------------------------------------------

    virtual bool IsIncomplete() const
    {
        return superM.IsIncomplete();
    }

    virtual bool HasSizeof() const
    {
        return superM.HasSizeof();
    }

    virtual u32 GetSizeof() const
    {
        return superM.GetSizeof();
    }

    virtual bool HasStructureName() const
    {
        return superM.HasStructureName();
    }

    virtual AccessType GetAccessType() const
    {
        return superM.GetAccessType();
    }

    virtual const std::type_info *GetTypeInfo() const
    {
        return superM.GetTypeInfo();
    }

    virtual u32 GetBaseCount() const
    {
        return superM.GetBaseCount();
    }

    virtual const Base &GetBase(u32 index) const
    {
        return superM.GetBase(index);
    }

    virtual u32 GetFriendCount() const
    {
        return superM.GetFriendCount();
    }

    virtual const Structure &GetFriend(u32 index) const
    {
        return superM.GetFriend(index);
    }

    virtual u32 GetFieldCount() const
    {
        return superM.GetFieldCount();
    }

    virtual const Field &GetField(u32 index) const
    {
        return superM.GetField(index);
    }

    virtual bool IsAnonymous() const
    {
        return superM.IsAnonymous();
    }

    virtual u32 GetConstructorCount() const
    {
        return superM.GetConstructorCount();
    }

    virtual const Constructor &GetConstructor(u32 index) const
    {
        return superM.GetConstructor(index);
    }

    virtual bool HasDestructor() const
    {
        return superM.HasDestructor();
    }

    virtual const Destructor &GetDestructor() const
    {
        return superM.GetDestructor();
    }

    virtual bool IsCreatable() const
    {
        return superM.IsCreatable();
    }

    virtual void *Create() const
    {
        return superM.Create();
    }

    virtual void *CreateArray(u32 count) const
    {
        return superM.CreateArray(count);
    }

    virtual bool IsDeletable() const
    {
        return superM.IsDeletable();
    }

    virtual void Delete(void *pInstance) const
    {
        return superM.Delete(pInstance);
    }

    virtual void DeleteArray(void *pInstanceArray) const
    {
        return superM.DeleteArray(pInstanceArray);
    }

    // Struct methods ---------------------------------------------------------

    virtual bool IsAbstract() const
    {
        return superM.IsAbstract();
    }

    virtual u32 GetMethodCount() const
    {
        return superM.GetMethodCount();
    }

    virtual const Method &GetMethod(u32 index) const
    {
        return superM.GetMethod(index);
    }

private:

    ParsedStruct superM;
};


class ParsedMember : public Member
{
public:

    ParsedMember();

    virtual ~ParsedMember();

    void Initialize(ParsedContextSet &set, Context *pContext,
                    bool isStatic, Parser::Element *pElement);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    // Member methods ---------------------------------------------------------

    virtual AccessType GetAccessType() const
    {
        return accessTypeM;
    }

    virtual const Context &GetContext() const
    {
        return *pContextM;
    }

    virtual const char *GetName() const
    {
        return nameM.c_str();
    }

    virtual bool IsStatic() const
    {
        return isStaticM;
    }

private:

    AccessType accessTypeM;

    Context *pContextM;

    std::string nameM;

    bool isStaticM;
};


class ParsedField : public Field
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Context *pContext, bool isStatic,
                    Parser::Element *pElement, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Member methods ---------------------------------------------------------

    virtual AccessType GetAccessType() const
    {
        return superM.GetAccessType();
    }

    virtual const Context &GetContext() const
    {
        return superM.GetContext();
    }

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual bool IsStatic() const
    {
        return superM.IsStatic();
    }

    // Field methods ----------------------------------------------------------

    virtual const Type &GetType() const;

    virtual u32 GetBitfieldBitCount() const
    {
        return bitCountM;
    }

    virtual bool HasOffset() const
    {
        return false;
    }

    virtual u32 GetOffset() const
    {
        return 0;
    }

    virtual bool IsAccessible() const
    {
        return false;
    }

    virtual void *Get(void * /* pInstance */) const
    {
        return 0;
    }

private:

    ParsedMember superM;

    u32 bitCountM;

    Type *pTypeM;
};


class ParsedArgument : public Argument
{
public:

    ParsedArgument();

    virtual ~ParsedArgument();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Parser::Element *pElement, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Argument methods -------------------------------------------------------

    virtual const Type &GetType() const
    {
        return *pTypeM;
    }

    virtual bool HasDefault() const
    {
        return hasDefaultM;
    }

    virtual const char *GetDefault() const
    {
        return defaultM.c_str();
    }

private:

    Type *pTypeM;

    bool hasDefaultM;

    std::string defaultM;
};


class ParsedDestructorSignature : public DestructorSignature
{
public:

    ParsedDestructorSignature();

    virtual ~ParsedDestructorSignature();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Parser::Element *pElement, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    // DestructorSignature methods --------------------------------------------

    virtual u32 GetThrowCount() const
    {
        return vThrowsM.size();
    }

    virtual const Type &GetThrow(u32 index) const
    {
        return *(vThrowsM[index]);
    }

private:

    std::vector<Type *> vThrowsM;
};


class ParsedConstructorSignature : public ConstructorSignature
{
public:

    ParsedConstructorSignature();

    virtual ~ParsedConstructorSignature();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Parser::Element *pElement, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // DestructorSignature methods -------------------------------------------

    virtual u32 GetThrowCount() const
    {
        return superM.GetThrowCount();
    }

    virtual const Type &GetThrow(u32 index) const
    {
        return superM.GetThrow(index);
    }

    // ConstructorSignature methods -------------------------------------------

    virtual u32 GetArgumentCount() const
    {
        return vArgumentsM.size();
    }

    virtual const Argument &GetArgument(u32 index) const
    {
        return vArgumentsM[index];
    }

    virtual bool HasEllipsis() const
    {
        return hasEllipsisM;
    }

private:

    ParsedDestructorSignature superM;

    std::vector<ParsedArgument> vArgumentsM;

    bool hasEllipsisM;
};


class ParsedMethodSignature : public MethodSignature
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Parser::Element *pElement, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // ConstructorSignature methods ------------------------------------------

    virtual u32 GetArgumentCount() const
    {
        return superM.GetArgumentCount();
    }

    virtual const Argument &GetArgument(u32 index) const
    {
        return superM.GetArgument(index);
    }

    virtual bool HasEllipsis() const
    {
        return superM.HasEllipsis();
    }

    // DestructorSignature methods -------------------------------------------

    virtual u32 GetThrowCount() const
    {
        return superM.GetThrowCount();
    }

    virtual const Type &GetThrow(u32 index) const
    {
        return superM.GetThrow(index);
    }

    // MethodSignature methods ------------------------------------------------

    virtual const Type &GetReturnType() const
    {
        return *pReturnTypeM;
    }

private:

    ParsedConstructorSignature superM;

    Type *pReturnTypeM;
};


class ParsedDestructor : public Destructor
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Context *pContext, Parser::Element *pElement,
                    std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    // Member methods ---------------------------------------------------------

    virtual AccessType GetAccessType() const
    {
        return superM.GetAccessType();
    }

    virtual const Context &GetContext() const
    {
        return superM.GetContext();
    }

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual bool IsStatic() const
    {
        return superM.IsStatic();
    }

    // Destructor methods -----------------------------------------------------

    virtual bool IsVirtual() const
    {
        return isVirtualM;
    }

    virtual bool IsPureVirtual() const
    {
        return isPureVirtualM;
    }

    virtual const DestructorSignature &GetSignature() const
    {
        return signatureM;
    }

    virtual bool IsInvokeable() const
    {
        return false;
    }

    virtual void Invoke(void * /* pInstance */) const
    {
    }

private:

    ParsedMember superM;

    bool isVirtualM, isPureVirtualM;

    ParsedDestructorSignature signatureM;
};


class ParsedConstructor : public Constructor
{
public:

    ParsedConstructor();

    virtual ~ParsedConstructor();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Context *pContext, Parser::Element *pElement,
                    std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Member methods ---------------------------------------------------------

    virtual AccessType GetAccessType() const
    {
        return superM.GetAccessType();
    }

    virtual const Context &GetContext() const
    {
        return superM.GetContext();
    }

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual bool IsStatic() const
    {
        return superM.IsStatic();
    }

    // Constructor methods ----------------------------------------------------

    virtual const ConstructorSignature &GetSignature() const
    {
        return signatureM;
    }

    virtual const char *GetArgumentName(u32 index) const
    {
        return argumentNamesM[index].c_str();
    }

    virtual bool IsInvokeable() const
    {
        return false;
    }

    virtual void *Invoke(void ** /* pArgumentValues */) const
    {
        return 0;
    }

private:

    ParsedMember superM;

    ParsedConstructorSignature signatureM;

    std::vector<std::string> argumentNamesM;
};


class ParsedMethod : public Method
{
public:

    ParsedMethod();

    virtual ~ParsedMethod();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Context *pContext, Parser::Element *pElement,
                    bool isOperator, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Member methods ---------------------------------------------------------

    virtual AccessType GetAccessType() const
    {
        return superM.GetAccessType();
    }

    virtual const Context &GetContext() const
    {
        return superM.GetContext();
    }

    virtual const char *GetName() const
    {
        return superM.GetName();
    }

    virtual bool IsStatic() const
    {
        return superM.IsStatic();
    }

    // Method methods ---------------------------------------------------------

    virtual bool IsOperatorMethod() const
    {
        return isOperatorMethodM;
    }

    virtual bool IsConst() const
    {
        return isConstM;
    }

    virtual bool IsVirtual() const
    {
        return isVirtualM;
    }

    virtual bool IsPureVirtual() const
    {
        return isPureVirtualM;
    }

    virtual const MethodSignature &GetSignature() const
    {
        return signatureM;
    }

    virtual const char *GetArgumentName(u32 index) const
    {
        return vArgumentNamesM[index].c_str();
    }

    virtual bool IsInvokeable() const
    {
        return false;
    }

    virtual void Invoke(void * /* pInstance */, void * /* pReturnValue */, 
                        void ** /* pArgumentValues */) const
    {
    }

private:

    ParsedMember superM;

    bool isOperatorMethodM, isConstM, isVirtualM, isPureVirtualM;

    ParsedMethodSignature signatureM;

    std::vector<std::string> vArgumentNamesM;
};


class ParsedType : public Type
{
public:

    ParsedType();

    virtual ~ParsedType();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Type::BaseType baseType, Parser::Element *pElement,
                    std::string &error);

    // This is called to initialize the type if it's really a TypeStructure
    // which was initialized with a context; this is a type that gccxml didn't
    // declare but that it should have
    void Initialize()
    {
        baseTypeM = BaseType_Structure;

        isConstM = isVolatileM = isReferenceM = false;
    }

    // Type methods -----------------------------------------------------------

    virtual BaseType GetBaseType() const
    {
        return baseTypeM;
    }

    virtual bool IsConst() const
    {
        return isConstM;
    }

    virtual bool IsVolatile() const
    {
        return isVolatileM;
    }

    virtual bool IsReference() const
    {
        return isReferenceM;
    }

    virtual u32 GetArrayOrPointerCount() const
    {
        return vArrayOrPointersM.size();
    }

    virtual const ArrayOrPointer &GetArrayOrPointer(u32 index) const
    {
        return *(vArrayOrPointersM[index]);
    }

private:

    BaseType baseTypeM;

    bool isConstM;

    bool isVolatileM;

    bool isReferenceM;

    std::vector<ArrayOrPointer *> vArrayOrPointersM;
};


class ParsedArray : public Array
{
public:

    ParsedArray(bool isUnbounded, u32 elementCount)
        : isUnboundedM(isUnbounded), elementCountM(elementCount)
    {
    }

    // Array methods ----------------------------------------------------------

    virtual bool IsUnbounded() const
    {
        return isUnboundedM;
    }

    virtual u32 GetElementCount() const 
    {
        return elementCountM;
    }

private:

    bool isUnboundedM;

    u32 elementCountM;
};


class ParsedEnumerationValue : public EnumerationValue
{
public:

    ParsedEnumerationValue();

    virtual ~ParsedEnumerationValue();

    void Initialize(ParsedContextSet &set, const std::string &name, s32 value);

    // EnumerationValue methods -----------------------------------------------

    virtual const char *GetName() const
    {
        return nameM.c_str();
    }

    virtual s32 GetValue() const
    {
        return valueM;
    }

private:

    std::string nameM;

    s32 valueM;
};


class ParsedPointer : public Pointer
{
public:

    ParsedPointer(bool isConst, bool isVolatile)
        : isConstM(isConst), isVolatileM(isVolatile)
    {
    }

    // Pointer methods --------------------------------------------------------

    virtual bool IsConst() const
    {
        return isConstM;
    }

    virtual bool IsVolatile() const
    {
        return isVolatileM;
    }

private:

    bool isConstM;

    bool isVolatileM;
};


class ParsedEnumeration : public Enumeration
{
public:

    ParsedEnumeration();

    virtual ~ParsedEnumeration();

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Parser::Element *pElement, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    // Enumeration methods ----------------------------------------------------

    virtual AccessType GetAccessType() const
    {
        return accessTypeM;
    }

    virtual const Context &GetContext() const
    {
        return *pContextM;
    };

    virtual const char *GetName() const
    {
        return nameM.c_str();
    }
    
    virtual u32 GetValueCount() const
    {
        return valuesM.size();
    }

    virtual const EnumerationValue &GetValue(u32 index) const
    {
        return valuesM[index];
    }

private:

    AccessType accessTypeM;

    Context *pContextM;

    std::string nameM;

    std::vector<ParsedEnumerationValue> valuesM;
};


class ParsedTypeEnumeration : public TypeEnumeration
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser, 
                    Parser::Element *pElementBase, Parser::Element *pElement,
                    std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    // Type methods -----------------------------------------------------------

    virtual BaseType GetBaseType() const
    {
        return superM.GetBaseType();
    }

    virtual bool IsConst() const
    {
        return superM.IsConst();
    }

    virtual bool IsVolatile() const
    {
        return superM.IsVolatile();
    }

    virtual bool IsReference() const
    {
        return superM.IsReference();
    }

    virtual u32 GetArrayOrPointerCount() const
    {
        return superM.GetArrayOrPointerCount();
    }

    virtual const ArrayOrPointer &GetArrayOrPointer(u32 index) const
    {
        return superM.GetArrayOrPointer(index);
    }

    // TypeEnumeration methods ------------------------------------------------

    virtual const Enumeration &GetEnumeration() const
    {
        return *pEnumerationM;
    }

private:

    ParsedType superM;

    Enumeration *pEnumerationM;
};


class ParsedTypeFunction : public TypeFunction
{
public:

    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Parser::Element *pElementBase, Parser::Element *pElement,
                    std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Type methods -----------------------------------------------------------

    virtual BaseType GetBaseType() const
    {
        return superM.GetBaseType();
    }

    virtual bool IsConst() const
    {
        return superM.IsConst();
    }

    virtual bool IsVolatile() const
    {
        return superM.IsVolatile();
    }

    virtual bool IsReference() const
    {
        return superM.IsReference();
    }

    virtual u32 GetArrayOrPointerCount() const
    {
        return superM.GetArrayOrPointerCount();
    }

    virtual const ArrayOrPointer &GetArrayOrPointer(u32 index) const
    {
        return superM.GetArrayOrPointer(index);
    }

    // TypeFunction methods ---------------------------------------------------

    virtual const MethodSignature &GetSignature() const
    {
        return signatureM;
    }

private:

    ParsedType superM;

    ParsedMethodSignature signatureM;
};


class ParsedTypeStructure : public TypeStructure
{
public:
    
    bool Initialize(ParsedContextSet &set, Parser &parser,
                    Parser::Element *pElementBase, Parser::Element *pElement,
                    std::string &error);

    bool Initialize(Context *pContext, std::string &error);

    void MergeContents(ParsedContextSet &to, ParsedContextSet &from);

    void ReplaceContext(Context *pOld, Context *pNew);

    // Type methods -----------------------------------------------------------

    virtual BaseType GetBaseType() const
    {
        return superM.GetBaseType();
    }

    virtual bool IsConst() const
    {
        return superM.IsConst();
    }

    virtual bool IsVolatile() const
    {
        return superM.IsVolatile();
    }

    virtual bool IsReference() const
    {
        return superM.IsReference();
    }

    virtual u32 GetArrayOrPointerCount() const
    {
        return superM.GetArrayOrPointerCount();
    }

    virtual const ArrayOrPointer &GetArrayOrPointer(u32 index) const
    {
        return superM.GetArrayOrPointer(index);
    }

    // TypeStructure methods --------------------------------------------------

    virtual const Structure &GetStructure() const
    {
        return *pStructureM;
    }

private:

    ParsedType superM;

    Structure *pStructureM;
};


class ParsedContextSet : public ContextSet
{
public:

    ParsedContextSet();

    virtual ~ParsedContextSet();

    // ContextSet methods ParsedContex----------------------------------------

    virtual bool AddHeader(const char *file, u32 includeCount,
                           const char **pIncludes, u32 definitionCount,
                           const char **pDefinitions, const char *tmppath);

    virtual const char *GetLastError() const
    {
        return errorM.c_str();
    }

    virtual u32 GetContextCount() const;
    
    virtual const Context *GetContext(u32 index) const;
    
    virtual const Context *LookupContext(const char *pFullName) const;

    // ParsedContextSet methods ----------------------------------------------

    // Get a context; creates it if necessary, and returns it as a completely
    // initialize context (or returns 0 and sets error to an error string
    // if there was an error initializing the context)
    Context *GetContext(Parser &parser, const std::string &id, 
                        std::string &error);
    
    // Get a type; creates it if necessary, and returns it as a completely
    // initialize context (or returns 0 and sets error to an error string
    // if there was an error initializing the type)
    Type *GetType(Parser &parser, const std::string &id, std::string &error);

    // Get a type; creates it if necessary, and returns it as a completely
    // initialize context (or returns 0 and sets error to an error string
    // if there was an error initializing the type).  This is a special type
    // which is fabricated (not having an input gccxml representation) as a
    // TypeStructure with the given context (which must be a Structure).
    Type *GetType(Context *pContext, std::string &error);

    // Get an enumeration; creates it if necessary, and returns it as a
    // completely initialize context (or returns 0 and sets error to an error
    // string if there was an error initializing the enumeration)
    Enumeration *GetEnumeration(Parser &parser, const std::string &id,
                                std::string &error);

    // Returns a merged version of the given context.  This may be the exact
    // same object if the object was already merged.
    Context *MergeContext(Context *pContext, ParsedContextSet &from);

    // Returns a merged version of the given type.  This may be the exact
    // same object if the object was already merged.
    Type *MergeType(Type *pType, ParsedContextSet &from);

    // Returns a merged version of the given enumeration.  This may be the
    // exact same object if the object was already merged.
    Enumeration *MergeEnumeration(Enumeration *pEnumeration, 
                                  ParsedContextSet &from);

    // Replaces all occurrences of pOld with pNew in the entire ContextSet
    void ReplaceContext(Context *pOld, Context *pNew);

private:

    bool AddHeaderToEmptySet(const char *file, u32 includeCount,
                             const char **pIncludes, u32 definitionCount,
                             const char **pDefinitions, const char *tmpfile,
                             std::string &error);

    bool CanMerge(ParsedContextSet &set, std::string &error);

    void Merge(ParsedContextSet &set);

    std::vector<Context *> vContextsM;

    std::map<std::string, Context *> htNonAnonymousContextsByNameM;

    std::map<std::string, Context *> htContextsByIdM;

    std::map<std::string, Type *> htTypesByIdM;

    std::map<std::string, Type *> htExtraTypesByNameM;

    std::vector<Type *> vTypesM;

    std::map<std::string, Enumeration *> htEnumerationsByIdM;

    std::vector<Enumeration *> vEnumerationsM;

    std::string errorM;
};


}; // namespace Xrtti

#endif // PARSED_H
