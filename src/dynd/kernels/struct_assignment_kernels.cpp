//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <stdexcept>
#include <sstream>

#include <dynd/dtype.hpp>
#include <dynd/diagnostics.hpp>
#include <dynd/kernels/assignment_kernels.hpp>
#include <dynd/kernels/struct_assignment_kernels.hpp>

using namespace std;
using namespace dynd;

/////////////////////////////////////////
// struct to identical struct assignment

namespace { struct struct_asn_kernel {
    struct auxdata_storage {
        dtype dt;
        size_t field_count;
        kernel_instance<unary_operation_pair_t> *kernels;

        auxdata_storage()
            : dt(), kernels(NULL)
        {}
        auxdata_storage(const auxdata_storage& rhs)
            : dt(rhs.dt), field_count(rhs.field_count), kernels(new kernel_instance<unary_operation_pair_t>[field_count])
        {
            for (size_t i = 0; i < field_count; ++i) {
                kernels[i].copy_from(rhs.kernels[i]);
            }
        }
        ~auxdata_storage() {
            delete[] kernels;
        }

        void init(const dtype& d, size_t fcount) {
            dt = d;
            field_count = fcount;
            kernels = new kernel_instance<unary_operation_pair_t>[fcount];
        }
    };

    static void single(char *dst, const char *src, unary_kernel_static_data *extra)
    {
        auxdata_storage& ad = get_auxiliary_data<auxdata_storage>(extra->auxdata);
        const struct_dtype *sd = static_cast<const struct_dtype *>(ad.dt.extended());
        kernel_instance<unary_operation_pair_t> *kernels = &ad.kernels[0];
        const size_t *metadata_offsets = &sd->get_metadata_offsets()[0];
        const size_t *dst_data_offsets = reinterpret_cast<const size_t *>(extra->dst_metadata);
        const size_t *src_data_offsets = reinterpret_cast<const size_t *>(extra->src_metadata);

        unary_kernel_static_data kernel_extra;
        size_t field_count = ad.field_count;
        for (size_t i = 0; i < field_count; ++i) {
            kernel_extra.auxdata = kernels[i].auxdata;
            kernel_extra.dst_metadata = extra->dst_metadata + metadata_offsets[i];
            kernel_extra.src_metadata = extra->src_metadata + metadata_offsets[i];
            kernels[i].kernel.single(dst + dst_data_offsets[i], src + src_data_offsets[i], &kernel_extra);
        }
    }
};} // anonymous namespace

void dynd::get_struct_assignment_kernel(const dtype& val_struct_dtype,
                kernel_instance<unary_operation_pair_t>& out_kernel)
{
    if (val_struct_dtype.get_type_id() != struct_type_id) {
        stringstream ss;
        ss << "get_struct_assignment_kernel: provided dtype " << val_struct_dtype << " is not a struct dtype";
        throw runtime_error(ss.str());
    }
    const struct_dtype *sd = static_cast<const struct_dtype *>(val_struct_dtype.extended());
    size_t field_count = sd->get_field_count();

    out_kernel.kernel.single = &struct_asn_kernel::single;
    out_kernel.kernel.contig = NULL;

    make_auxiliary_data<struct_asn_kernel::auxdata_storage>(out_kernel.auxdata);
    struct_asn_kernel::auxdata_storage& ad = out_kernel.auxdata.get<struct_asn_kernel::auxdata_storage>();
    ad.init(val_struct_dtype, field_count);
    for (size_t i = 0, i_end = sd->get_field_types().size(); i != i_end; ++i) {
        ::get_dtype_assignment_kernel(sd->get_field_types()[i], ad.kernels[i]);
    }
}

/////////////////////////////////////////
// struct to different struct assignment

namespace { struct struct_struct_asn_kernel {
    struct auxdata_storage {
        dtype dst_dt, src_dt;
        size_t field_count;
        vector<int> field_reorder;
        kernel_instance<unary_operation_pair_t> *kernels;

        auxdata_storage()
            : dst_dt(), src_dt(), field_reorder(), kernels(NULL)
        {}
        auxdata_storage(const auxdata_storage& rhs)
            : dst_dt(rhs.dst_dt), src_dt(rhs.src_dt), field_count(rhs.field_count),
                    kernels(new kernel_instance<unary_operation_pair_t>[field_count])
        {
            for (size_t i = 0; i < field_count; ++i) {
                kernels[i].copy_from(rhs.kernels[i]);
            }
        }
        ~auxdata_storage() {
            delete[] kernels;
        }

        void init(const dtype& dst_d, const dtype& src_d, size_t fcount) {
            dst_dt = dst_d;
            src_dt = src_d;
            field_count = fcount;
            field_reorder.resize(fcount);
            kernels = new kernel_instance<unary_operation_pair_t>[fcount];
        }
    };

    static void single(char *dst, const char *src, unary_kernel_static_data *extra)
    {
        auxdata_storage& ad = get_auxiliary_data<auxdata_storage>(extra->auxdata);
        const struct_dtype *dst_sd = static_cast<const struct_dtype *>(ad.dst_dt.extended());
        const struct_dtype *src_sd = static_cast<const struct_dtype *>(ad.src_dt.extended());
        kernel_instance<unary_operation_pair_t> *kernels = &ad.kernels[0];
        const size_t *dst_metadata_offsets = &dst_sd->get_metadata_offsets()[0];
        const size_t *src_metadata_offsets = &src_sd->get_metadata_offsets()[0];
        const size_t *dst_data_offsets = reinterpret_cast<const size_t *>(extra->dst_metadata);
        const size_t *src_data_offsets = reinterpret_cast<const size_t *>(extra->src_metadata);
        size_t field_count = ad.field_count;

        unary_kernel_static_data kernel_extra;
        for (size_t i = 0; i < field_count; ++i) {
            kernel_extra.auxdata = kernels[i].auxdata;
            size_t i_src = ad.field_reorder[i];
            kernel_extra.dst_metadata = extra->dst_metadata + dst_metadata_offsets[i];
            kernel_extra.src_metadata = extra->src_metadata + src_metadata_offsets[i_src];
            kernels[i].kernel.single(dst + dst_data_offsets[i], src + src_data_offsets[i_src], &kernel_extra);
        }
    }
};} // anonymous namespace

void dynd::get_struct_assignment_kernel(const dtype& dst_struct_dtype, const dtype& src_struct_dtype,
                assign_error_mode errmode,
                kernel_instance<unary_operation_pair_t>& out_kernel)
{
    if (src_struct_dtype.get_type_id() != struct_type_id) {
        stringstream ss;
        ss << "get_struct_assignment_kernel: provided source dtype " << src_struct_dtype << " is not a struct dtype";
        throw runtime_error(ss.str());
    }
    if (dst_struct_dtype.get_type_id() != struct_type_id) {
        stringstream ss;
        ss << "get_struct_assignment_kernel: provided destination dtype " << dst_struct_dtype << " is not a struct dtype";
        throw runtime_error(ss.str());
    }
    const struct_dtype *dst_sd = static_cast<const struct_dtype *>(dst_struct_dtype.extended());
    const struct_dtype *src_sd = static_cast<const struct_dtype *>(src_struct_dtype.extended());
    size_t field_count = dst_sd->get_field_count();

    if (field_count != src_sd->get_field_count()) {
        stringstream ss;
        ss << "cannot assign dynd struct " << src_struct_dtype << " to " << dst_struct_dtype;
        ss << " because they have different numbers of fields";
        throw runtime_error(ss.str());
    }

    out_kernel.kernel.single = &struct_struct_asn_kernel::single;
    out_kernel.kernel.contig = NULL;

    make_auxiliary_data<struct_struct_asn_kernel::auxdata_storage>(out_kernel.auxdata);
    struct_struct_asn_kernel::auxdata_storage& ad = out_kernel.auxdata.get<struct_struct_asn_kernel::auxdata_storage>();
    ad.init(dst_struct_dtype, src_struct_dtype, field_count);
    // Match up the fields
    const vector<string>& dst_field_names = dst_sd->get_field_names();
    const vector<string>& src_field_names = src_sd->get_field_names();
    for (size_t i = 0; i != field_count; ++i) {
        const std::string& dst_name = dst_field_names[i];
        // TODO: accelerate this linear search if there are lots of fields?
        vector<string>::const_iterator it = std::find(src_field_names.begin(), src_field_names.end(), dst_name);
        if (it == src_field_names.end()) {
            stringstream ss;
            ss << "cannot assign dynd struct " << src_struct_dtype << " to " << dst_struct_dtype;
            ss << " because they have different field names";
            throw runtime_error(ss.str());
        }
        ad.field_reorder[i] = it - src_field_names.begin();
    }
    // Create the kernels for copying individual fields
    for (size_t i = 0; i != field_count; ++i) {
        int i_src = ad.field_reorder[i];
        ::get_dtype_assignment_kernel(dst_sd->get_field_types()[i], src_sd->get_field_types()[i_src],
                            errmode, NULL, ad.kernels[i]);
    }
}