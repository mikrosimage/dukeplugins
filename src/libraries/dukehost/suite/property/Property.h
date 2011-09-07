/*
 * Property.h
 *
 *  Created on: 28 sept. 2010
 *      Author: Guillaume Chatelet
 *
 * Implementation of a Property
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <vector>
#include <string>
#include <cstdarg>

namespace openfx {
namespace host {

enum EPropertyType {
    eNone = -1, eInt = 0, eDouble = 1, eString = 2, ePointer = 3
};

/**
 * type traits declaration to infer template
 * Parameter's type at compile time
 */
template<typename TYPE>
struct TypeTraits {
    const static EPropertyType type = eNone;
};
template<>
struct TypeTraits<int> {
    const static EPropertyType type = eInt;
};
template<>
struct TypeTraits<void*> {
    const static EPropertyType type = ePointer;
};
template<>
struct TypeTraits<std::string> {
    const static EPropertyType type = eString;
};
template<>
struct TypeTraits<double> {
    const static EPropertyType type = eDouble;
};

/**
 * Type erasure to allow concrete template instantiations to
 * be manipulated through pointers and in a uniform way
 *
 * Please note :
 *
 * > The type variable allow the dynamic_cast to the concrete type to
 * access data in a type safe way.
 *
 * > No data are exposed in this interface to prevent manipulating
 * raw pointers.
 *
 * > the members are public on purpose it prevents
 * a lot of 'friend' declaration boilerplate, especially
 * when implementing global functions as ostream::operator<<
 */
struct Property {
    const EPropertyType type;
    const std::string name;

    Property(EPropertyType type, const std::string &name) :
        type(type), name(name) {
    }

    virtual ~Property() {
    }

    virtual size_t size() const = 0;

    virtual void reset() = 0;

    virtual Property* clone() const = 0;

    virtual bool operator ==(const Property& b) const = 0;
    virtual bool operator !=(const Property& b) const = 0;
};

/**
 * Implementation of the concrete types using templates
 *
 * Please note :
 *
 * > the members are public on purpose it prevents
 * a lot of 'friend' declaration boilerplate, especially
 * when implementing global functions as ostream::operator<<
 *
 */
template<typename TYPE>
struct TProperty : public Property {
    typedef std::vector<TYPE> ValueType;
    ValueType defaultValue;
    ValueType value;

    /**
     * you have to provide values and defaultValues
     * yourself using this constructor
     */
    TProperty(const std::string& name) :
        Property(TypeTraits<TYPE>::type, name) {
    }

    /**
     * constructor with pre-initialized value and/or defaultvalue
     */
    TProperty(const std::string& name, TYPE oneValue, TYPE oneDefaultValue = TYPE()) :
        Property(TypeTraits<TYPE>::type, name) {
        value.push_back(oneValue);
        defaultValue.push_back(oneDefaultValue);
    }

    /**
     * constructor with pre-initialized value and/or defaultvalue
     */
    TProperty(const std::string& name, const ValueType &_values, const ValueType &_defaultValues = ValueType()) :
        Property(TypeTraits<TYPE>::type, name) {
        // we must enforce the fact values and defaultValues have the same size
        // as a call to reset would change the property's size
        assert(_values.size() == _defaultValues.size());
        value = _values;
        defaultValue = _defaultValues;
    }

    virtual ~TProperty() {
    }

    virtual size_t size() const {
        return value.size();
    }

    virtual void reset() {
        value = defaultValue;
    }

    virtual Property* clone() const {
        return new TProperty<TYPE> (*this);
    }

    virtual bool operator ==(const Property& b) const {
        return !this->operator !=(b);
    }

    virtual bool operator !=(const Property& b) const {
        if (type != b.type)
            return true;
        const TProperty<TYPE>& other = dynamic_cast<const TProperty<TYPE>&> (b);
        return name != other.name || value != other.value || defaultValue != other.defaultValue;
    }
};

/**
 * Actual declaration of common concrete template types
 */
typedef TProperty<int> IntProperty;
typedef TProperty<double> DoubleProperty;
typedef TProperty<std::string> StringProperty;
typedef TProperty<void*> PointerProperty;

} // namespace host
} // namespace openfx

#endif /* PROPERTY_H_ */
