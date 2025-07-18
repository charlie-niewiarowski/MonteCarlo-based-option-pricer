file(REMOVE_RECURSE
  "dispscalar.c"
  "dispscalar.c.body"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/dispscalar.c_generated.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
