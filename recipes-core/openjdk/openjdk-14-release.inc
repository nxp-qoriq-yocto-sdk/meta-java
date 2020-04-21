# Copyright (C) 2020 Richard Leitner <richard.leitner@skidata.com>
# Released under the MIT license (see COPYING.MIT for the terms)

SRC_URI[jdk.sha256sum] = "f9c4a55ac858f858222bc5fe6e4b890f9b4a3f942fd0211575b0418aec5c14d6"

FILESPATH =. "${FILE_DIRNAME}/patches-openjdk-14:"

SRC_URI_append = "\
    file://libjava_childproc_multiple_definition_link_errors_with_GCC10.patch \
    file://libj2gss_NativeFunc_multiple_definition_link_errors_with_GCC10.patch \
    file://fixed_libsctp_link_errors_caused_by_GCC10.patch \
    "
