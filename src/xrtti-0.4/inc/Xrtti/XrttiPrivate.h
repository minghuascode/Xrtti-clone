/*****************************************************************************\
 *                                                                           *
 * XrttiPrivate.h                                                            *
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
 * These are private data structure definitions shared between               *
 * xrttigen-generated code and the xrtti library.                            *
 *                                                                           *
 * THIS HEADER FILE IS NOT MEANT TO BE INSPECTED OR USED BY ANY END-USER OF  *
 * THE XRTTI LIBRARY.  IGNORE THE CONTENTS OF THIS FILE.                     *
 *                                                                           *
\*****************************************************************************/

#ifndef XRTTI_PRIVATE_H
#define XRTTI_PRIVATE_H

#include <Xrtti/Xrtti.h>


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif

typedef void *(CompiledCreate)();
typedef void *(CompiledCreateArray)(u32);
typedef void (CompiledDelete)(void *);
typedef void (CompiledDeleteArray)(void *);
typedef void *(CompiledGet)(void *);
typedef void (CompiledDestructorInvoke)(void *);
typedef void *(CompiledConstructorInvoke)(void **);
typedef void (CompiledMethodInvoke)(void *, void *, void **);
typedef void *(CompiledCast)(void *);

class CompiledArgument;
class CompiledArray;
class CompiledBase;
class CompiledClass;
class CompiledConstructor;
class CompiledConstructorSignature;
class CompiledContext;
class CompiledContextSet;
class CompiledDestructor;
class CompiledDestructorSignature;
class CompiledEnumeration;
class CompiledEnumerationValue;
class CompiledField;
class CompiledImplementation;
class CompiledMethod;
class CompiledMethodSignature;
class CompiledNamespace;
class CompiledPointer;
class CompiledStructure;
class CompiledStruct;
class CompiledType;
class CompiledTypeEnumeration;
class CompiledTypeFunction;
class CompiledTypeStructure;
class CompiledTypeUnion;
class CompiledUnion;


class CompiledContext : public Context
{
public:

    CompiledContext(const char *pName, const char *pFullName, 
                    const Context *pContext)
        : pNameM(pName), pFullNameM(pFullName), pContextM(pContext)
    {
    }

    // Context methods --------------------------------------------------------
    
    virtual Type GetType() const
    {
        return (Type) 0xFFFFFFFF;
    }

    virtual const char *GetName() const
    {
        return pNameM;
    }

    virtual const char *GetFullName() const
    {
        return pFullNameM;
    }

    virtual const Context *GetContext() const
    {
        return pContextM;
    }

private:

    const char *pNameM;

    const char *pFullNameM;

    const Context *pContextM;
};


class CompiledNamespace : public Namespace
{
public:

    CompiledNamespace(const char *pName, const char *pFullName, 
                      const Context *pContext)
        : superM(pName, pFullName, pContext)
    {
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

    CompiledContext superM;
};

class CompiledStructure : public Structure
{
public:

    CompiledStructure(const char *pName, const char *pFullName, 
                      const Context *pContext, bool isIncomplete,
                      bool hasSizeof, u32 size, bool hasStructureName,
                      AccessType accessType,
                      const std::type_info *pTypeInfo,
                      u32 baseCount, Base **pBases,
                      u32 friendCount, Structure **pFriends,
                      u32 fieldCount, Field **pFields, 
                      bool isAnonymous, u32 constructorCount,
                      Constructor **pConstructors,
                      Destructor *pDestructor,
                      CompiledCreate *pCreate, 
                      CompiledCreateArray *pCreateArray,
                      CompiledDelete *pDelete,
                      CompiledDeleteArray *pDeleteArray)
        : superM(pName, pFullName, pContext), isIncompleteM(isIncomplete),
          hasSizeofM(hasSizeof), sizeofM(size), 
          hasStructureNameM(hasStructureName), accessTypeM(accessType),
          pTypeInfoM(pTypeInfo), baseCountM(baseCount), pBasesM(pBases),
          friendCountM(friendCount), pFriendsM(pFriends), 
          fieldCountM(fieldCount), pFieldsM(pFields),
          isAnonymousM(isAnonymous), constructorCountM(constructorCount),
          pConstructorsM(pConstructors), pDestructorM(pDestructor), 
          pCreateM(pCreate), pCreateArrayM(pCreateArray),
          pDeleteM(pDelete), pDeleteArrayM(pDeleteArray)
    {
    }

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
        return hasSizeofM;
    }
    
    virtual u32 GetSizeof() const
    {
        return sizeofM;
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
        return baseCountM;
    }

    virtual const Base &GetBase(u32 index) const;

    virtual u32 GetFriendCount() const
    {
        return friendCountM;
    }

    virtual const Structure &GetFriend(u32 index) const;

    virtual u32 GetFieldCount() const
    {
        return fieldCountM;
    }

    virtual const Field &GetField(u32 index) const;

    virtual bool IsAnonymous() const
    {
        return isAnonymousM;
    }

    virtual u32 GetConstructorCount() const
    {
        return constructorCountM;
    }

    virtual const Constructor &GetConstructor(u32 index) const;

    virtual bool HasDestructor() const
    {
        return pDestructorM;
    }

    virtual const Destructor &GetDestructor() const;

    virtual bool IsCreatable() const
    {
        return pCreateM;
    }

    virtual void *Create() const
    {
        return (*pCreateM)();
    }

    virtual void *CreateArray(u32 count) const
    {
        return (*pCreateArrayM)(count);
    }

    virtual bool IsDeletable() const
    {
        return pDeleteM;
    }

    virtual void Delete(void *pInstance) const
    {
        (*pDeleteM)(pInstance);
    }

    virtual void DeleteArray(void *pInstanceArray) const
    {
        (*pDeleteArrayM)(pInstanceArray);
    }

private:

    CompiledContext superM;

    bool isIncompleteM;

    bool hasSizeofM;

    u32 sizeofM;

    bool hasStructureNameM;

    AccessType accessTypeM;

    const std::type_info *pTypeInfoM;

    u32 baseCountM;
    
    Base **pBasesM;
    
    u32 friendCountM;

    Structure **pFriendsM;

    u32 fieldCountM;
    
    Field **pFieldsM;

    bool isAnonymousM;

    u32 constructorCountM;

    Constructor **pConstructorsM;

    Destructor *pDestructorM;

    void *(*pCreateM)();

    void *(*pCreateArrayM)(u32);

    void (*pDeleteM)(void *);

    void (*pDeleteArrayM)(void *);
};


class CompiledBase : public Base
{
public:

    CompiledBase(AccessType accessType, bool isVirtual, 
                 Structure *pStructure, CompiledCast *pCompiledCast)
        : accessTypeM(accessType), isVirtualM(isVirtual), 
          pStructureM(pStructure), pCompiledCastM(pCompiledCast)
    {
    }

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
        return pCompiledCastM;
    }

    virtual void *CastSubclass(void *pObject) const
    {
        return (*pCompiledCastM)(pObject);
    }

private:

    AccessType accessTypeM;

    bool isVirtualM;

    Structure *pStructureM;

    CompiledCast *pCompiledCastM;
};


class CompiledUnion : public Union
{
public:

    CompiledUnion(const char *pName, const char *pFullName, 
                  const Context *pContext, bool isIncomplete,
                  bool hasSizeof, u32 size, bool hasStructureName,
                  AccessType accessType,
                  const std::type_info *pTypeInfo,
                  u32 baseCount, Base **pBases,
                  u32 friendCount, Structure **pFriends,
                  u32 fieldCount, Field **pFields, 
                  bool isAnonymous, u32 constructorCount,
                  Constructor **pConstructors,
                  Destructor *pDestructor,
                  CompiledCreate *pCreate, 
                  CompiledCreateArray *pCreateArray,
                  CompiledDelete *pDelete,
                  CompiledDeleteArray *pDeleteArray)
        : superM(pName, pFullName, pContext, isIncomplete, hasSizeof, size, 
                 hasStructureName, accessType, pTypeInfo, baseCount, pBases,
                 friendCount, pFriends, fieldCount, pFields, isAnonymous,
                 constructorCount, pConstructors, pDestructor, pCreate,
                 pCreateArray, pDelete, pDeleteArray)
    {
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

    CompiledStructure superM;
};


class CompiledStruct : public Struct
{
public:

    CompiledStruct(const char *pName, const char *pFullName, 
                   const Context *pContext, bool isIncomplete,
                   bool hasSizeof, u32 size, bool hasStructureName,
                   AccessType accessType,
                   const std::type_info *pTypeInfo,
                   u32 baseCount, Base **pBases,
                   u32 friendCount, Structure **pFriends,
                   u32 fieldCount, Field **pFields, 
                   bool isAnonymous, u32 constructorCount,
                   Constructor **pConstructors,
                   Destructor *pDestructor,
                   CompiledCreate *pCreate, 
                   CompiledCreateArray *pCreateArray,
                   CompiledDelete *pDelete,
                   CompiledDeleteArray *pDeleteArray, bool isAbstract,
                   u32 methodCount, Method **pMethods)
        : superM(pName, pFullName, pContext, isIncomplete, hasSizeof, size, 
                 hasStructureName, accessType, pTypeInfo, baseCount, pBases,
                 friendCount, pFriends, fieldCount, pFields, isAnonymous,
                 constructorCount, pConstructors, pDestructor, pCreate,
                 pCreateArray, pDelete, pDeleteArray),
          isAbstractM(isAbstract), methodCountM(methodCount),
          pMethodsM(pMethods)
    {
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
        return isAbstractM;
    }

    virtual u32 GetMethodCount() const
    {
        return methodCountM;
    }

    virtual const Method &GetMethod(u32 index) const;

private:

    CompiledStructure superM;

    bool isAbstractM;

    u32 methodCountM;

    Method **pMethodsM;
};


class CompiledClass : public Class
{
public:
    
    CompiledClass(const char *pName, const char *pFullName, 
                  const Context *pContext, bool isIncomplete,
                  bool hasSizeof, u32 size, bool hasStructureName,
                  AccessType accessType,
                  const std::type_info *pTypeInfo,
                  u32 baseCount, Base **pBases,
                  u32 friendCount, Structure **pFriends,
                  u32 fieldCount, Field **pFields, 
                  bool isAnonymous, u32 constructorCount,
                  Constructor **pConstructors,
                  Destructor *pDestructor,
                  CompiledCreate *pCreate, 
                  CompiledCreateArray *pCreateArray,
                  CompiledDelete *pDelete,
                  CompiledDeleteArray *pDeleteArray, bool isAbstract,
                  u32 methodCount, Method **pMethods)
        : superM(pName, pFullName, pContext, isIncomplete, hasSizeof, size,
                 hasStructureName, accessType, pTypeInfo, baseCount, pBases,
                 friendCount, pFriends, fieldCount, pFields, isAnonymous,
                 constructorCount, pConstructors, pDestructor, pCreate,
                 pCreateArray, pDelete, pDeleteArray, isAbstract, methodCount,
                 pMethods)
    {
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

    CompiledStruct superM;
};


class CompiledMember : public Member
{
public:
    
    CompiledMember(AccessType accessType, Context *pContext,
                   const char *pName, bool isStatic)
        : accessTypeM(accessType), pContextM(pContext), pNameM(pName),
          isStaticM(isStatic)
    {
    }

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
        return pNameM;
    }

    virtual bool IsStatic() const
    {
        return isStaticM;
    }

private:

    AccessType accessTypeM;

    Context *pContextM;

    const char *pNameM;

    bool isStaticM;
};


class CompiledField : public Field
{
public:

    CompiledField(AccessType accessType, Context *pContext,
                  const char *pName, bool isStatic, Type *pType,
                  u32 bitCount, bool hasOffset, u32 offset, CompiledGet *pGet)
        : superM(accessType, pContext, pName, isStatic), pTypeM(pType),
          bitCountM(bitCount), hasOffsetM(hasOffset), offsetM(offset),
          pGetM(pGet)
    {
    }

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

    virtual u32 GetBitfieldBitCount() const
    {
        return bitCountM;
    }

    virtual const Type &GetType() const;

    virtual bool HasOffset() const
    {
        return hasOffsetM;
    }

    virtual u32 GetOffset() const
    {
        return offsetM;
    }

    virtual bool IsAccessible() const
    {
        return pGetM;
    }

    virtual void *Get(void *pInstance) const
    {
        return (*pGetM)(pInstance);
    }

private:

    CompiledMember superM;

    Type *pTypeM;

    u32 bitCountM;

    bool hasOffsetM;

    u32 offsetM;

    void *(*pGetM)(void *);
};


class CompiledArgument : public Argument
{
public:
    
    CompiledArgument(Type *pType, bool hasDefault, const char *pDefault)
        : pTypeM(pType), hasDefaultM(hasDefault), pDefaultM(pDefault)
    {
    }

    // Argument methods -------------------------------------------------------

    virtual const Type &GetType() const;

    virtual bool HasDefault() const
    {
        return hasDefaultM;
    }

    virtual const char *GetDefault() const
    {
        return pDefaultM;
    }

private:

    Type *pTypeM;

    bool hasDefaultM;

    const char *pDefaultM;
};


class CompiledDestructorSignature : public DestructorSignature
{
public:

    CompiledDestructorSignature(u32 throwCount, Type **pThrows)
        : throwCountM(throwCount), pThrowsM(pThrows)
    {
    }

    // DestructorSignature methods --------------------------------------------

    virtual u32 GetThrowCount() const
    {
        return throwCountM;
    }

    virtual const Type &GetThrow(u32 index) const
    {
        return (Type &) *(pThrowsM[index]);
    }

private:

    u32 throwCountM;

    Type **pThrowsM;
};


class CompiledConstructorSignature : public ConstructorSignature
{
public:

    CompiledConstructorSignature(u32 throwCount, Type **pThrows,
                                 u32 argumentCount, 
                                 Argument **pArguments,
                                 bool hasEllipsis)
        : superM(throwCount, pThrows), argumentCountM(argumentCount),
          pArgumentsM(pArguments), hasEllipsisM(hasEllipsis)
    {
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

    // ConstructorSignature methods -------------------------------------------

    virtual u32 GetArgumentCount() const
    {
        return argumentCountM;
    }

    virtual const Argument &GetArgument(u32 index) const
    {
        return *(pArgumentsM[index]);
    }

    virtual bool HasEllipsis() const
    {
        return hasEllipsisM;
    }

private:

    CompiledDestructorSignature superM;

    u32 argumentCountM;

    Argument **pArgumentsM;

    bool hasEllipsisM;
};


class CompiledMethodSignature : public MethodSignature
{
public:

    CompiledMethodSignature(u32 throwCount, Type **pThrows,
                            u32 argumentCount,  Argument **pArguments,
                            bool hasEllipsis, Type *pReturnType)
        : superM(throwCount, pThrows, argumentCount, pArguments, 
                 hasEllipsis), pReturnTypeM(pReturnType)
    {
    }

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

    virtual const Type &GetReturnType() const;

private:

    CompiledConstructorSignature superM;

    Type *pReturnTypeM;
};


class CompiledDestructor : public Destructor
{
public:

    CompiledDestructor(AccessType accessType, Context *pContext,
                       const char *pName, bool isStatic, bool isVirtual,
                       bool isPureVirtual, 
                       CompiledDestructorSignature *pSignature,
                       CompiledDestructorInvoke *pDestructorInvoke)
        : superM(accessType, pContext, pName, isStatic),
          isVirtualM(isVirtual), isPureVirtualM(isPureVirtual), 
          pSignatureM(pSignature), pDestructorInvokeM(pDestructorInvoke)
    {
    }

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
        return *pSignatureM;
    }

    virtual bool IsInvokeable() const
    {
        return pDestructorInvokeM;
    }

    virtual void Invoke(void *pInstance) const
    {
        (*pDestructorInvokeM)(pInstance);
    }

private:

    CompiledMember superM;

    bool isVirtualM, isPureVirtualM;

    CompiledDestructorSignature *pSignatureM;

    CompiledDestructorInvoke *pDestructorInvokeM;
};


class CompiledConstructor : public Constructor
{
public:

    CompiledConstructor(AccessType accessType, Context *pContext,
                        const char *pName, bool isStatic,
                        ConstructorSignature *pSignature,
                        const char **pArgumentNames,
                        CompiledConstructorInvoke *pConstructorInvoke)
        : superM(accessType, pContext, pName, isStatic),
          pSignatureM(pSignature), pArgumentNamesM(pArgumentNames),
          pConstructorInvokeM(pConstructorInvoke)
    {
    }

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
        return *pSignatureM;
    }

    virtual const char *GetArgumentName(u32 index) const
    {
        return pArgumentNamesM[index];
    }

    virtual bool IsInvokeable() const
    {
        return pConstructorInvokeM;
    }

    virtual void *Invoke(void **pArgumentValues) const
    {
        return (*pConstructorInvokeM)(pArgumentValues);
    }

private:

    CompiledMember superM;

    ConstructorSignature *pSignatureM;

    const char **pArgumentNamesM;

    void *(*pConstructorInvokeM)(void **);
};


class CompiledMethod : public Method
{
public:

    CompiledMethod(AccessType accessType, Context *pContext,
                   const char *pName, bool isStatic, bool isOperatorMethod,
                   bool isConst, bool isVirtual, bool isPureVirtual,
                   CompiledMethodSignature *pSignature, 
				   const char **pArgumentNames,
                   CompiledMethodInvoke *pMethodInvoke)
        : superM(accessType, pContext, pName, isStatic), 
          isOperatorMethodM(isOperatorMethod), isConstM(isConst),
          isVirtualM(isVirtual), isPureVirtualM(isPureVirtual),
          pSignatureM(pSignature), pArgumentNamesM(pArgumentNames),
          pMethodInvokeM(pMethodInvoke)
    {
    }

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
        return *pSignatureM;
    }

    virtual const char *GetArgumentName(u32 index) const
    {
        return pArgumentNamesM[index];
    }

    virtual bool IsInvokeable() const
    {
        return pMethodInvokeM;
    }

    virtual void Invoke(void *pInstance, void *pReturnValue,
                        void **pArgumentValues) const
    {
        (*pMethodInvokeM)(pInstance, pReturnValue, pArgumentValues);
    }

private:

    CompiledMember superM;

    bool isOperatorMethodM, isConstM, isVirtualM, isPureVirtualM;

    MethodSignature *pSignatureM;

    const char **pArgumentNamesM;

    CompiledMethodInvoke *pMethodInvokeM;
};


class CompiledType : public Type
{
public:

    CompiledType(BaseType baseType, bool isConst, bool isVolatile,
                 bool isReference, u32 arrayOrPointerCount,
                 ArrayOrPointer **pArrayOrPointers)
        : baseTypeM(baseType), isConstM(isConst), isVolatileM(isVolatile),
          isReferenceM(isReference), arrayOrPointerCountM(arrayOrPointerCount), 
          pArrayOrPointersM(pArrayOrPointers)
    {
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
        return arrayOrPointerCountM;
    }

    virtual const ArrayOrPointer &GetArrayOrPointer(u32 index) const;

private:

    BaseType baseTypeM;

    bool isConstM;

    bool isVolatileM;

    bool isReferenceM;

    u32 arrayOrPointerCountM;

    ArrayOrPointer **pArrayOrPointersM;
};


class CompiledPointer : public Pointer
{
public:

    CompiledPointer(bool isConst, bool isVolatile)
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

class CompiledArray : public Array
{
public:

    CompiledArray(bool isUnbounded, u32 elementCount)
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


class CompiledEnumeration : public Enumeration
{
public:

    CompiledEnumeration(AccessType accessType, Context *pContext,
                        const char *pName, u32 valueCount,
                        EnumerationValue **pValues)
        : accessTypeM(accessType), pContextM(pContext), pNameM(pName),
          valueCountM(valueCount), pValuesM(pValues)
    {
    }

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
        return pNameM;
    }
    
    virtual u32 GetValueCount() const
    {
        return valueCountM;
    }

    virtual const EnumerationValue &GetValue(u32 index) const;

private:

    AccessType accessTypeM;

    Context *pContextM;

    const char *pNameM;

    u32 valueCountM;

    EnumerationValue **pValuesM;
};


class CompiledEnumerationValue : public EnumerationValue
{
public:

    CompiledEnumerationValue(const char *pName, s32 value)
        : pNameM(pName), valueM(value)
    {
    }

    // EnumerationValue methods -----------------------------------------------

    virtual const char *GetName() const
    {
        return pNameM;
    }

    virtual s32 GetValue() const
    {
        return valueM;
    }

private:

    const char *pNameM;

    s32 valueM;
};


class CompiledTypeEnumeration : public TypeEnumeration
{
public:

    CompiledTypeEnumeration(Type::BaseType /* baseType */, bool isConst,
                            bool isVolatile, bool isReference,
                            u32 arrayOrPointerCount,
                            ArrayOrPointer **pArrayOrPointers,
                            Enumeration *pEnumeration)
        : superM(BaseType_Enumeration, isConst, isVolatile,
                 isReference, arrayOrPointerCount, pArrayOrPointers),
          pEnumerationM(pEnumeration)
    {
    }

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

    CompiledType superM;

    Enumeration *pEnumerationM;
};


class CompiledTypeFunction : public TypeFunction
{
public:

    CompiledTypeFunction(Type::BaseType /* baseType */, bool isConst,
                         bool isVolatile, bool isReference,
                         u32 arrayOrPointerCount, 
                         ArrayOrPointer **pArrayOrPointers,
                         MethodSignature *pSignature)
        : superM(BaseType_Function, isConst, isVolatile,
                 isReference, arrayOrPointerCount, pArrayOrPointers),
          pSignatureM(pSignature)
    {
    }

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
        return *pSignatureM;
    }

private:

    CompiledType superM;

    MethodSignature *pSignatureM;
};


class CompiledTypeStructure : public TypeStructure
{
public:
    
    CompiledTypeStructure(Type::BaseType /* baseType */, bool isConst,
                          bool isVolatile, bool isReference,
                          u32 arrayOrPointerCount, 
                          ArrayOrPointer **pArrayOrPointers,
                          Structure *pStructure)
        : superM(BaseType_Structure, isConst, isVolatile,
                 isReference, arrayOrPointerCount, pArrayOrPointers),
          pStructureM(pStructure)
    {
    }

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

    CompiledType superM;

    Structure *pStructureM;
};

class CompiledRegister
{
public:

    CompiledRegister(u32 contextCount, Context **pContexts);
};


class CompiledContextSet
{
public:
    
    static u32 GetContextCount();

    static const Context *GetContext(u32 index);

    static const Context *LookupContext(const char *pFullName);

    static const Structure *LookupStructure(const std::type_info &typeinfo);

    static void RegisterContext(Context *pContext);

    static void RegisterType(Type *pType);

    static void RegisterEnumeration(Enumeration *pEnumeration);
};

}; // namespace Xrtti


#endif /* XRTTI_PRIVATE_H */
