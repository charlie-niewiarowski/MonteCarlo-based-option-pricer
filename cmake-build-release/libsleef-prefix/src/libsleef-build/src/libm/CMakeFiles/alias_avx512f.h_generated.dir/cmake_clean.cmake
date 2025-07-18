file(REMOVE_RECURSE
  "alias_AVX512F_dp.h.tmp"
  "alias_AVX512F_sp.h.tmp"
  "include/alias_avx512f.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/alias_avx512f.h_generated.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
