/*****************************************************************************\
 *                                                                           *
 * Generator.cpp                                                             *
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


#include <private/Generator.h>


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


GeneratorStructure::GeneratorStructure(Generator &generator, 
                                       const Structure &structure)
    : GeneratorContext(generator, (const Context &) structure), 
      rttiM(generator.GetConfiguration().GetRtti()), structureM(structure),
	  pDestructorM(0)
{
    u32 count = structure.GetBaseCount();

    if (count) {
        vBasesM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vBasesM[i] = new GeneratorBase
                (generator, structure.GetBase(i), this);
        }
    }

    count = structure.GetFriendCount();

    if (count) {
        vFriendsM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vFriendsM[i] = (GeneratorStructure *)
                generator.GetGeneratorContext(&(structure.GetFriend(i)));
        }
    }

    count = structure.GetFieldCount();

    if (count) {
        vFieldsM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vFieldsM[i] = new GeneratorField(generator, structure.GetField(i));
        }
    }

    count = structure.GetConstructorCount();

    if (count) {
        vConstructorsM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vConstructorsM[i] = new GeneratorConstructor
                (generator, structure.GetConstructor(i));
        }
    }

    if (structure.HasDestructor()) {
        pDestructorM = new GeneratorDestructor
            (generator, structure.GetDestructor());
    }
    else {
        pDestructorM = 0;
    }
}


GeneratorStructure::~GeneratorStructure()
{
	for (u32 i = 0; i < vBasesM.size(); i++) {
		delete vBasesM[i];
	}

	for (u32 i = 0; i < vFieldsM.size(); i++) {
		delete vFieldsM[i];
	}

	for (u32 i = 0; i < vConstructorsM.size(); i++) {
		delete vConstructorsM[i];
	}

	delete pDestructorM;
}


void GeneratorStructure::EmitXrttiAccess(FILE *file)
{
    if (!structureM.IsIncomplete() && !structureM.IsAnonymous() &&
        Generator::IsAccessible(&structureM)) {
        fprintf(file, "    static const Xrtti::u32 _%lu_sizeof = "
                "sizeof(%s);\n\n", (unsigned long) this->GetNumber(),
                structureM.GetFullName());
        if (rttiM) {
            fprintf(file, "    static const std::type_info "
                    "*_%lu_get_typeinfo()\n    {\n        "
                    "return &typeid(%s);\n    }\n\n",
                    (unsigned long) this->GetNumber(), 
                    structureM.GetFullName());
        }
    }
    
    u32 count = structureM.GetBaseCount();
    for (u32 i = 0; i < count; i++) {
        vBasesM[i]->EmitXrttiAccess(file);
    }

    count = structureM.GetFieldCount();
    for (u32 i = 0; i < count; i++) {
        vFieldsM[i]->EmitXrttiAccess(file);
    }

    // While emitting xrtti access for constructors and destructors, make
    // sure to note if the default constructor and/or default destructor
    // are inaccessible, which will prevent them from being emitted.  Also,
    // shortcircuit this process if the structure itself is not accessible.
    bool defaultConstructorAccessible, defaultDestructorAccessible;
    defaultConstructorAccessible = defaultDestructorAccessible =
        Generator::IsAccessible(&structureM);

    count = structureM.GetConstructorCount();
    for (u32 i = 0; i < count; i++) {
        GeneratorConstructor *pConstructor = vConstructorsM[i];
        pConstructor->EmitXrttiAccess(file);

        // If it's the default constructor, is it accessible?
        if (pConstructor->IsDefault() && !pConstructor->IsAccessible()) {
            defaultConstructorAccessible = false;
        }
    }

    if (pDestructorM) {
        pDestructorM->EmitXrttiAccess(file);
        if (!pDestructorM->IsAccessible()) {
            defaultDestructorAccessible = false;
        }
    }

    // Finally, only emit constructors and destructors if this structure
    // is not anonymous, abstract, or incomplete
    if (structureM.IsAnonymous() || structureM.IsIncomplete() ||
        (((structureM.GetType() == Context::Type_Class) ||
          (structureM.GetType() == Context::Type_Struct)) &&
         ((const Struct &) structureM).IsAbstract())) {
        return;
    }

    if (this->CanInvokeDefaultConstructor()) {
        // create
        fprintf(file, "    static void *_%lu_create()\n    {\n", 
                (unsigned long) this->GetNumber());
        fprintf(file, "        return new %s();\n    }\n\n", 
                Generator::GetTypeName(&structureM).c_str());

        // createarray
        fprintf(file, "    static void *_%lu_create_array(Xrtti::u32 count)\n"
                "    {\n", (unsigned long) this->GetNumber());
        fprintf(file, "        return new %s[count];\n    }\n\n", 
                Generator::GetTypeName(&structureM).c_str());
    }

    if (this->CanInvokeDefaultDestructor()) {
        // delete
        fprintf(file, "    static void _%lu_delete(void *pThis)\n    {\n",
                (unsigned long) this->GetNumber());
        fprintf(file, "        delete (%s *) pThis;\n    }\n\n", 
                Generator::GetTypeName(&structureM).c_str());

        // deletearray
        fprintf(file, "    static void _%lu_delete_array(void *pThis)\n    {\n",
                (unsigned long) this->GetNumber());
        fprintf(file, "        delete [] (%s *) pThis;\n    }\n\n", 
                Generator::GetTypeName(&structureM).c_str());
    }
}


void GeneratorStructure::EmitXrttiAccessStaticDefinitions(FILE *file)
{
    u32 count = structureM.GetFieldCount();
    for (u32 i = 0; i < count; i++) {
        vFieldsM[i]->EmitXrttiAccessStaticDefinitions(file);
    }
}


void GeneratorStructure::EmitNeededTypedefs(FILE *file)
{
    u32 count = structureM.GetFieldCount();
    for (u32 i = 0; i < count; i++) {
        vFieldsM[i]->EmitTypedefs(file);
    }

    count = structureM.GetConstructorCount();
    for (u32 i = 0; i < count; i++) {
        vConstructorsM[i]->EmitTypedefs(file);
    }
}

bool GeneratorStructure::CanInvokeDefaultConstructor()
{
    if (!this->CanInvokeDefaultDestructor()) {
        return false;
    }

    bool hasAccessibleDefaultConstructor = false;
    bool hasNonDefaultConstructor = false;

    u32 count = structureM.GetConstructorCount();
    for (u32 i = 0; i < count; i++) {
        GeneratorConstructor *pConstructor = vConstructorsM[i];

        // If it's the default constructor, can it be invoked?
        if (pConstructor->IsDefault()) {
            hasAccessibleDefaultConstructor = 
                pConstructor->IsAccessible() && pConstructor->CanInvoke();
            break;
        }
        else {
            hasNonDefaultConstructor = true;
        }
    }

    return (hasAccessibleDefaultConstructor || !hasNonDefaultConstructor);
}


bool GeneratorStructure::CanInvokeDefaultDestructor()
{
    if (!Generator::IsAccessible(&structureM)) {
        return false;
    }

    if (pDestructorM) {
        return (pDestructorM->IsAccessible() && pDestructorM->CanInvoke());
    }

    return true;
}


void GeneratorStructure::EmitExtraDeclarations(FILE *file)
{
    u32 count = structureM.GetBaseCount();
    for (u32 i = 0; i < count; i++) {
        vBasesM[i]->EmitDeclaration(file);
    }

    count = structureM.GetFieldCount();
    for (u32 i = 0; i < count; i++) {
        vFieldsM[i]->EmitDeclaration(file);
    }
    
    count = structureM.GetConstructorCount();
    for (u32 i = 0; i < count; i++) {
        vConstructorsM[i]->EmitDeclaration(file);
    }

    if (pDestructorM) {
        pDestructorM->EmitDeclaration(file);
    }
}


void GeneratorStructure::EmitExtraDefinitions(FILE *file)
{
    u32 count = structureM.GetBaseCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vBasesM[i]->EmitDefinition(file);
        }
        fprintf(file, "    static Xrtti::Base *_%lu_bases[] =\n    {\n",
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vBasesM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }

    count = structureM.GetFriendCount();
    if (count) {
        fprintf(file, "    static Xrtti::Structure *_%lu_friends[] =\n    {\n",
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vFriendsM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }

    count = structureM.GetFieldCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vFieldsM[i]->EmitDefinition(file);
        }
        fprintf(file, "    static Xrtti::Field *_%lu_fields[] =\n    {\n",
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vFieldsM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }

    count = structureM.GetConstructorCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vConstructorsM[i]->EmitDefinition(file);
        }
        fprintf(file, "    static Xrtti::Constructor *"
                "_%lu_constructors[] =\n    {\n", 
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vConstructorsM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }

    if (pDestructorM) {
        pDestructorM->EmitDefinition(file);
    }
}


void GeneratorStructure::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorContext::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    // bool isIncomplete
    Generator::EmitBooleanArgument(file, structureM.IsIncomplete(), true);

    // bool hasSizeof
    // u32 sizeof
    if (structureM.IsIncomplete() || structureM.IsAnonymous() ||
        !Generator::IsAccessible(&structureM)) {
        Generator::EmitBooleanArgument(file, false, true);
        Generator::EmitU32Argument(file, 0, true);
    }
    else {
        Generator::EmitBooleanArgument(file, true, true);
        fprintf(file, "        XrttiAccess::_%lu_sizeof,\n", 
                (unsigned long) this->GetNumber());
    }

    // bool hasStructureName
    Generator::EmitBooleanArgument(file, structureM.HasStructureName(), true);
    
    // AccessType accessType
    Generator::EmitAccessTypeArgument(file, structureM.GetAccessType(), true);

    // std::type_info *pTypeInfo
    if (rttiM && !structureM.IsAnonymous() && !structureM.IsIncomplete() &&
        Generator::IsAccessible(&structureM)) {
        fprintf(file, "        XrttiAccess::_%lu_get_typeinfo(),\n",
                (unsigned long) this->GetNumber());
    }
    else {
        fprintf(file, "        0,\n");
    }

    // u32 baseCount
    // CompiledBase **pBases
    Generator::EmitArrayArgument
        (file, structureM.GetBaseCount(), "bases", this->GetNumber(), true);

    // u32 friendCount
    // CompiledStructure **pFriends
    Generator::EmitArrayArgument
        (file, structureM.GetFriendCount(), "friends", this->GetNumber(), true);
    
    // u32 fieldCount
    // CompiledField **pFields
    Generator::EmitArrayArgument
        (file, structureM.GetFieldCount(), "fields", this->GetNumber(), true);

    // bool isAnonymous
    Generator::EmitBooleanArgument(file, structureM.IsAnonymous(), true);

    // u32 constructorCount
    // CompiledConstructor **pConstructors
    Generator::EmitArrayArgument
        (file, structureM.GetConstructorCount(), "constructors",
         this->GetNumber(), true);

    // CompiledDestructor *pDestructor
    if (pDestructorM) {
        fprintf(file, "        &_%lu,\n", 
                (unsigned long) pDestructorM->GetNumber());
    }
    else {
        fprintf(file, "        0,\n");
    }

    // CompiledCreate *pCreate
    // CompiledCreateArray *pCreateArray
    if (this->CanInvokeDefaultConstructor()) {
        fprintf(file, "        &XrttiAccess::_%lu_create,\n", 
                (unsigned long) this->GetNumber());
        fprintf(file, "        &XrttiAccess::_%lu_create_array,\n", 
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitU32Argument(file, 0, true);
        Generator::EmitU32Argument(file, 0, true);
    }

    // CompiledDelete *pDelete
    // CompiledDeleteArray *pDeleteArray    
    if (this->CanInvokeDefaultDestructor()) {
        fprintf(file, "        &XrttiAccess::_%lu_delete,\n", 
                (unsigned long) this->GetNumber());
        fprintf(file, "        &XrttiAccess::_%lu_delete_array",
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitU32Argument(file, 0, true);
        Generator::EmitU32Argument(file, 0, false);
    }
}


}; // namespace Xrtti
