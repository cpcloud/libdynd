//
// Copyright (C) 2011-15 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/type.hpp>
#include <dynd/types/pointer_type.hpp>

namespace dynd {

struct DYND_API groupby_type_arrmeta {
};

struct DYND_API groupby_type_data {
  const char *data_values_pointer;
  const char *by_values_pointer;
};

namespace ndt {

  /**
   * The groupby type represents a transformation of
   * operand values and by nd::arrays into a 2D variable-sized
   * array whose rows are the groups as specified by a categorical
   * type.
   */
  class DYND_API groupby_type : public base_expr_type {
    type m_value_type, m_operand_type, m_groups_type;

  public:
    groupby_type(const type &data_values_type, const type &by_values_type);

    virtual ~groupby_type();

    const type &get_value_type() const { return m_value_type; }
    const type &get_operand_type() const { return m_operand_type; }
    const type &get_groups_type() const { return m_groups_type; }
    void print_data(std::ostream &o, const char *arrmeta,
                    const char *data) const;

    void print_type(std::ostream &o) const;

    type get_data_values_type() const;
    type get_by_values_type() const;

    /**
     * Given some arrmeta for the groupby type, return arrmeta
     * for a single element of the data_values array.
     */
    const char *get_data_value_arrmeta(const char *arrmeta) const
    {
      // First at_single gets us to the pointer<array<data_value>> type
      type d = m_operand_type.at_single(0, &arrmeta);
      // Second at_single gets us to the data_value type
      d.at_single(0, &arrmeta);
      return arrmeta;
    }

    /**
     * Given some arrmeta for the groupby type, returns the
     * arrmeta for the pointer type that points at the data
     * values.
     *
     * \param arrmeta  An instance of groupby type arrmeta.
     *
     * \returns  The pointer<data_values_type> arrmeta within the
     *           groupby arrmeta.
     */
    pointer_type_arrmeta *get_data_values_pointer_arrmeta(char *arrmeta) const
    {
      m_operand_type.at_single(0, const_cast<const char **>(&arrmeta));
      return reinterpret_cast<pointer_type_arrmeta *>(arrmeta);
    }

    /**
     * Given some arrmeta for the groupby type, returns the
     * arrmeta for the pointer type that points at the by
     * values.
     *
     * \param arrmeta  An instance of groupby type arrmeta.
     *
     * \returns  The pointer<by_values_type> arrmeta within the
     *           groupby arrmeta.
     */
    pointer_type_arrmeta *get_by_values_pointer_arrmeta(char *arrmeta) const
    {
      m_operand_type.at_single(1, const_cast<const char **>(&arrmeta));
      return reinterpret_cast<pointer_type_arrmeta *>(arrmeta);
    }

    void get_shape(intptr_t ndim, intptr_t i, intptr_t *out_shape,
                   const char *arrmeta, const char *data) const;

    bool is_lossless_assignment(const type &dst_tp, const type &src_tp) const;

    bool operator==(const base_type &rhs) const;

    type with_replaced_storage_type(const type &replacement_type) const;

    size_t make_operand_to_value_assignment_kernel(
        void *ckb, intptr_t ckb_offset, const char *dst_arrmeta,
        const char *src_arrmeta, kernel_request_t kernreq,
        const eval::eval_context *ectx) const;
    size_t make_value_to_operand_assignment_kernel(
        void *ckb, intptr_t ckb_offset, const char *dst_arrmeta,
        const char *src_arrmeta, kernel_request_t kernreq,
        const eval::eval_context *ectx) const;

    void get_dynamic_array_properties(
        const std::pair<std::string, gfunc::callable> **out_properties,
        size_t *out_count) const;
  };

  /**
   * Makes a groupby type.
   */
  inline type make_groupby(const type &data_values_type,
                           const type &by_values_type)
  {
    return type(new groupby_type(data_values_type, by_values_type), false);
  }

} // namespace dynd::ndt
} // namespace dynd
