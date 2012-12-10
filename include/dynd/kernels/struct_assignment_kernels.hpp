//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__STRUCT_ASSIGNMENT_KERNELS_HPP_
#define _DYND__STRUCT_ASSIGNMENT_KERNELS_HPP_

#include <dynd/kernels/kernel_instance.hpp>
#include <dynd/dtypes/struct_dtype.hpp>
#include <dynd/dtypes/fixedstruct_dtype.hpp>

namespace dynd {

/**
 * Gets a kernel which copies values of the same struct dtype.
 *
 * \param val_struct_dtype  The struct dtype of both source and destination values.
 * \param out_kernel  The output where the kernel is placed.
 */
void get_struct_assignment_kernel(const dtype& val_struct_dtype,
                kernel_instance<unary_operation_pair_t>& out_kernel);

/**
 * Gets a kernel which converts from one struct to another.
 *
 * \param dst_struct_dtype  The dtype of the destination struct.
 * \param src_struct_dtype  The dtype of the source struct.
 * \param errmode  The error handling mode of the assignment.
 * \param out_kernel  The output where the kernel is placed.
 */
void get_struct_assignment_kernel(const dtype& dst_struct_dtype, const dtype& src_struct_dtype,
                assign_error_mode errmode,
                kernel_instance<unary_operation_pair_t>& out_kernel);

} // namespace dynd

#endif // _DYND__STRUCT_ASSIGNMENT_KERNELS_HPP_