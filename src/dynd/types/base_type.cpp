//
// Copyright (C) 2011-15 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <dynd/type.hpp>
#include <dynd/types/builtin_type_properties.hpp>
#include <dynd/func/callable.hpp>

using namespace std;
using namespace dynd;

// Default destructor for the extended type does nothing
ndt::base_type::~base_type()
{
}

bool ndt::base_type::is_type_subarray(const type &subarray_tp) const
{
  // The default implementation is to check by-value equality.
  // Dimension or wrapper types should override this.
  return !subarray_tp.is_builtin() && (*this) == (*subarray_tp.extended());
}

void ndt::base_type::print_data(ostream &DYND_UNUSED(o),
                                const char *DYND_UNUSED(arrmeta),
                                const char *DYND_UNUSED(data)) const
{
  stringstream ss;
  ss << "cannot print data of type \"";
  print_type(ss);
  ss << "\"";

  throw std::runtime_error(ss.str());
}

bool ndt::base_type::is_expression() const
{
  return false;
}

bool
ndt::base_type::is_unique_data_owner(const char *DYND_UNUSED(arrmeta)) const
{
  return true;
}

void ndt::base_type::transform_child_types(
    type_transform_fn_t DYND_UNUSED(transform_fn),
    intptr_t DYND_UNUSED(arrmeta_offset), void *DYND_UNUSED(self),
    type &out_transformed_type, bool &DYND_UNUSED(out_was_transformed)) const
{
  // Default to behavior with no child types
  out_transformed_type = type(this, true);
}

ndt::type ndt::base_type::get_canonical_type() const
{
  // Default to no transformation of the type
  return type(this, true);
}

void ndt::base_type::set_from_utf8_string(
    const char *DYND_UNUSED(arrmeta), char *DYND_UNUSED(data),
    const char *DYND_UNUSED(utf8_begin), const char *DYND_UNUSED(utf8_end),
    const eval::eval_context *DYND_UNUSED(ectx)) const
{
  stringstream ss;
  ss << "Cannot set a dynd value with type " << type(this, true)
     << " from a string";
  throw type_error(ss.str());
}

ndt::type ndt::base_type::apply_linear_index(
    intptr_t nindices, const irange *DYND_UNUSED(indices), size_t current_i,
    const type &DYND_UNUSED(root_tp), bool DYND_UNUSED(leading_dimension)) const
{
  // Default to scalar behavior
  if (nindices == 0) {
    return type(this, true);
  } else {
    throw too_many_indices(type(this, true), current_i + nindices, current_i);
  }
}

intptr_t ndt::base_type::apply_linear_index(
    intptr_t nindices, const irange *DYND_UNUSED(indices), const char *arrmeta,
    const type &DYND_UNUSED(result_tp), char *out_arrmeta,
    memory_block_data *embedded_reference, size_t current_i,
    const type &DYND_UNUSED(root_tp), bool DYND_UNUSED(leading_dimension),
    char **DYND_UNUSED(inout_data),
    memory_block_data **DYND_UNUSED(inout_dataref)) const
{
  // Default to scalar behavior
  if (nindices == 0) {
    // Copy any arrmeta verbatim
    arrmeta_copy_construct(out_arrmeta, arrmeta, embedded_reference);
    return 0;
  } else {
    throw too_many_indices(type(this, true), current_i + nindices, current_i);
  }
}

ndt::type ndt::base_type::at_single(intptr_t DYND_UNUSED(i0),
                                    const char **DYND_UNUSED(inout_arrmeta),
                                    const char **DYND_UNUSED(inout_data)) const
{
  // Default to scalar behavior
  throw too_many_indices(type(this, true), 1, 0);
}

ndt::type
ndt::base_type::get_type_at_dimension(char **DYND_UNUSED(inout_arrmeta),
                                      intptr_t i, intptr_t total_ndim) const
{
  // Default to heterogeneous dimension/scalar behavior
  if (i == 0) {
    return type(this, true);
  } else {
    throw too_many_indices(type(this, true), total_ndim + i, total_ndim);
  }
}

void ndt::base_type::get_shape(intptr_t DYND_UNUSED(ndim),
                               intptr_t DYND_UNUSED(i),
                               intptr_t *DYND_UNUSED(out_shape),
                               const char *DYND_UNUSED(arrmeta),
                               const char *DYND_UNUSED(data)) const
{
  // Default to scalar behavior
  stringstream ss;
  ss << "requested too many dimensions from type " << type(this, true);
  throw runtime_error(ss.str());
}

void ndt::base_type::get_strides(size_t DYND_UNUSED(i),
                                 intptr_t *DYND_UNUSED(out_strides),
                                 const char *DYND_UNUSED(arrmeta)) const
{
  // Default to scalar behavior
}

bool ndt::base_type::is_c_contiguous(const char *DYND_UNUSED(arrmeta)) const
{
  return false;
}

axis_order_classification_t
ndt::base_type::classify_axis_order(const char *DYND_UNUSED(arrmeta)) const
{
  // Scalar types have no axis order
  return axis_order_none;
}

bool ndt::base_type::is_lossless_assignment(const type &dst_tp,
                                            const type &src_tp) const
{
  // Default to just an equality check
  return dst_tp == src_tp;
}

size_t ndt::base_type::get_default_data_size() const
{
  return get_data_size();
}

// TODO: Make this a pure virtual function eventually
void ndt::base_type::arrmeta_default_construct(
    char *DYND_UNUSED(arrmeta), bool DYND_UNUSED(blockref_alloc)) const
{
  stringstream ss;
  ss << "TODO: arrmeta_default_construct for " << type(this, true)
     << " is not implemented";
  throw std::runtime_error(ss.str());
}

void ndt::base_type::arrmeta_copy_construct(
    char *DYND_UNUSED(dst_arrmeta), const char *DYND_UNUSED(src_arrmeta),
    memory_block_data *DYND_UNUSED(embedded_reference)) const
{
  stringstream ss;
  ss << "TODO: arrmeta_copy_construct for " << type(this, true)
     << " is not implemented";
  throw std::runtime_error(ss.str());
}

void ndt::base_type::arrmeta_reset_buffers(char *DYND_UNUSED(arrmeta)) const
{
  // By default there are no buffers to reset
}

void ndt::base_type::arrmeta_finalize_buffers(char *DYND_UNUSED(arrmeta)) const
{
  // By default there are no buffers to finalize
}

// TODO: Make this a pure virtual function eventually
void ndt::base_type::arrmeta_destruct(char *DYND_UNUSED(arrmeta)) const
{
  stringstream ss;
  ss << "TODO: arrmeta_destruct for " << type(this, true)
     << " is not implemented";
  throw std::runtime_error(ss.str());
}

// TODO: Make this a pure virtual function eventually
void ndt::base_type::arrmeta_debug_print(
    const char *DYND_UNUSED(arrmeta), std::ostream &DYND_UNUSED(o),
    const std::string &DYND_UNUSED(indent)) const
{
  stringstream ss;
  ss << "TODO: arrmeta_debug_print for " << type(this, true)
     << " is not implemented";
  throw std::runtime_error(ss.str());
}

void ndt::base_type::data_construct(const char *DYND_UNUSED(arrmeta),
                                    char *DYND_UNUSED(data)) const
{
  stringstream ss;
  ss << "TODO: data_construct for " << type(this, true)
     << " is not implemented";
  throw runtime_error(ss.str());
}

void ndt::base_type::data_destruct(const char *DYND_UNUSED(arrmeta),
                                   char *DYND_UNUSED(data)) const
{
  stringstream ss;
  ss << "TODO: data_destruct for " << type(this, true) << " is not implemented";
  throw runtime_error(ss.str());
}

void ndt::base_type::data_destruct_strided(const char *DYND_UNUSED(arrmeta),
                                           char *DYND_UNUSED(data),
                                           intptr_t DYND_UNUSED(stride),
                                           size_t DYND_UNUSED(count)) const
{
  stringstream ss;
  ss << "TODO: data_destruct_strided for " << type(this, true)
     << " is not implemented";
  throw runtime_error(ss.str());
}

size_t ndt::base_type::get_iterdata_size(intptr_t DYND_UNUSED(ndim)) const
{
  stringstream ss;
  ss << "get_iterdata_size: dynd type " << type(this, true)
     << " is not uniformly iterable";
  throw std::runtime_error(ss.str());
}

size_t ndt::base_type::iterdata_construct(
    iterdata_common *DYND_UNUSED(iterdata),
    const char **DYND_UNUSED(inout_arrmeta), intptr_t DYND_UNUSED(ndim),
    const intptr_t *DYND_UNUSED(shape), type &DYND_UNUSED(out_uniform_tp)) const
{
  stringstream ss;
  ss << "iterdata_default_construct: dynd type " << type(this, true)
     << " is not uniformly iterable";
  throw std::runtime_error(ss.str());
}

size_t ndt::base_type::iterdata_destruct(iterdata_common *DYND_UNUSED(iterdata),
                                         intptr_t DYND_UNUSED(ndim)) const
{
  stringstream ss;
  ss << "iterdata_destruct: dynd type " << type(this, true)
     << " is not uniformly iterable";
  throw std::runtime_error(ss.str());
}

intptr_t ndt::base_type::make_assignment_kernel(
    void *DYND_UNUSED(ckb), intptr_t DYND_UNUSED(ckb_offset),
    const type &dst_tp, const char *DYND_UNUSED(dst_arrmeta),
    const type &src_tp, const char *DYND_UNUSED(src_arrmeta),
    kernel_request_t DYND_UNUSED(kernreq),
    const eval::eval_context *DYND_UNUSED(ectx)) const
{
  stringstream ss;
  ss << "make_assignment_kernel has not been implemented for type '";
  if (this == dst_tp.extended()) {
    ss << dst_tp;
  } else {
    ss << src_tp;
  }
  ss << "'";
  throw std::runtime_error(ss.str());
}

size_t ndt::base_type::make_comparison_kernel(
    void *DYND_UNUSED(ckb), intptr_t DYND_UNUSED(ckb_offset),
    const type &src0_dt, const char *DYND_UNUSED(src0_arrmeta),
    const type &src1_dt, const char *DYND_UNUSED(src1_arrmeta),
    comparison_type_t comptype,
    const eval::eval_context *DYND_UNUSED(ectx)) const
{
  throw not_comparable_error(src0_dt, src1_dt, comptype);
}

bool
ndt::base_type::match(const char *DYND_UNUSED(arrmeta),
                      const type &candidate_tp,
                      const char *DYND_UNUSED(candidate_arrmeta),
                      std::map<std::string, type> &DYND_UNUSED(tp_vars)) const
{
  // The default match implementation is equality, pattern types
  // must override this virtual function.
  if (candidate_tp.is_builtin()) {
    return false;
  }

  return *this == *candidate_tp.extended();
}

void ndt::base_type::foreach_leading(const char *DYND_UNUSED(arrmeta),
                                     char *DYND_UNUSED(data),
                                     foreach_fn_t DYND_UNUSED(callback),
                                     void *DYND_UNUSED(callback_data)) const
{
  // Default to scalar behavior
  stringstream ss;
  ss << "dynd type " << type(this, true)
     << " is a scalar, foreach_leading cannot process";
  throw std::runtime_error(ss.str());
}

void ndt::base_type::get_scalar_properties_and_functions(
    std::vector<std::pair<std::string, gfunc::callable>> &out_properties,
    std::vector<std::pair<std::string, gfunc::callable>> &out_functions) const
{
  if ((get_flags() & type_flag_symbolic) == 0) {
    // This copies properties from the first non-array data type dimension
    // to
    // the requested vectors. It is for use by array data types, which by
    // convention
    // expose the properties from the first non-array data types, and
    // possibly add
    // additional properties of their own.
    size_t ndim = get_ndim();
    size_t properties_count = 0, functions_count = 0;
    const std::pair<std::string, gfunc::callable> *properties = NULL,
                                                  *functions = NULL;
    if (ndim == 0) {
      get_dynamic_array_properties(&properties, &properties_count);
      get_dynamic_array_functions(&functions, &functions_count);
    } else {
      type dt = get_type_at_dimension(NULL, ndim);
      if (!dt.is_builtin()) {
        dt.extended()->get_dynamic_array_properties(&properties,
                                                    &properties_count);
        dt.extended()->get_dynamic_array_functions(&functions,
                                                   &functions_count);
      } else {
        get_builtin_type_dynamic_array_properties(dt.get_type_id(), &properties,
                                                  &properties_count);
      }
    }
    out_properties.resize(properties_count);
    for (size_t i = 0; i < properties_count; ++i) {
      out_properties[i] = properties[i];
    }
    out_functions.resize(functions_count);
    for (size_t i = 0; i < functions_count; ++i) {
      out_functions[i] = functions[i];
    }
  }
}

void ndt::base_type::get_dynamic_type_properties(
    const std::pair<std::string, nd::callable> **out_properties,
    size_t *out_count) const
{
  // Default to no properties
  *out_properties = NULL;
  *out_count = 0;
}

void ndt::base_type::get_dynamic_type_functions(
    const std::pair<std::string, nd::callable> **out_functions,
    size_t *out_count) const
{
  // Default to no functions
  *out_functions = NULL;
  *out_count = 0;
}

void ndt::base_type::get_dynamic_array_properties(
    const std::pair<std::string, gfunc::callable> **out_properties,
    size_t *out_count) const
{
  // Default to no properties
  *out_properties = NULL;
  *out_count = 0;
}

void ndt::base_type::get_dynamic_array_functions(
    const std::pair<std::string, gfunc::callable> **out_functions,
    size_t *out_count) const
{
  // Default to no functions
  *out_functions = NULL;
  *out_count = 0;
}

size_t ndt::base_type::get_elwise_property_index(
    const std::string &property_name) const
{
  std::stringstream ss;
  ss << "the dynd type " << type(this, true);
  ss << " doesn't have a property \"" << property_name << "\"";
  throw std::runtime_error(ss.str());
}

ndt::type ndt::base_type::get_elwise_property_type(
    size_t DYND_UNUSED(elwise_property_index), bool &DYND_UNUSED(out_readable),
    bool &DYND_UNUSED(out_writable)) const
{
  throw std::runtime_error("get_elwise_property_type: this dynd type does "
                           "not have any properties");
}

size_t ndt::base_type::make_elwise_property_getter_kernel(
    void *DYND_UNUSED(ckb), intptr_t DYND_UNUSED(ckb_offset),
    const char *DYND_UNUSED(dst_arrmeta), const char *DYND_UNUSED(src_arrmeta),
    size_t DYND_UNUSED(src_elwise_property_index),
    kernel_request_t DYND_UNUSED(kernreq),
    const eval::eval_context *DYND_UNUSED(ectx)) const
{
  std::stringstream ss;
  ss << "the dynd type " << type(this, true);
  ss << " doesn't have any readable properties";
  throw std::runtime_error(ss.str());
}

size_t ndt::base_type::make_elwise_property_setter_kernel(
    void *DYND_UNUSED(ckb), intptr_t DYND_UNUSED(ckb_offset),
    const char *DYND_UNUSED(dst_arrmeta),
    size_t DYND_UNUSED(dst_elwise_property_index),
    const char *DYND_UNUSED(src_arrmeta), kernel_request_t DYND_UNUSED(kernreq),
    const eval::eval_context *DYND_UNUSED(ectx)) const
{
  std::stringstream ss;
  ss << "the dynd type " << type(this, true);
  ss << " doesn't have any writable properties";
  throw std::runtime_error(ss.str());
}

bool ndt::base_type::adapt_type(const type &DYND_UNUSED(operand_tp),
                                const std::string &DYND_UNUSED(op),
                                nd::callable &DYND_UNUSED(out_forward),
                                nd::callable &DYND_UNUSED(out_reverse)) const
{
  return false;
}

bool
ndt::base_type::reverse_adapt_type(const type &DYND_UNUSED(value_tp),
                                   const std::string &DYND_UNUSED(op),
                                   nd::callable &DYND_UNUSED(out_forward),
                                   nd::callable &DYND_UNUSED(out_reverse)) const
{
  return false;
}

// Some information about the builtin types

DYND_API uint8_t ndt::detail::builtin_data_sizes[builtin_type_id_count] = {
    0,                            sizeof(bool1),
    sizeof(int8),                 sizeof(int16),
    sizeof(int32),                sizeof(int64),
    sizeof(int128),               sizeof(uint8),
    sizeof(uint16),               sizeof(uint32),
    sizeof(uint64),               sizeof(uint128),
    sizeof(float16),              sizeof(float32),
    sizeof(float64),              sizeof(float128),
    sizeof(dynd::complex<float>), sizeof(dynd::complex<double>),
    0};

DYND_API uint8_t ndt::detail::builtin_kinds[builtin_type_id_count] = {
    void_kind, bool_kind,    sint_kind,    sint_kind, sint_kind,
    sint_kind, sint_kind,    uint_kind,    uint_kind, uint_kind,
    uint_kind, uint_kind,    real_kind,    real_kind, real_kind,
    real_kind, complex_kind, complex_kind, void_kind};

DYND_API uint8_t ndt::detail::builtin_data_alignments[builtin_type_id_count] = {
    1,
    1,
    1,
    scalar_align_of<int16>::value,
    scalar_align_of<int32>::value,
    scalar_align_of<int64>::value,
    scalar_align_of<int128>::value,
    1,
    scalar_align_of<uint16>::value,
    scalar_align_of<uint32>::value,
    scalar_align_of<uint64>::value,
    scalar_align_of<uint128>::value,
    scalar_align_of<float16>::value,
    scalar_align_of<float32>::value,
    scalar_align_of<float64>::value,
    scalar_align_of<float128>::value,
    scalar_align_of<dynd::complex<float>>::value,
    scalar_align_of<dynd::complex<double>>::value,
    1};
