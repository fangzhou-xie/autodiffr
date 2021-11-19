# download the source codes


system("svn export https://github.com/autodiff/autodiff/trunk/autodiff/common inst/include/autodiff/common --force")
system("svn export https://github.com/autodiff/autodiff/trunk/autodiff/forward inst/include/autodiff/forward --force")
system("svn export https://github.com/autodiff/autodiff/trunk/autodiff/reverse inst/include/autodiff/reverse --force")

# replace all the #include <Eigen/Core> by #include <RcppEigen.h>
files <- c(
  "forward/real/eigen.hpp",
  "forward/dual/eigen.hpp",
  "reverse/var/eigen.hpp",
  "common/eigen.hpp"
)

# replace Eigen with RcppEigen
# this works if we only want to compile something within package
for (file in files) {
  # file <- files[1]
  filepath <- paste0("inst/include/autodiff/", file)
  lines <- readLines(filepath)
  lines[grepl("#include <Eigen/Core>", lines, fixed = TRUE)] <- "#include <RcppEigen.h>"
  writeLines(lines, filepath)
}

# next, try to establish a header that will include all headers
# but we might need to replace the #pragma once by actual headers
# headerfiles <- fs::dir_ls("inst/include/autodiff", recurse = TRUE, glob = "*.hpp")
# for (header in as.character(headerfiles)) {
#   # header <- as.character(headerfiles[1])
#   filename <- gsub("inst/include/autodiff", "", header, fixed = TRUE)
#   guard <- paste0("autodiffr", gsub("[/.]", "_", filename))
#   lines <- readLines(header)
#   lines[grepl("#pragma once", lines, fixed = TRUE)] <- paste0("#ifndef ", guard, "\n#define ", guard)
#   lines <- c(lines, paste0("#endif // ", guard, "\n"))
#   writeLines(lines, header)
# }

#ifndef __diffusr_h__\n#define __diffusr_h__