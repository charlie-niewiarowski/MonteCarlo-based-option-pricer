file(REMOVE_RECURSE
  "alias_AVX2_dp.h.tmp"
  "alias_AVX2_sp.h.tmp"
  "include/alias_avx2.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/alias_avx2.h_generated.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
