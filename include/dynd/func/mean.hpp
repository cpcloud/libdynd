//
// Copyright (C) 2011-15 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/func/callable.hpp>

namespace dynd {
namespace nd {

  extern struct mean : declfunc<mean> {
    static callable make();
  } mean;

} // namespace dynd::nd
} // namespace dynd
