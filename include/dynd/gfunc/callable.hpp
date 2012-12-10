//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__CALLABLE_HPP_
#define _DYND__CALLABLE_HPP_

#include <dynd/ndobject.hpp>
#include <dynd/dtypes/fixedstruct_dtype.hpp>
#include <dynd/dtype_assign.hpp>

namespace dynd { namespace gfunc {

/**
 * Generic prototype for a dynd callable object.
 *
 * \param params  The packed parameters for the function call.
 *                This corresponds to a particular fixedstruct parameters_pack dtype.
 * \param extra  Some static memory to help. TODO: switch to auxdata.
 *
 * \returns  A reference to an ndobject.
 */
typedef ndobject_preamble *(*callable_function_t)(const ndobject_preamble *params, void *extra);

/**
 * Object that provides a dynd-based parameter passing mechanism
 */
class callable {
    /** DType for the parameters, must be a fixedstruct dtype */
    dtype m_parameters_dtype;
    callable_function_t m_function;
    void *m_extra;
public:
    inline callable()
        : m_parameters_dtype(), m_function(), m_extra()
    {}

    inline callable(const dtype& parameters_dtype, callable_function_t function, void *extra = NULL)
        : m_parameters_dtype(parameters_dtype), m_function(function), m_extra(extra)
    {}

    inline void set(const dtype& parameters_dtype, callable_function_t function, void *extra = NULL)
    {
        m_parameters_dtype = parameters_dtype;
        m_function = function;
        m_extra = extra;
    }

    inline const dtype& get_parameters_dtype() const {
        return m_parameters_dtype;
    }
    
    inline void *get_extra() const {
        return m_extra;
    }

    inline callable_function_t get_function() const {
        return m_function;
    }

    inline ndobject call_generic(const ndobject& n) const {
        return ndobject(m_function(n.get_ndo(), m_extra), false);
    }

    template<class T>
    ndobject call(const T& p0) const;

    void debug_print(std::ostream& o, const std::string& indent = "") const;
};

namespace detail {
    template<class T>
    struct callable_argument_setter {
        static typename enable_if<is_dtype_scalar<T>::value, void>::type set(const dtype& paramtype, char *metadata, char *data, const T& value) {
            if (paramtype.get_type_id() == type_id_of<T>::value) {
                *reinterpret_cast<T *>(data) = value;
            } else {
                dtype_assign(paramtype, metadata, data, make_dtype<T>(), NULL, &value);
            }
        }
    };

    template<>
    struct callable_argument_setter<ndobject> {
        static void set(const dtype& paramtype, char *metadata, char *data, const ndobject& value) {
            if (paramtype.get_type_id() == void_pointer_type_id) {
                // TODO: switch to a better mechanism for passing ndobject references
                *reinterpret_cast<const ndobject_preamble **>(data) = value.get_ndo();
            } else {
                dtype_assign(paramtype, metadata, data, value.get_dtype(), value.get_ndo_meta(), value.get_ndo()->m_data_pointer);
            }
        }
    };
} // namespace detail

template<class T>
inline ndobject callable::call(const T& p0) const
{
    const fixedstruct_dtype *fsdt = static_cast<const fixedstruct_dtype *>(m_parameters_dtype.extended());
    if (fsdt->get_field_types().size() != 1) {
        stringstream ss;
        ss << "incorrect number of arguments (received 1) for dynd callable with parameters " << m_parameters_dtype;
        throw runtime_error(ss.str());
    }
    ndobject params(m_parameters_dtype);
    detail::callable_argument_setter<T>::set(fsdt->get_field_types()[0],
                    params.get_ndo_meta() + fsdt->get_metadata_offsets()[0],
                    params.get_ndo()->m_data_pointer + fsdt->get_data_offsets()[0],
                    p0);
    return call_generic(params);
}


}} // namespace dynd::gfunc

#endif // _DYND__CALLABLE_HPP_