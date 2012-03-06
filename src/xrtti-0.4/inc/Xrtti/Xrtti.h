/*****************************************************************************\
 *                                                                           *
 * Xrtti.h                                                                   *
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


#ifndef XRTTI_H
#define XRTTI_H

#include <inttypes.h>
#include <typeinfo>

/** **************************************************************************
 * Everything which follows is in the Xrtti C++ namespace.
 ************************************************************************** **/

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


/** **************************************************************************
 * Short names for fixed-width integer types
 ************************************************************************** **/

typedef uint32_t           u32;       /**< 32-bit unsigned integer */
typedef int32_t            s32;       /**< 32-bit signed integer */


/** **************************************************************************
 * Forward declaration of classes and structures
 ************************************************************************** **/

class Argument;
class Array;
class ArrayOrPointer;
class Base;
class Class;
class Constructor;
class ConstructorSignature;
class Context;
class Destructor;
class DestructorSignature;
class Enumeration;
class EnumerationValue;
class Field;
class Method;
class MethodSignature;
class Namespace;
class Pointer;
class Structure;
class Struct;
class Type;
class TypeEnumeration;
class TypeFunction;
class TypeStructure;
class TypeUnion;
class Union;


/** **************************************************************************
 * AccessType defines the access types of C++, which are used to define how
 * code may access classes, structs, methods, fields, and enumerations
 ************************************************************************** **/
typedef enum AccessType
{
	AccessType_Public,
	AccessType_Protected,
	AccessType_Private
} AccessType;


/** **************************************************************************
 * An Xrtti::Context is either a namespace or a structure (class, struct,
 * or union)
 ************************************************************************** **/
class Context
{
public:

    /** **********************************************************************
     * This enumerates the valid Context types
     ********************************************************************** **/
    enum Type
    {
		Type_Class          = 0,
		Type_Namespace      = 1,
		Type_Struct         = 2,
		Type_Union          = 3
	};

    /**
     * Destructor
     **/
    virtual ~Context() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Context &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Context &other) const
    { return !(*this == other); }

    /** 
     * Returns the type of this context.  The most derived class of this
     * object is determined by the value returned here:
     *
     * Type_Class: most derived type is Class <br>
     * Type_Namespace: most derived type is Namespace <br>
     * Type_Struct: most derived type is Struct <br>
     * Type_Union: most derived type is Union <br>
     *
     * @return the type of this context
     **/
    virtual Type GetType() const = 0;

    /**
     * Returns the name of this context.
     *
     * @return the name of this context.
     **/
    virtual const char *GetName() const = 0;

    /**
     * Returns the full name of this context.  This includes the entire
     * namespace leading up to this context.  It is the name of the parent
     * Context prepended to the name of this Context, with a double-colon
     * separator.
     *
     * @return the full name of this context
     **/
    virtual const char *GetFullName() const = 0;

    /**
     * This is the parent context of this context.  If this is NULL, then this
     * Context is the root Context and has no parent.
     **/
    virtual const Context *GetContext() const = 0;
};


/** **************************************************************************
 * Namespace describes a C++ namespace.  A Namespace has no members beyond
 * those of a Context.
 ************************************************************************** **/
class Namespace : public Context
{
public:

    /**
     * Destructor
     **/
    virtual ~Namespace() { }

    /** 
     * Returns the type of this Context, which is Type_Namespace.
     * 
     * @return the type of this Context, which is Type_Namespace.
     **/
    virtual Type GetType() const
    {
        return Type_Namespace;
    }
};


/** **************************************************************************
 * Structure is the base class for Class, Struct, and Union.  It defines the
 * members that are shared by these three context types.
 ************************************************************************** **/
class Structure : public Context
{
public:

    /**
     * Destructor
     **/
    virtual ~Structure() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Structure &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Structure &other) const
    { return !(*this == other); }

    /**
     * If the Structure is Incomplete, its definition is not present in the
     * Xrtti system.  None of the remaining methods of the class will return
     * anything meaningful; only the methods of Context will return any
     * valid information.
     * 
     * @return true if the Structure is incomplete, meaning that although it
     *              was declared, no definition was given for it and virtually
     *              no information will be available about it
     **/
    virtual bool IsIncomplete() const = 0;

    /**
     * Returns true if this Structure object can report the size of the
     * described C++ structure via the GetSizeof() method, false if it cannot.
     *
     * @return true if this Structure object can report the size of the
     * described C++ structure via the GetSizeof() method, false if it cannot.
     **/
    virtual bool HasSizeof() const = 0;

    /**
     * Returns the size of an instance of this Structure in memory, as
     * would be the result of a sizeof() operation on the C++ type described
     * by this Structure.
     *
     * @return the size of an instance of this Structure in memory, as would
     *         be the result of a sizeof() operation on the C++ type described
     *         by this Structure.
     **/
    virtual u32 GetSizeof() const = 0;

    /**
     * Each Structure type (class, struct, union) can be defined in three
     * possible ways in C++, as illustrated by this example:
     *
     * struct a
     * {
     * };
     *
     * typedef struct b
     * {
     * } b;
     *
     * typedef struct
     * {
     * } c;
     *
     * (a) and (b) are essentially equivalent in C++; for (a), an implicit
     * typedef of "a" to "struct a" will be in effect, *unless* there is
     * another symbol defined as "a" (for example, a function name).
     *
     * (c) is different in that there is no "struct c" construct, there is
     * only a typedef of "c" defining an unnamed structured type.
     *
     * In Xrtti, a Structure object will be created for all of (a), (b), and
     * (c), and they will be indistinguishable, except that HasStructureName()
     * will return true for (a) and (b), and false for (c).
     *
     * If HasStructureName returns true, then it is safe (and sometimes
     * necessary) to refer to the structure as "struct a" (or "class a"
     * or "union a" depending on the actual type of the Structure).  If
     * HasStructureName returns false, then the Structure can only be
     * referred to by name, never by "struct a", "class b", or "union c".
     *
     * @return true if "class/struct/union <this->GetFullName()>" is a valid
     *         C++ construct, false if not
     **/
    virtual bool HasStructureName() const = 0;
    
    /**
     * Returns the access type of the Struct within the context in which it is
     * defined.
     *
     * @return the access type of the Struct within the context in which it is
     *         defined.
     **/
    virtual AccessType GetAccessType() const = 0;

    /**
     * If the -r option was specified to the xrttigen program, then C++ RTTI
     * type_info objects will be associated with each Structure.  This method
     * will then return the type_info object associated with the C++
     * class/struct/union described by this Structure.
     *
     * If -r was not used with xrttigen, then C++ RTTI is not supported, and
     * this method will return NULL.
     *
     * @return the C++ RTTI type_info object associated with the C++ class/
     *         struct/union described by this Structure, or NULL if RTTI
     *         options were not enabled for xrttigen when it produced the
     *         Generated object resulting in this Structure.
     **/
    virtual const std::type_info *GetTypeInfo() const = 0;

    /**
     * This is the number of base structures of this Structure.  Base
     * structures are, in essential, base classes, although they are not
     * necessarily classes; they may be structs or unions as well.  The
     * Base class identifies both the base Structure, and the offset from
     * this structure to the base class.
     *
     * @return the number of base structures of this Structure
     **/
    virtual u32 GetBaseCount() const = 0;

    /**
     * Returns a base structure of this Structure.
     * 
     * @param index is the number of the base Structure to return
     * @return a base structure of this Structure.
     **/
    virtual const Base &GetBase(u32 index) const = 0;

    /**
     * Returns the number of friends of this Structure.
     *
     * @return the number of friends of this Structure.
     **/
    virtual u32 GetFriendCount() const = 0;

    /**
     * Returns a Structure which was declared to be a friend of this
     * Structure.
     *
     * @param index is the number of the friend to return
     * @return a friend of this Structure.
     **/
    virtual const Structure &GetFriend(u32 index) const = 0;

    /**
     * Returns the number of Fields in this Context.
     *
     * @return the number of Fields in this Context.
     **/
    virtual u32 GetFieldCount() const = 0;

    /**
     * Returns a Field of this Structure.  These are in the same order as in
     * defined in the C++ class described by this Structure.  The number of
     * Fields in this Structure is given by the return value of
     * GetFieldCount().
     *
     * @param index is the number of the Field to return
     * @return a Field of this Structure.
     **/
    virtual const Field &GetField(u32 index) const = 0;

    /**
     * Returns true if the Structure is anonymous.  Anonymous structures have
     * no type name and thus cannot be instantiated.  Therefore, the
     * Constructors will not be invokeable; and the Create and CreateArray
     * methods will return NULL.  If this structure or any containing
	 * structure is anonymous, then this will return true.
	 * 
     * @return true if the Structure is anonymous; false if not.
     **/
    virtual bool IsAnonymous() const = 0;

    /**
     * This is the number of constructors of this Structure.
     *
     * @return the number of constructors of this Structure
     **/
    virtual u32 GetConstructorCount() const = 0;

    /**
     * Returns a constructor of this Structure.
     *
     * @param index is the number of the Constructor to return
     * @return a constructor of this Structure.
     **/
    virtual const Constructor &GetConstructor(u32 index) const = 0;

    /**
     * Returns true if this Structure has a destructor, false if not.
     *
     * @return true if this Structure has a destructor, false if not.
     **/
    virtual bool HasDestructor() const = 0;

    /**
     * Returns the destructor of this Structure, if one exists; this method
     * should only by called if HasDestructor returns true.
     *
     * @return the destructor of this Structure, if one exists
     **/
    virtual const Destructor &GetDestructor() const = 0;

    /**
     * Returns true if the Structure can be created.  Only if this method
     * returns true may the Create() method or CreateArray() methods be
     * called.
     *
     * @return true if the Structure can be created.  Only if this method
     *              returns true may the Create() method or CreateArray()
     *              methods be called.
     **/
    virtual bool IsCreatable() const = 0;

    /**
     * Returns a newly-constructed object of this Structure type; the
     * default constructor is used.  The returned object must be deleted
     * using the Delete() method.  If the Structure is not creatable,
     * this method must not be called.
     *
     * @return a newly-constructed object of this Structure type; the
     *         default constructor is used.
     **/
    virtual void *Create() const = 0;

    /**
     * Returns a newly-constructed array of objects of this Structure type;
     * the default array constructor is used.  The returned array must be
     * deleted using the DeleteArray() method.  If the Structure is not
     * creatable, this method must not be called.
     *
     * @param count is the number of elements in the array to be created
     * @return a newly-created array of objects of this Structure type; the
     *         default constructor is used.
     **/
    virtual void *CreateArray(u32 count) const = 0;

    /**
     * Returns true if the Structure can be deleted.  Only if this method
     * returns true may the Delete() method or DeleteArray() methods be
     * called.
     *
     * @return true if the Structure can be deleted.  Only if this method
     *              returns true may the Delete() method or DeleteArray()
     *              methods be called.
     **/
    virtual bool IsDeletable() const = 0;

    /**
     * Deletes an object created by the Create() method.
     *
     * @param pInstance is the object to be deleted.
     **/
    virtual void Delete(void *pInstance) const = 0;

    /**
     * Deletes an array of objects created by the CreateArray() method.
     *
     * @param pInstanceArray is the array of objects to be deleted.
     **/
    virtual void DeleteArray(void *pInstanceArray) const = 0;
};


/** **************************************************************************
 * Base describes a super class (or super struct or super union) of a
 * class, struct, or union.  It gives both the Structure that is the base
 * class, and also the offset from the sub class to the super class; this
 * is the value which, when added to a pointer to a sub class, yields a
 * pointer to the base class.
 ************************************************************************** **/
class Base
{
public:

    /**
     * Destructor
     **/
    virtual ~Base() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Base &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Base &other) const
    { return !(*this == other); }

    /**
     * Returns the access privileges granted to the subclass by this base
     * class.
     *
     * @return the access privileges granted to the subclass by this base
     *         class.
     **/
    virtual AccessType GetAccessType() const = 0;

    /**
     * Returns true if the subclass' inheritence via this base was virtual,
     * false if not.
     *
     * @return true if the subclass' inheritence via this base was virtual,
     *         false if not.
     **/
    virtual bool IsVirtual() const = 0;

    /**
     * Returns the structure which is the base class/struct/union.
     *
     * @return the structure which is the base class/struct/union.
     **/
    virtual const Structure &GetStructure() const = 0;
    
    /**
     * Returns true if this Base is "castable", meaning that the CastSubclass
     * method of this class may be called.  If this method returns false,
     * then the CastSubclass method must NOT be called.
     *
     * @return true if this Base is "accessible", meaning that the
     *         CastSubclass method of this class may be called
     **/
    virtual bool IsCastable() const = 0;

    /**
     * Casts an instance of the immediate subclass/substruct, given by
     * pObject, into an instance of the class/struct represented by this
     * Base object.  NOTE that pObject MUST be of the type described by the
     * Structure object of which this Base class represents a base
     * class/struct.  Therefore the only proper use of this method is
     * something like:
     *
     * void *pObject;
     * Structure *pStruct;   // Type of pObject
     *
     * void *pBase = (pStruct->GetBase(0)).CastSubclass(pObject);
     * // The type of pBase is described by pStruct->GetBase(0).GetStructure()
     *
     * @param pObject is an instance of the base class from which this Base
     *        object was obtained; pObject will be cast to an object of the
     *        type described by GetStructure()
     * @return pObject cast to an instance of the base class described by this
     *         Base object
     **/
    virtual void *CastSubclass(void *pObject) const = 0;
};


/** **************************************************************************
 * A Union describes a C++ union.  It has no additional fields beyond that of
 * a Structure, but each of its Field's space is shared with all other Fields;
 * thus, only one Field of the union may be used without interfering with the
 * others.
 ************************************************************************** **/
class Union : public Structure
{
public:

    /**
     * Destructor
     **/
    virtual ~Union() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Context &other) const
    {
        if (other.GetType() != Type_Union) {
            return false;
        }

        return (*this == (const Union &) other);
    }

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Context &other) const
    { return !(*this == other); }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Union &other) const
    { return (((const Structure *) this)->Structure::operator == 
              ((const Structure &) other)); }

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Union &other) const
    { return !(*this == other); }

    /** 
     * Returns the type of this Context, which is Type_Union.
     * 
     * @return the type of this Context, which is Type_Union.
     **/
    virtual Type GetType() const
    {
        return Type_Union;
    }
};


/** **************************************************************************
 * A Struct represents a C++ struct.  It is a Structure with Methods.
 ************************************************************************** **/
class Struct : public Structure
{
public:

    /**
     * Destructor
     **/
    virtual ~Struct() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Context &other) const
    {
        if (other.GetType() != Type_Struct) {
            return false;
        }

        return (*this == (const Struct &) other);
    }

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Context &other) const
    { return !(*this == other); }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Struct &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Struct &other) const
    { return !(*this == other); }

    /** 
     * Returns the type of this Context, which is Type_Struct.
     * 
     * @return the type of this Context, which is Type_Struct.
     **/
    virtual Type GetType() const
    {
        return Type_Struct;
    }

    /**
     * Returns true if this Structure is abstract (defines at least one
     * pure virtual method), false if not.
     *
     * @return true if this Structure is abstract (defines at least one
     *         pure virtual method), false if not.
     **/
    virtual bool IsAbstract() const = 0;

    /**
     * Returns the number of Methods of this Struct.
     *
     * @return the number of Methods of this Struct.
     **/
    virtual u32 GetMethodCount() const = 0;

    /**
     * Returns a Method of this Struct.
     *
     * @param index is the number of the Method to return
     * @return a Method of this Struct.
     **/
    virtual const Method &GetMethod(u32 index) const = 0;
};


/** **************************************************************************
 * A Class represents a C++ class.  It is no different from a Struct.
 ************************************************************************** **/
class Class : public Struct
{
public:

    /**
     * Destructor
     **/
    virtual ~Class() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Context &other) const
    {
        if (other.GetType() != Type_Class) {
            return false;
        }

        return (*this == (const Class &) other);
    }

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Context &other) const
    { return !(*this == other); }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Class &other) const
    { return (((const Struct *) this)->Struct::operator == 
              ((const Struct &) other)); }

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Class &other) const
    { return !(*this == other); }

    /** 
     * Returns the type of this Context, which is Type_Class.
     * 
     * @return the type of this Context, which is Type_Class.
     **/
    virtual Type GetType() const
    {
        return Type_Class;
    }
};


/** **************************************************************************
 * Member is the base class of all members of a Structure, including
 * Constructor, Method, and Field.
 ************************************************************************** **/
class Member
{
public:

    /**
     * Destructor
     **/
    virtual ~Member() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Member &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Member &other) const
    { return !(*this == other); }

    /**
     * Returns the access type of the member.
     *
     * @return the access type of the member.
     **/
    virtual AccessType GetAccessType() const = 0;

    /**
     * Returns the Context in which the Member is declared; it is the Context
     * which "owns" the Member and in which the Member lives.
     *
     * @return the Context in which the Member is declared
     **/
    virtual const Context &GetContext() const = 0;

    /**
     * Returns the name of this member.
     *
     * @return the name of this member.
     **/
    virtual const char *GetName() const = 0;

    /**
     * Returns true if the Member is static in scope, false if it is not.
     *
     * @return true if the Member is static in scope, false if it is not.
     **/
    virtual bool IsStatic() const = 0;
};


/** **************************************************************************
 * Field represents a C++ field (i.e. member variable).
 ************************************************************************** **/
class Field : public Member
{
public:

    /**
     * Destructor
     **/
    virtual ~Field() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Field &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Field &other) const
    { return !(*this == other); }

    /**
     * Returns the type of the Field.
     *
     * @return the type of the Field.
     **/
    virtual const Type &GetType() const = 0;

    /**
     * If the field was declared as a bitfield, then this will return the
     * number of bits in the bitfield.
     *
     * @return the number of bits in the bitfield if the field was declared
     *         as a bitfield
     **/
    virtual u32 GetBitfieldBitCount() const = 0;
    
    /**
     * Returns true if the offset of this field within its containing
     * structure is available, and can be obtained by calling GetOffset().
     *
     * @return true if the offset of this field within its containing
     *         structure is available, and can be obtained by calling
     *         GetOffset().
     **/
    virtual bool HasOffset() const = 0;

    /**
     * Returns the offset in bytes from the beginning of the containing
     * Structure to this Field.  This field's value is undefined for all
     * static Fields, and for all bitfield Fields.  NOTE that this method
     * returns 0 for Fields of anonymous Structures.
     *
     * @return the offset in bytes from the beginning of the containing
     *         Structure to this Field, or 0 if the Field is a field of an
     *         anonymous Structure
     **/
    virtual u32 GetOffset() const = 0;

    /**
     * Returns true if this Field is "accessible", meaning that the Get method
     * of this class may be called.  If this method returns false, then the
     * Get methods must NOT be called.
     *
     * @return true if this Field is "accessible", meaning that the Get method
     *         of this class may be called
     **/
    virtual bool IsAccessible() const = 0;

    /**
     * Returns a pointer to the Field within the given instance object.
     * Returns NULL if the containing object is anonymous.  The value of
     * the field can then be obtained by casting this pointer to the
     * appropriate C++ type and dereferencing the pointer.  The value of
     * the field can also be changed in this way.
     *
     * @param pInstance is an object of the type of the context containing
     *        this field.  NULL may be passed in if the field is static.
     * @return a pointer to the field within the instance object.  This
     *         pointer may be cast to a pointer of the type appropriate for
     *         this field and then dereferenced to get the value of the field
     *         or assigned to change the value of the field.
     **/
    virtual void *Get(void *pInstance) const = 0;
};


/** **************************************************************************
 * Argument represents a single argument in a constructor or function
 * signature.
 ************************************************************************** **/
class Argument
{
public:

    /**
     * Destructor
     **/
    virtual ~Argument() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Argument &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Argument &other) const
    { return !(*this == other); }

    /**
     * Returns the type of the argument.
     *
     * @return the type of the argument.
     **/
    virtual const Type &GetType() const = 0;

    /**
     * Returns true if the Argument has a default value, false if not.
     *
     * @return true if the Argument has a default value, false if not.
     **/
    virtual bool HasDefault() const = 0;

    /**
     * This returns a string representation of the default value for this
     * Argument.  This is the exact text as it would have appeared in the
     * original source code defining the default value for this Argument
     *
     * @return a string representation of the default value for this
     *         Argument.
     **/
    virtual const char *GetDefault() const = 0;
};


/** **************************************************************************
 * DestructorSignature describes the signature of a destructor
 ************************************************************************** **/
class DestructorSignature
{
public:

    /**
     * Destructor
     **/
    virtual ~DestructorSignature() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const DestructorSignature &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const DestructorSignature &other) const
    { return !(*this == other); }

    /**
     * Returns the number of types which this destructor declares that it
     * throws as exceptions.
     *
     * @return the number of types which this destructor declares that it
     *         throws as exceptions.
     **/
    virtual u32 GetThrowCount() const = 0;

    /**
     * Returns a type which this destructor declares that it throws as an
     * exception.
     *
     * @param index is the index of the throw type to return
     * @return a type which this destructor declares that it throws as an
     *         exception.
     **/
    virtual const Type &GetThrow(u32 index) const = 0;
};


/** **************************************************************************
 * ConstructorSignature describes the signature of a constructor
 ************************************************************************** **/
class ConstructorSignature : public DestructorSignature
{
public:

    /**
     * Destructor
     **/
    virtual ~ConstructorSignature() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const ConstructorSignature &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const ConstructorSignature &other) const
    { return !(*this == other); }

    /**
     * Returns the number of arguments of the constructor.
     *
     * @return the number of arguments of the constructor.
     **/
    virtual u32 GetArgumentCount() const = 0;

    /**
     * Returns the argument at index [index].
     *
     * @return the argument at index [index].
     **/
    virtual const Argument &GetArgument(u32 index) const = 0;

    /**
     * Returns true if the constructor has ellipsis - an undefined number of
     * arguments following the final declared argument, or false if it
     * does not.
     *
     * @return true if the constructor has ellipsis
     **/
    virtual bool HasEllipsis() const = 0;
};


/** **************************************************************************
 * MethodSignature describes the signature of a function
 ************************************************************************** **/
class MethodSignature : public ConstructorSignature
{
public:

    /**
     * Destructor
     **/
    virtual ~MethodSignature() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const MethodSignature &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const MethodSignature &other) const
    { return !(*this == other); }

    /**
     * Returns the return type of the function.
     *
     * @return the return type of the function.
     **/
    virtual const Type &GetReturnType() const = 0;
};


/** **************************************************************************
 * Destructor represents a C++ destructor
 ************************************************************************** **/
class Destructor : public Member
{
public:

    /**
     * Destructor
     **/
    virtual ~Destructor() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Destructor &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Destructor &other) const
    { return !(*this == other); }

    /**
     * Returns true if the destructor is virtual, false if it is concrete.
     *
     * @return true if the destructor is virtual, false if it is concrete.
     **/
    virtual bool IsVirtual() const = 0;

    /**
     * Returns true if the destructor is pure virtual, false if it is not.
     *
     * @return true if the destructor is pure virtual, false if it is not.
     **/
    virtual bool IsPureVirtual() const = 0;

    /**
     * Returns an object describing the signature of this destructor.
     *
     * @return an object describing the signature of this destructor.
     **/
    virtual const DestructorSignature &GetSignature() const = 0;

    /**
     * Returns true if this method is invokeable.  Only if this method returns
     * true may the Invoke() method be called.
     *
     * @return true if this method is invokeable.  Only if this method returns
     *         true may the Invoke() method be called.
     **/
    virtual bool IsInvokeable() const = 0;

    /**
     * Invokes this destructor on a constructed object.
     * constructed instance must be deleted by the caller.  If the containing
     * Structure is anonymous, this method will return NULL, since anonymous
     * Structures cannot be instantiated.
     *
     * @param pInstance is the instance of the object to destroy
     **/
    virtual void Invoke(void *pInstance) const = 0;
};


/** **************************************************************************
 * Constructor represents a C++ constructor.
 ************************************************************************** **/
class Constructor : public Member
{
public:

    /**
     * Destructor
     **/
    virtual ~Constructor() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Constructor &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Constructor &other) const
    { return !(*this == other); }

    /**
     * Returns an object describing the signature of this constructor.
     *
     * @return an object describing the signature of this constructor.
     **/
    virtual const ConstructorSignature &GetSignature() const = 0;

    /**
     * Additionally, a Constructor gives names to its arguments.  This returns
     * the name of argument at index [index].
     *
     * @return the name of argument at index [index]
     **/
    virtual const char *GetArgumentName(u32 index) const = 0;

    /**
     * Returns true if this method is invokeable.  Only if this method returns
     * true may the Invoke() method be called.
     *
     * @return true if this method is invokeable.  Only if this method returns
     *         true may the Invoke() method be called.
     **/
    virtual bool IsInvokeable() const = 0;

    /**
     * Invokes this constructor and returns the constructed instance.  The
     * constructed instance must be deleted by the caller.  If the containing
     * Structure is anonymous, this method will return NULL, since anonymous
     * Structures cannot be instantiated.
     *
     * @param pArgumentValues is an array of pointers to the arguments to
     *        pass to this constructor.  The number of pointers in pArguments
     *        must match the number of arguments to this constructor.
     * @return a newly-constructed instance of the containing Structure; or
     *         NULL if the containing Structure is anonymous
     **/
    virtual void *Invoke(void **pArgumentValues) const = 0;
};


/** **************************************************************************
 * Method represents a C++ method.  
 ************************************************************************** **/
class Method : public Member
{
public:

    /**
     * Destructor
     **/
    virtual ~Method() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Method &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Method &other) const
    { return !(*this == other); }

    /**
     * Returns true if the method is an operator method, false if not.
     *
     * @return true if the method is an operator method, false if not.
     **/
    virtual bool IsOperatorMethod() const = 0;

    /**
     * Returns true if the method is const, false if it is not.
     *
     * @return true if the method is const, false if it is not.
     **/
    virtual bool IsConst() const = 0;

    /**
     * Returns true if the method is virtual, false if it is concrete.
     *
     * @return true if the method is virtual, false if it is concrete.
     **/
    virtual bool IsVirtual() const = 0;

    /**
     * Returns true if the method is pure virtual, false if it is not.
     *
     * @return true if the method is pure virtual, false if it is not.
     **/
    virtual bool IsPureVirtual() const = 0;

    /**
     * Returns an object describing the signature of this method.
     *
     * @return an object describing the signature of this method.
     **/
    virtual const MethodSignature &GetSignature() const = 0;

    /**
     * Additionally, a Method gives names to its arguments.  This returns
     * the name of argument at index [index].
     *
     * @return the name of argument at index [index]
     **/
    virtual const char *GetArgumentName(u32 index) const = 0;

    /**
     * Returns true if this method is invokeable.  Only if this method returns
     * true may the Invoke() method be called.
     *
     * @return true if this method is invokeable.  Only if this method returns
     *         true may the Invoke() method be called.
     **/
    virtual bool IsInvokeable() const = 0;

    /**
     * Invokes this method on an instance of the class represented by the
     * Class containing this Method.  It is an error to attempt to invoke
     * a pure virtual method.  This method has no effect if the containing
     * Structure is anonymous, since methods cannot be called on anonymous
     * structures.
     *
     * @param pInstance is the instance of the class to invoke this method on
     * @param pReturnValue is a pointer to the memory in which the return
     *        value resulting from the invocation of the method will be
     *        stored.  This pointer must be to a type which matches the return
     *        type of the method.  If the method has no return value, then
     *        this argument is ignored.
     * @param pArgumentValues is an array of pointers to the arguments to
     *        pass to  this method.  The number of pointers in pArguments
     *        must match the number of arguments to this method.
     **/
    virtual void Invoke(void *pInstance, void *pReturnValue, 
                        void **pArgumentValues) const = 0;
};


/** **************************************************************************
 * Type represents a C++ type.  Each Type may have a number of Pointers,
 * each of which describes the properties of one level of pointer indirection
 * of the Type, and a number of Arrays, each of which describes the properties
 * of one array dimension.  For example, the C++ type:
 *
 * const * volatile * int[5][];
 *
 * ... would have two Pointers (the first one volatile, the second const), and
 * two Arrays (the first one with element count 5, the second with element
 * count unbounded).
 ************************************************************************** **/
class Type
{
public:
    
    /**
     * Destructor
     **/
    virtual ~Type() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Type &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Type &other) const
    { return !(*this == other); }

    /** **********************************************************************
     * These are the C++ types which may be represented by a Type object.
     *
     * If the Type of this Type is BaseType_Enumeration, then this Type is
     * actually a TypeEnumeration object.
     *
     * If the Type of this Type is BaseType_Function, then this Type is
     * actually a TypeFunction object.
     *
     * If the Type of this Type is BaseType_Structure, then this Type is
     * actually a TypeStructure object.
     *
     * Otherwise, the Type object is not subclassed.
     ********************************************************************** **/
    enum BaseType
    {
        BaseType_Void                    = 0,
        BaseType_Bool                    = 1,
        BaseType_Char                    = 2,
        BaseType_Unsigned_Char           = 3,
        BaseType_WChar                   = 4,
        BaseType_Short                   = 5,
        BaseType_Unsigned_Short          = 6,
        BaseType_Int                     = 7,
        BaseType_Unsigned_Int            = 8,
        BaseType_Long                    = 9,
        BaseType_Unsigned_Long           = 10,
        BaseType_Long_Long               = 11,
        BaseType_Unsigned_Long_Long      = 12,
        BaseType_Float                   = 13,
        BaseType_Double                  = 14,
        BaseType_Long_Double             = 15,
        BaseType_Enumeration             = 16,
        BaseType_Function                = 17,
        BaseType_Structure               = 18
    };

    /**
     * Returns the C++ type of this Type.
     *
     * @return the C++ type of this Type.
     **/
    virtual BaseType GetBaseType() const = 0;

    /**
     * If this returns true, then the C++ type represented by this Type
     * is const.  If this returns false, then it is not.
     *
     * @return true if C++ type represented by this Type is const
     **/
    virtual bool IsConst() const = 0;

    /**
     * If this returns true, then the C++ type represented by this Type
     * is volatile.  If this returns false, then it is not.
     *
     * @return true if C++ type represented by this Type is volatile
     **/
    virtual bool IsVolatile() const = 0;

    /**
     * If this returns true, then the C++ type represented by this Type
     * is a C++ reference.  If this returns false, then it is not.
     *
     * @return true if C++ type represented by this Type is a C++ reference
     **/
    virtual bool IsReference() const = 0;

    /**
     * This class has an ordered list of ArrayOrPointer objects each of
     * which describes how the heck to explain this???
     *
     * @return the number of ArrayOrPointer objects for this Type.
     **/
    virtual u32 GetArrayOrPointerCount() const = 0;

    /**
     * Returns the ArrayOrPointer object at the given index.
     *
     * @param index is the index of the ArrayOrPointer object to return.
     * @return the ArrayOrPointer object at the given index.
     **/
    virtual const ArrayOrPointer &GetArrayOrPointer(u32 index) const = 0;
};


/** **************************************************************************
 * ArrayOrPointer is either an Array or a Pointer.
 ************************************************************************** **/
class ArrayOrPointer
{
public:

    /** **********************************************************************
     * This enumerates the valid subtypes
     ********************************************************************** **/
    enum Type
    {
		Type_Array       = 0,
		Type_Pointer     = 1
	};

    virtual ~ArrayOrPointer() { }

    /** 
     * Returns the type of this ArrayOrPointer.  The most derived class of
     * this object is determined by the value returned here:
     *
     * Type_Array: most derived type is Array <br>
     * Type_Pointer: most derived type is Pointer <br>
     *
     * @return the type of this context
     **/
    virtual Type GetType() const = 0;
};


/** **************************************************************************
 * Array describes the element count of a single array dimension of a Type.
 ************************************************************************** **/
class Array : public ArrayOrPointer
{
public:

    /**
     * Destructor
     **/
    virtual ~Array() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const ArrayOrPointer &other) const
    {
        if (other.GetType() != Type_Array) {
            return false;
        }

        return (*this == (const Array &) other);
    }

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const ArrayOrPointer &other) const
    { return !(*this == other); }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Array &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Array &other) const
    { return !(*this == other); }

    /** 
     * Returns the type of this ArrayOrPointer, which is Type_Array.
     * 
     * @return the type of this ArrayOrPointer, which is Type_Array.
     **/
    virtual Type GetType() const
    {
        return Type_Array;
    }

    /**
     * If this returns true, the array dimension is of unbounded size; it may
     * have any element count.  If this returns false, the element count is
     * fixed and specified, and can be obtained by calling GetElementCount().
     *
     * @return true if the array dimension is of unbounded size
     **/
    virtual bool IsUnbounded() const = 0;

    /**
     * If the array dimension is not unbounded, then this specifies the
     * element count of the array dimension.  The index of the last element of
     * the of the array is at this value minus one.
     *
     * @return the element count of the array dimension
     **/
    virtual u32 GetElementCount() const  = 0;
};


/** **************************************************************************
 * Pointer describes the dereference characteristics of a single pointer depth
 *  of a Type.
 ************************************************************************** **/
class Pointer : public ArrayOrPointer
{
public:

    /**
     * Destructor
     **/
    virtual ~Pointer() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const ArrayOrPointer &other) const
    {
        if (other.GetType() != Type_Pointer) {
            return false;
        }

        return (*this == (const Pointer &) other);
    }

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const ArrayOrPointer &other) const
    { return !(*this == other); }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Pointer &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Pointer &other) const
    { return !(*this == other); }

    /** 
     * Returns the type of this ArrayOrPointer, which is Type_Pointer.
     * 
     * @return the type of this ArrayOrPointer, which is Type_Pointer.
     **/
    virtual Type GetType() const
    {
        return Type_Pointer;
    }

    /**
     * If this returns true, then the dereferenced value is const.  If this
     * returns false, it is not.
     *
     * @return true if the dereferenced value is const
     **/
    virtual bool IsConst() const = 0;

    /**
     * If this returns true then the dereferenced value is volatile.  If this
     * returns false, it is not.
     *
     * @return true if the dereferenced value is volatile
     **/
    virtual bool IsVolatile() const = 0;
};


/** **************************************************************************
 * Enumeration describes a C++ enum.
 ************************************************************************** **/
class Enumeration
{
public:

    /**
     * Destructor
     **/
    virtual ~Enumeration() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const Enumeration &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const Enumeration &other) const
    { return !(*this == other); }

    /**
     * Returns the access type of the Enuimeration within the context in which
     * it is defined.
     *
     * @return the access type of the Enumeration within the context in which
     *         it is defined.
     **/
    virtual AccessType GetAccessType() const = 0;

    /**
     * Returns the Context in which this Enumeration is defined.
     *
     * @return the Context in which this Enumeration is defined
     **/
    virtual const Context &GetContext() const = 0;

    /**
     * Returns the name of this Enumeration.
     *
     * @return the name of this Enumeration
     **/
    virtual const char *GetName() const = 0;
    
    /**
     * Returns the number of EnumerationValues of this Enumeration.
     *
     * @return the number of EnumerationValues of this Enumeration.
     **/
    virtual u32 GetValueCount() const = 0;

    /**
     * Returns an EnumerationValue of this Enumeration.
     *
     * @param index is the number of the EnumerationValue to return
     * @return an EnumerationValue of this Enumeration
     **/
    virtual const EnumerationValue &GetValue(u32 index) const = 0;
};


/** **************************************************************************
 * EnumerationValue represents a single value of a C++ enum.
 ************************************************************************** **/
class EnumerationValue
{
public:

    /**
     * Destructor
     **/
    virtual ~EnumerationValue() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const EnumerationValue &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const EnumerationValue &other) const
    { return !(*this == other); }

    /**
     * Returns the name of the value.
     *
     * @return the name of the value.
     **/
    virtual const char *GetName() const = 0;

    /**
     * Returns the numerical const value of the value.
     *
     * @return the numerical const value of the value
     **/
    virtual s32 GetValue() const = 0;
};


/** **************************************************************************
 * TypeEnumeration represents a C++ type that is an enum.
 ************************************************************************** **/
class TypeEnumeration : public Type
{
public:

    /**
     * Destructor
     **/
    virtual ~TypeEnumeration() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const TypeEnumeration &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const TypeEnumeration &other) const
    { return !(*this == other); }

    /**
     * This returns the Enumeration of the TypeEnumeration, which describes
     * the set of allowed enumerated values that values of this type may
     * have.
     *
     * @return the set of allowed enumerated values for this type
     **/
    virtual const Enumeration &GetEnumeration() const = 0;
};


/** **************************************************************************
 * TypeEnumeration represents a C++ type that is a function.
 ************************************************************************** **/
class TypeFunction : public Type
{
public:

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const TypeFunction &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const TypeFunction &other) const
    { return !(*this == other); }

    /**
     * Returns an object describing the signature of the function type.
     *
     * @return an object describing the signature of the function type.
     **/
    virtual const MethodSignature &GetSignature() const = 0;
};


/** **************************************************************************
 * TypeStructure represents a C++ type which is a structure (class, struct,
 * or union).
 ************************************************************************** **/
class TypeStructure : public Type
{
public:

    /**
     * Destructor
     **/
    virtual ~TypeStructure() { }

    /**
     * Equality operator - returns true if this object is "functionally
     * equivalent" to [other].  The two objects don't have to be exactly
     * identical, but do have to be identical in every meaningful way.
     * Note also that this doesn't test for equality of any base class
     * members; it is a test only at this level of the inheritence hierarcy.
     * To test if an object is completely equals, including for all of its
     * subclasses defined in the Xrtti hierarchy, you must cast this class
     * to its most derived class and call that method.
     *
     * @param other is the other object to compare against
     * @return true if this object is functionally equal to the other, false
     *         if not
     **/
    bool operator ==(const TypeStructure &other) const;

    /**
     * Inequality operator - returns false when the == operator would 
     * return true, and vice versa.
     *
     * @param other is the other object to compare against
     * @return false when the == operator would return true, and vice versa.
     **/
    bool operator !=(const TypeStructure &other) const
    { return !(*this == other); }

    /**
     * Returns the Structure defining the type.
     *
     * @return the Structure defining the type.
     **/
    virtual const Structure &GetStructure() const = 0;
};


/** **************************************************************************
 * Utility helper functions
 ************************************************************************** **/

/**
 * The Context equality operator (operator ==) only tests fields of
 * Context for equality; since Context is only a base class and the
 * specific subclass of the instance has many more fields, a test of
 * equality of a Context is not sufficient to demonstrate that the actual
 * objects are equal.  The Equals method tests the most specific subclass
 * of the Context for equality, which means testing the entirety of the
 * object.  This is the correct method to use for testing equality between
 * two arbitrary Contexts.
 *
 * @param c1 is the first Context to test for equality (against c2)
 * @param c2 is the second Context to test for equality (against c1)
 * @return true if the two Contexts are equals, meaning that they have
 *         equivalent meaning (there may be minor inconsequential
 *         differences between the Contexts, such as non-virtual methods
 *         being in different orders, that do not affect this equality
 *         test)
 **/
bool Equals(const Context &c1, const Context &c2);

/**
 * The Type equality operator (operator ==) only tests fields of Type for
 * equality; since Type is only a base class and the specific subclass of
 * the instance has many more fields, a test of equality of a Type is not
 * sufficient to demonstrate that the actual objects are equal.  The
 * Equals method tests the most specific subclass of the Type for
 * equality, which means testing the entirety of the object.  This is the
 * correct method to use for testing equality between two arbitrary Types.
 *
 * @param type1 is the first Type to test for equality (against c2)
 * @param type2 is the second Type to test for equality (against c1)
 * @return true if the two Types are equals, meaning that they have
 *         equivalent meaning (there may be minor inconsequential
 *         differences between the Types, such as non-virtual methods
 *         being in different orders, that do not affect this equality
 *         test)
 **/
bool Equals(const Type &type1, const Type &type2);


/** **************************************************************************
 * Xrtti API
 ************************************************************************** **/


/**
 * Returns the number of compiled Contexts - these are the Contexts defining
 * classes compiled into the running application, or into any library linked
 * into the currently running application.
 * 
 * @return the number of compiled Contexts.
 **/
u32 GetContextCount();

/**
 * Returns a compiled Context.  The total number of available compiled
 * contexts is available via a call to GetCompiledContextCount().
 *
 * @param index is the index of the compiled Context to get; this value must
 *        be less than the number of Contexts returned by
 *        GetCompiledContextCount().
 *        @return the index'th compiled Context.
 **/
const Context *GetContext(u32 index);

/**
 * Looks up a compiled Context by full name, and returns it.  Returns NULL if
 * there is no such registered compiled Context.
 *
 * @param pFullName is the full name (i.e. foo::bar::baz for C++, or just the
 *        struct/union name for C) of the compiled Context to look up and
 *        return
 * @return the compiled Context with the given full name, or NULL if there is
 *         no such compiled Context
 **/
const Context *LookupContext(const char *pFullName);

/**
 * Looks up a Structure by C++ RTTI type_info, and returns it.  This only
 * works if the Structure was generated by xrttigen with the -r option
 * enabled.  Returns NULL if there is no such registered Structure.
 *
 * @param typeinfo is the C++ RTTI type_info to fetch the corresponding
 *        Structure for.  Only works if xrttigen had the -r option specified.
 * @return the Structure with the given type_info, or NULL if there is no
 *         such Structure
 **/
const Structure *LookupStructure(const std::type_info &typeinfo);

}; // namespace Xrtti


#endif // XRTTI_H
