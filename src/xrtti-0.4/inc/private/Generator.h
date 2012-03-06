/*****************************************************************************\
 *                                                                           *
 * Generator.h                                                               *
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

#ifndef GENERATOR_H
#define GENERATOR_H

#include <map>
#include <string>
#include <vector>
#include <Xrtti/XrttiParsed.h>
#include <private/Configuration.h>

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


class Generator;
class GeneratorArgument;
class GeneratorArray;
class GeneratorArrayOrPointer;
class GeneratorBase;
class GeneratorClass;
class GeneratorConstructor;
class GeneratorConstructorSignature;
class GeneratorContext;
class GeneratorDestructor;
class GeneratorDestructorSignature;
class GeneratorEnumeration;
class GeneratorEnumerationValue;
class GeneratorField;
class GeneratorImplementation;
class GeneratorMethod;
class GeneratorMethodSignature;
class GeneratorNamespace;
class GeneratorObject;
class GeneratorPointer;
class GeneratorStructure;
class GeneratorStruct;
class GeneratorType;
class GeneratorTypeEnumeration;
class GeneratorTypeFunction;
class GeneratorTypeStructure;
class GeneratorTypeUnion;
class GeneratorUnion;


class GeneratorObject
{
public:

    GeneratorObject()
        : numberM(nextNumberG++)
    {
    }

    virtual ~GeneratorObject()
    {
    }

    u32 GetNumber()
    {
        return numberM;
    }

    // Implemented by subclasses to define any XrttiAccess output which needs
    // to happen in the XrttiAccess class definition.  Subclasses should
    // always call their superclass EmitXrttiAccess() method first.
    virtual void EmitXrttiAccess(FILE * /* fileOut */)
    {
    }

    // Implemented by subclasses to emit the definition of static fields
    // defined in XrttiAccess
    virtual void EmitXrttiAccessStaticDefinitions(FILE * /* fileOut */)
    {
    }

    // Emits the declarations which need to happen in the _ class
    void EmitDeclaration(FILE *fileOut);

    // Emits the definitions of the declarations from EmitDeclarations
    void EmitDefinition(FILE *fileOut);

protected:

    // Implemented by subclasses to return the Compiled type to create at
    // runtime for this object
    virtual const char *GetCompiledTypeName() = 0;

    // Implemented by subclasses to emit any extra declarations aside from
    // the declaration of this object itself.  Subclasses should always
    // call their superclass EmitExtraDeclarations() method first.
    virtual void EmitExtraDeclarations(FILE * /* fileOut */)
    {
    }

    // Implemented by subclasses to emit any extra definitions aside from
    // the definition of this object itself.  Subclasses should always
    // call their superclass EmitExtraDefinitions() method first.
    virtual void EmitExtraDefinitions(FILE * /* fileOut */)
    {
    }

    // Implemented by subclasses to emit any arguments for this specific class
    // in the constructor of the Compiled representation of this object.
    // Subclasses should always call their superclass
    // EmitDefinitionArguments() method first.
    virtual void EmitDefinitionArguments(FILE *fileOut) = 0;

private:

    u32 numberM;

    static u32 nextNumberG;
};


class GeneratorContext : public GeneratorObject
{
public:

    GeneratorContext(Generator &generator, const Context &context);

    bool IsAccessible();

    std::string GetTypeName();

    void EmitTypedefs(FILE *fileOut);

protected:

    virtual void EmitNeededTypedefs(FILE *fileOut) = 0;

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    bool typedefsEmittedM;

    const Context &contextM;

    GeneratorContext *pContextM;
};


class GeneratorNamespace : public GeneratorContext
{
public:

    GeneratorNamespace(Generator &generator, const Context &context)
        : GeneratorContext(generator, context)
    {
    }

protected:

    virtual void EmitNeededTypedefs(FILE * /* fileOut */)
    {
    }

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledNamespace";
    }
};


class GeneratorStructure : public GeneratorContext
{
public:

    GeneratorStructure(Generator &generator, const Structure &structure);

    virtual ~GeneratorStructure();

    virtual void EmitXrttiAccess(FILE *fileOut);

    virtual void EmitXrttiAccessStaticDefinitions(FILE *fileOut);

protected:

    virtual void EmitNeededTypedefs(FILE *fileOut);

    bool CanInvokeDefaultConstructor();

    bool CanInvokeDefaultDestructor();

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    bool rttiM;

    const Structure &structureM;

    std::vector<GeneratorBase *> vBasesM;

    std::vector<GeneratorStructure *> vFriendsM;

    std::vector<GeneratorField *> vFieldsM;

    std::vector<GeneratorConstructor *> vConstructorsM;

    GeneratorDestructor *pDestructorM;
};


class GeneratorBase : public GeneratorObject
{
public:

    GeneratorBase(Generator &generator, const Base &base,
                  GeneratorStructure *pSubstructure);

    virtual void EmitXrttiAccess(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledBase";
    }

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    bool CanEmitCast();

    const Base &baseM;

    GeneratorStructure *pSubstructureM;

    GeneratorStructure *pStructureM;
};


class GeneratorUnion : public GeneratorStructure
{
public:

    GeneratorUnion(Generator &generator, const Union &unionRef)
        : GeneratorStructure(generator, (const Structure &) unionRef)
    {
    }

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledUnion";
    }
};


class GeneratorStruct : public GeneratorStructure
{
public:

    GeneratorStruct(Generator &generator, const Struct &structRef);

    virtual ~GeneratorStruct();

    virtual void EmitXrttiAccess(FILE *fileOut);

protected:

    virtual void EmitNeededTypedefs(FILE *fileOut);

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledStruct";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const Struct &structM;
    
    std::vector<GeneratorMethod *> vMethodsM;
};


class GeneratorClass : public GeneratorStruct
{
public:

    GeneratorClass(Generator &generator, const Class &classRef)
        : GeneratorStruct(generator, (const Struct &) classRef)
    {
    }

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledClass";
    }
};


class GeneratorMember : public GeneratorObject
{
public:
    
    GeneratorMember(Generator &generator, const Member &member);

    bool IsAccessible();

    const char *GetName()
    {
        return memberM.GetName();
    }

protected:

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const Member &memberM;

    GeneratorContext *pContextM;
};


class GeneratorField : public GeneratorMember
{
public:

    GeneratorField(Generator &generator, const Field &field);

    void EmitTypedefs(FILE *fileOut);

    virtual void EmitXrttiAccess(FILE *fileOut);

    virtual void EmitXrttiAccessStaticDefinitions(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledField";
    }

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    bool CanEmitOffsetof();

    bool CanEmitGet();

    void EmitGet(FILE *fileOut);

    const Field &fieldM;

    GeneratorType *pTypeM;
};


class GeneratorArgument : public GeneratorObject
{
public:

    GeneratorArgument(Generator &generator, const Argument &argument);

    bool IsPointer();

    bool TypeIsConstNonPointer();

    u32 GetTypeNumber();

    bool CanEmitType();

    void EmitTypedefs(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledArgument";
    }

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const Argument &argumentM;

    GeneratorType *pTypeM;
};


class GeneratorDestructorSignature : public GeneratorObject
{
public:

    GeneratorDestructorSignature(Generator &generator,
                                 const DestructorSignature &destructorSig);

    virtual ~GeneratorDestructorSignature();

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledDestructorSignature";
    }

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const DestructorSignature &destructorSignatureM;
    
    std::vector<GeneratorType *> vThrowsM;
};


class GeneratorConstructorSignature : public GeneratorDestructorSignature
{
public:

    GeneratorConstructorSignature(Generator &generator,
                                  const ConstructorSignature &sig);

    virtual ~GeneratorConstructorSignature();

    u32 GetArgumentCount()
    {
        return constructorSignatureM.GetArgumentCount();
    }

    u32 GetArgumentTypeNumber(u32 index)
    {
        return vArgumentsM[index]->GetTypeNumber();
    }

    bool CanEmitArguments();

    void EmitInvokeArguments(FILE *fileOut);

    void EmitTypedefs(FILE *fileOut);

    void EmitArgumentTypeList(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledConstructorSignature";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const ConstructorSignature &constructorSignatureM;

    std::vector<GeneratorArgument *> vArgumentsM;
};


class GeneratorMethodSignature : public GeneratorConstructorSignature
{
public:
    
    GeneratorMethodSignature(Generator &generator, const MethodSignature &sig);
    
    u32 GetReturnTypeNumber();

    bool CanEmitReturnTypeAndArguments();

    void EmitTypedefs(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledMethodSignature";
    }

    virtual void EmitDefinitionArguments(FILE *fileOut);
    
private:

    const MethodSignature &methodSignatureM;

    GeneratorType *pReturnTypeM;
};


class GeneratorDestructor : public GeneratorMember
{
public:

    GeneratorDestructor(Generator &generator, const Destructor &destructor)
        : GeneratorMember(generator, (const Member &) destructor),
          destructorM(destructor), 
          destructorSignatureM(generator, destructor.GetSignature())
    {
    }

    bool IsAccessible();

    bool CanInvoke();

    virtual void EmitXrttiAccess(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledDestructor";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:
    
    const Destructor &destructorM;

    GeneratorDestructorSignature destructorSignatureM;
};


class GeneratorConstructor : public GeneratorMember
{
public:

    GeneratorConstructor(Generator &generator, const Constructor &constructor)
        : GeneratorMember(generator, (const Member &) constructor),
          constructorM(constructor), 
          constructorSignatureM(generator, constructor.GetSignature())
    {
    }

    bool IsDefault()
    {
        return !constructorM.GetSignature().GetArgumentCount();
    }

    bool IsAccessible();

    bool CanInvoke();

    virtual void EmitTypedefs(FILE *fileOut);

    void EmitXrttiAccess(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledConstructor";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const Constructor &constructorM;

    GeneratorConstructorSignature constructorSignatureM;
};


class GeneratorMethod : public GeneratorMember
{
public:

    GeneratorMethod(Generator &generator, const Method &method);

    bool CanInvoke();

    virtual void EmitTypedefs(FILE *fileOut);

    void EmitXrttiAccess(FILE *fileOut);

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledMethod";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const Method &methodM;

    GeneratorMethodSignature methodSignatureM;
};


class GeneratorType : public GeneratorObject
{
public:

    GeneratorType(Generator &generator, const Type &type);

    virtual ~GeneratorType();

    virtual bool CanEmitTypedef()
    {
        return true;
    }

    void EmitTypedef(FILE *fileOut);

protected:

    virtual void EmitPrerequisiteTypedefs(FILE * /* fileOut */)
    {
    }

    virtual void EmitTypedefContents(FILE *fileOut);

    virtual std::string GetTypedefTypeName();

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledType";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

    std::string GetTypedefTypeDefinition();

private:

    const char *GetBaseTypeName();

    std::string GetCppTypeName();

    bool typedefEmittedM;

    const Type &typeM;

    std::vector<GeneratorArrayOrPointer *> vArrayOrPointersM;
};


class GeneratorArrayOrPointer : public GeneratorObject
{
public:

    GeneratorArrayOrPointer()
    {
    }

    virtual ArrayOrPointer::Type GetType() = 0;
};


class GeneratorArray : public GeneratorArrayOrPointer
{
public:

    GeneratorArray(const Array &array)
        : arrayM(array)
    {
    }

    virtual ArrayOrPointer::Type GetType()
    {
        return ArrayOrPointer::Type_Array;
    }

protected:
    
    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledArray";
    }
    
    virtual void EmitDefinitionArguments(FILE *fileOut);

private:
    
    const Array &arrayM;
};


class GeneratorPointer : public GeneratorArrayOrPointer
{
public:

    GeneratorPointer(const Pointer &pointer)
        : pointerM(pointer)
    {
    }

    virtual ArrayOrPointer::Type GetType()
    {
        return ArrayOrPointer::Type_Pointer;
    }

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledPointer";
    }
    
    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const Pointer &pointerM;
};


class GeneratorEnumeration : public GeneratorObject
{
public:

    GeneratorEnumeration(Generator &generator, const Enumeration &enumeration);

    virtual ~GeneratorEnumeration();

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledEnumeration";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const Enumeration &enumerationM;

    GeneratorContext *pContextM;

    std::vector<GeneratorEnumerationValue *> vValuesM;
};


class GeneratorEnumerationValue : public GeneratorObject
{
public:

    GeneratorEnumerationValue(const EnumerationValue &value)
        : valueM(value)
    {
    }

protected:

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledEnumerationValue";
    }

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const EnumerationValue &valueM;
};


class GeneratorTypeEnumeration : public GeneratorType
{
public:

    GeneratorTypeEnumeration(Generator &generator,
                             const TypeEnumeration &typeEnumeration);

    virtual bool CanEmitTypedef();

protected:

    virtual std::string GetTypedefTypeName();

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledTypeEnumeration";
    }

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:
    
    const TypeEnumeration &typeEnumerationM;

    GeneratorEnumeration *pEnumerationM;
};


class GeneratorTypeFunction : public GeneratorType
{
public:

    GeneratorTypeFunction(Generator &generator, 
                          const TypeFunction &typeFunction);

	virtual ~GeneratorTypeFunction();

    virtual bool CanEmitTypedef();

protected:

    virtual void EmitTypedefContents(FILE *fileOut);

    virtual void EmitPrerequisiteTypedefs(FILE *fileOut);

    virtual std::string GetTypedefTypeName();

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledTypeFunction";
    }

    virtual void EmitExtraDeclarations(FILE *fileOut);

    virtual void EmitExtraDefinitions(FILE *fileOut);

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const TypeFunction &typeFunctionM;
    
    GeneratorMethodSignature *pMethodSignatureM;
};


class GeneratorTypeStructure : public GeneratorType
{
public:

    GeneratorTypeStructure(Generator &generator,
                           const TypeStructure &typeStructure);

    virtual bool CanEmitTypedef();

protected:

    virtual std::string GetTypedefTypeName();

    virtual const char *GetCompiledTypeName()
    {
        return "Xrtti::CompiledTypeStructure";
    }

    virtual void EmitDefinitionArguments(FILE *fileOut);

private:

    const TypeStructure &typeStructureM;
    
    GeneratorStructure *pStructureM;
};


class Generator
{
public:

    Generator(Configuration &config, const ContextSet &contextSet);
    ~Generator();

    bool Generate();

    const Configuration &GetConfiguration()
    {
        return configM;
    }

    GeneratorContext *GetGeneratorContext(const Context *pContext);
    GeneratorType *GetGeneratorType(const Type *pType);
    GeneratorEnumeration *GetGeneratorEnumeration(const Enumeration *pEnum);

    void RegisterGeneratorContext(const Context *pContext,
                                  GeneratorContext *pGeneratorContext);
    void RegisterGeneratorType(const Type *pType,
                               GeneratorType *pGeneratorType);
    void RegisterGeneratorEnumeration(const Enumeration *pEnumeration,
                                      GeneratorEnumeration *pGeneratorEnum);

    static void EmitBooleanArgument(FILE *fileOut, bool val, bool comma);
    static void EmitStringArgument(FILE *fileOut, const char *str, bool comma);
    static void EmitU32Argument(FILE *fileOut, u32 number, bool comma);
    static void EmitAccessTypeArgument(FILE *fileOut, 
                                       AccessType accessType, bool comma);
    static void EmitArrayArgument(FILE *fileOut, u32 count,
                                  const char *name, u32 number, bool comma);

    static bool IsAccessible(const Context *pContext);
    static bool IsAccessible(const Member *pMember);
    static bool IsPod(const Context *pContext);

    static std::string GetTypeName(const Context *pContext);

private:

    static bool IsXrttiAccessFriend(const Structure *pStructure);

    void EmitHeader(FILE *fileOut);
    void EmitXrttiAccess(FILE *fileOut);
    void EmitXrttiAccessStaticDefinitions(FILE *fileOut);
    void EmitTypedefs(FILE *fileOut);
    void EmitDeclarations(FILE *fileOut);
    void EmitDefinitions(FILE *fileOut);
    void EmitRegister(FILE *fileOut);

    Configuration &configM;

    std::map<const Context *, GeneratorContext *> htGeneratorContextsM;

    std::map<const Type *, GeneratorType *> htGeneratorTypesM;

    std::map<const Enumeration *, 
             GeneratorEnumeration *> htGeneratorEnumerationsM;
};


}; // namespace Xrtti

#endif // GENERATOR_H
