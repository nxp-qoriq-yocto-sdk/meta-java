# Copyright (C) 2020 Richard Leitner <richard.leitner@skidata.com>
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Java runtime based upon the OpenJDK Project"
HOMEPAGE = "http://openjdk.java.net/"
LICENSE  = "GPL-2.0-with-classpath-exception"

LIC_FILES_CHKSUM = "file://LICENSE;md5=3e0b59f8fac05c3c03d4a26bbda13f8f"

DEPENDS = "\
    alsa-lib \
    bash \
    fontconfig \
    freetype \
    giflib \
    jpeg \
    libpng \
    libx11 xorgproto libxt libxext libxrender libxtst libxrandr \
    unzip-native \
    zip-native \
    zlib \
    "

DEPENDS_append_class-target = "\
    cups \
    openjdk-14-native \
    "

SRC_URI = "\
    https://hg.openjdk.java.net/jdk-updates/jdk14u/archive/jdk-${PV}-ga.tar.bz2;name=jdk \
    file://0001-make-autoconf-remove-hard-dependency-on-cups.patch \
    file://0002-make-autoconf-toolchain-remove-invalid-compiler-chec.patch \
    "

S = "${WORKDIR}/jdk14u-jdk-${PV}-ga"

require openjdk-common.inc

JAVA_HOME = "${libdir_jvm}/${PN}"

AUTOTOOLS_SCRIPT_PATH = "${S}/make/autoconf"

EXTRA_OECONF_append = "\
    --with-jobs=${@openjdk_build_helper_get_parallel_make(d)} \
    --with-tools-dir='${STAGING_DIR_NATIVE}' \
    --with-extra-cflags='${CFLAGS}' \
    --with-extra-cxxflags='${CXXFLAGS}' \
    --with-extra-ldflags='${LDFLAGS}' \
    --with-alsa-include='${STAGING_INCDIR}/alsa' \
    --with-alsa-lib='${STAGING_LIBDIR}' \
    --with-fontconfig-include='${STAGING_INCDIR}' \
    --with-freetype-include='${STAGING_INCDIR}/freetype2' \
    --with-freetype-lib='${STAGING_LIBDIR}' \
    --with-giflib=bundled \
    --with-libjpeg=bundled \
    --with-libpng=bundled \
    --with-zlib=bundled \
    --x-includes='${STAGING_INCDIR}/X11' \
    --x-libraries='${STAGING_LIBDIR}' \
    --with-vendor-name='meta-java' \
    --disable-manpages \
    --disable-warnings-as-errors \
    "

CONFIGUREOPTS_remove_class-native = "--host=${HOST_SYS}"
CONFIGUREOPTS_remove_class-native = "--target=${TARGET_SYS}"

EXTRA_OECONF_append_class-native = "\
    --with-boot-jdk='${OPENJDK14_BOOT_JDK}' \
    --enable-headless-only \
    --enable-openjdk-only \
    --without-cups \
    "

EXTRA_OECONF_append_class-target = "\
    --with-sys-root='${STAGING_DIR_HOST}' \
    --with-boot-jdk='${STAGING_LIBDIR_NATIVE}/jvm/openjdk-14-native' \
    "

do_patch_append() {
    bb.build.exec_func('do_fix_configure', d)
}

do_fix_configure() {
    chmod a+x ${S}/configure
}

do_configure_prepend () {
    export TOPDIR=${S}
    export CXXFILT="c++filt"
    unset CFLAGS CXXFLAGS LDFLAGS
}

EXTRA_OEMAKE_append = '\
    STRIP_POLICY=no_strip \
    QUIETLY= \
    jdk-image \
    legacy-jre-image \
'

do_install() {
    rm -rf ${D}${JAVA_HOME}
    mkdir -p ${D}${JAVA_HOME}
    if [[ "${PN}" = openjre* ]]; then
        cp -rp ${B}/images/jre/* ${D}${JAVA_HOME}
    else
        cp -rp ${B}/images/jdk/* ${D}${JAVA_HOME}
    fi
    chown -R root:root ${D}${JAVA_HOME}
    #install -m644 ${WORKDIR}/jvm.cfg  ${D}${JAVA_HOME}/jre/lib/${JDK_ARCH}/
    find ${D}${JAVA_HOME} -name "*.debuginfo" -exec rm {} \;
}

PACKAGES_append = " \
    ${PN}-demo \
    ${PN}-jmods \
"

FILES_${PN}-demo = " ${JAVA_HOME}/demo ${JAVA_HOME}/sample "
FILES_${PN}-jmods = " ${JAVA_HOME}/jmods "

FILES_${PN}_append = "\
    ${JAVA_HOME}/bin/[a-z]* \
    ${JAVA_HOME}/lib/[a-z]* \
    ${JAVA_HOME}/jre/bin/[a-z]* \
    ${JAVA_HOME}/jre/lib/[a-z]* \
    ${JAVA_HOME}/LICENSE \
    ${JAVA_HOME}/jre/LICENSE \
    ${JAVA_HOME}/release \
    ${JAVA_HOME}/conf \
"

FILES_${PN}-dev_append = "\
    ${JAVA_HOME}/include \
"

FILES_${PN}-doc_append = "\
    ${JAVA_HOME}/man \
    ${JAVA_HOME}/ASSEMBLY_EXCEPTION \
    ${JAVA_HOME}/THIRD_PARTY_README \
    ${JAVA_HOME}/jre/ASSEMBLY_EXCEPTION \
    ${JAVA_HOME}/jre/THIRD_PARTY_README \
    ${JAVA_HOME}/legal \
"

inherit update-alternatives

ALTERNATIVE_PRIORITY = "100"

ALTERNATIVE_openjdk-14 = "java javac"
ALTERNATIVE_openjre-14 = "java"
ALTERNATIVE_LINK_NAME[java] = "${bindir}/java"
ALTERNATIVE_TARGET[java] = "${JAVA_HOME}/bin/java"
ALTERNATIVE_LINK_NAME[javac] = "${bindir}/javac"
ALTERNATIVE_TARGET[javac] = "${JAVA_HOME}/bin/javac"
