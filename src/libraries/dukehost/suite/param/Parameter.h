/*
 * Parameter.h
 *
 *  Created on: Oct 9, 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <openfx/ofxCore.h>
#include <dukehost/suite/property/PropertySet.h>

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <string>
#include <map>

// linking our type with OpenFx blind Type
struct OfxParamStruct {
};

namespace openfx {
namespace host {

enum EParameterType {
    eNoneParam,
    eStringParam,
    eIntParam,
    eInt2DParam,
    eInt3DParam,
    eDoubleParam,
    eDouble2DParam,
    eDouble3DParam,
    eRGBParam,
    eRGBAParam,
    eBooleanParam,
    eChoiceParam,
    eCustomParam,
    eGroupParam,
    ePageParam,
    ePushButtonParam
};

struct ParameterValue {
    const EParameterType type;
    ParameterValue(EParameterType type) :
        type(type) {
    }
    virtual ~ParameterValue() {
    }
    virtual ParameterValue* clone() const = 0;
    virtual bool operator ==(const ParameterValue& b) const = 0;
    virtual bool operator !=(const ParameterValue& b) const = 0;
};

typedef boost::tuples::tuple<std::string> StringType;
typedef boost::tuples::tuple<int> IntType;
typedef boost::tuples::tuple<int, int> Int2DType;
typedef boost::tuples::tuple<int, int, int> Int3DType;
typedef boost::tuples::tuple<double> DoubleType;
typedef boost::tuples::tuple<double, double> Double2DType;
typedef boost::tuples::tuple<double, double, double> Double3DType;
typedef boost::tuples::tuple<bool> BooleanType;

namespace details {
template<typename TYPE>
struct ParameterTraits {
    const static EParameterType type = eNoneParam;
};
template<>
struct ParameterTraits<StringType> {
    const static EParameterType type = eStringParam;
};
template<>
struct ParameterTraits<IntType> {
    const static EParameterType type = eIntParam;
};
template<>
struct ParameterTraits<Int2DType> {
    const static EParameterType type = eInt2DParam;
};
template<>
struct ParameterTraits<Int3DType> {
    const static EParameterType type = eInt3DParam;
};
template<>
struct ParameterTraits<DoubleType> {
    const static EParameterType type = eDoubleParam;
};
template<>
struct ParameterTraits<Double2DType> {
    const static EParameterType type = eDouble2DParam;
};
template<>
struct ParameterTraits<Double3DType> {
    const static EParameterType type = eDouble3DParam;
};
template<>
struct ParameterTraits<BooleanType> {
    const static EParameterType type = eBooleanParam;
};

} // namespace details


template<typename TYPE>
struct TParameterValue : public ParameterValue {
    TYPE value;

    TParameterValue() :
        ParameterValue(details::ParameterTraits<TYPE>::type), value() {
    }
    TParameterValue(const TYPE& other) :
        ParameterValue(details::ParameterTraits<TYPE>::type), value(other) {
    }
    template<typename T0>
    TParameterValue(const T0 &v0) :
        ParameterValue(details::ParameterTraits<TYPE>::type), value(v0) {
    }
    template<typename T0, typename T1>
    TParameterValue(const T0 &v0, const T1 &v1) :
        ParameterValue(details::ParameterTraits<TYPE>::type), value(v0, v1) {
    }
    template<typename T0, typename T1, typename T2>
    TParameterValue(const T0 &v0, const T1 &v1, const T2 &v2) :
        ParameterValue(details::ParameterTraits<TYPE>::type), value(v0, v1, v2) {
    }
    template<typename T0, typename T1, typename T2, typename T3>
    TParameterValue(const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3) :
        ParameterValue(details::ParameterTraits<TYPE>::type), value(v0, v1, v2, v3) {
    }
    virtual ~TParameterValue() {
    }
    virtual TParameterValue<TYPE>* clone() const {
        return new TParameterValue<TYPE> (*this);
    }
    virtual bool operator ==(const ParameterValue& b) const {
        return !this->operator !=(b);
    }
    virtual bool operator !=(const ParameterValue& b) const {
        if (type != b.type)
            return true;
        const TParameterValue<TYPE>* pOther = dynamic_cast<const TParameterValue<TYPE>*> (&b);
        assert(pOther); // we are the same so dynamic_cast should always return non NULL
        return value != pOther->value;
    }
};

typedef TParameterValue<StringType> StringParamValue;
typedef TParameterValue<IntType> IntParamValue;
typedef TParameterValue<Int2DType> Int2DParamValue;
typedef TParameterValue<Int3DType> Int3DParamValue;
typedef TParameterValue<DoubleType> DoubleParamValue;
typedef TParameterValue<Double2DType> Double2DParamValue;
typedef TParameterValue<Double3DType> Double3DParamValue;
typedef TParameterValue<BooleanType> BooleanParamValue;

struct Parameter : public OfxParamStruct {
    typedef boost::shared_ptr<ParameterValue> ParameterValuePtr;

    const EParameterType type;
    const std::string name;
    std::map<OfxTime, ParameterValuePtr> timeToValues;
    PropertySet propertySet;

    Parameter(EParameterType type, const std::string& name) :
        type(type), name(name) {
    }
};

} // namespace host
} // namespace openfx

#endif /* PARAMETER_H_ */
