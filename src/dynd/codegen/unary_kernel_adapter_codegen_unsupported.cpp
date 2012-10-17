//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//


#include <dynd/platform_definitions.h>

#if !defined(DYND_CALL_SYSV_X64) && !defined (DYND_CALL_MSFT_X64)

#include <dynd/codegen/unary_kernel_adapter_codegen.hpp>
#include <stdexcept>

namespace dynd
{
    
    namespace
    {
        void unimplemented()
        {
            throw std::runtime_error("unimplemented error");
        }
    }
    uint64_t
    get_unary_function_adapter_unique_id( const dtype& DYND_UNUSED(restype)
                                          , const dtype& DYND_UNUSED(arg0type)
                                          , const dtype& DYND_UNUSED(arg1type)
                                          , calling_convention_t DYND_UNUSED(callconv))
    {
        unimplemented();
        return 0;
    }
    
    std::string
    get_unary_function_adapter_unique_id_string(uint64_t DYND_UNUSED(unique_id))
    {
        unimplemented();
        return std::string();
    }
    
    binary_operation_t
    codegen_unary_function_adapter(const memory_block_ptr& DYND_UNUSED(exec_memblock)
                                    , const dtype& DYND_UNUSED(restype)
                                    , const dtype& DYND_UNUSED(arg0type)
                                    , const dtype& DYND_UNUSED(arg1type)
                                    , calling_convention_t DYND_UNUSED(callconv))
    {
        unimplemented();
        return 0;
    }
    
}


#endif // defined(DYND_CALL_SYSV_X64)
