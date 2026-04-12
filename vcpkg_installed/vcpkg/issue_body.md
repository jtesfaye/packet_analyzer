Package: python3[core,extensions]:arm64-osx@3.12.9#7

**Host Environment**

- Host: arm64-osx
- Compiler: AppleClang 15.0.0.15000309
- CMake Version: 3.31.0
-    vcpkg-tool version: 2025-10-16-71538f2694db93da4668782d094768ba74c45991
    vcpkg-scripts version: c4e4a719ed 2025-11-06 (4 months ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached python-cpython-v3.12.9.tar.gz
-- Cleaning sources at /Users/jt/vcpkg-root/vcpkg/buildtrees/python3/src/v3.12.9-fd59580369.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source /Users/jt/vcpkg-root/vcpkg/downloads/python-cpython-v3.12.9.tar.gz
-- Applying patch 0001-only-build-required-projects.patch
-- Applying patch 0003-use-vcpkg-zlib.patch
-- Applying patch 0004-devendor-external-dependencies.patch
-- Applying patch 0005-dont-copy-vcruntime.patch
-- Applying patch 0008-python.pc.patch
-- Applying patch 0010-dont-skip-rpath.patch
-- Applying patch 0012-force-disable-modules.patch
-- Applying patch 0015-dont-use-WINDOWS-def.patch
-- Applying patch 0016-undup-ffi-symbols.patch
-- Applying patch 0018-fix-sysconfig-include.patch
-- Applying patch 0019-fix-ssl-linkage.patch
-- Applying patch 0020-Py_NO_LINK_LIB.patch
-- Applying patch 0002-static-library.patch
-- Using source at /Users/jt/vcpkg-root/vcpkg/buildtrees/python3/src/v3.12.9-fd59580369.clean
-- Getting CMake variables for arm64-osx
-- Loading CMake variables from /Users/jt/vcpkg-root/vcpkg/buildtrees/python3/cmake-get-vars_C_CXX-arm64-osx.cmake.log
CMake Error at /Users/jt/projects/workspace/personal_projects/sniffer/vcpkg_installed/arm64-osx/share/vcpkg-make/vcpkg_make.cmake:108 (message):
  python3 currently requires the following programs from the system package
  manager:

      autoconf autoconf-archive automake libtoolize

  

      On Debian and Ubuntu derivatives:
          sudo apt install autoconf autoconf-archive automake libtool
      On recent Red Hat and Fedora derivatives:
          sudo dnf install autoconf autoconf-archive automake libtool
      On Arch Linux and derivatives:
          sudo pacman -S autoconf autoconf-archive automake libtool
      On Alpine:
          apk add autoconf autoconf-archive automake libtool
      On macOS:
          brew install autoconf autoconf-archive automake libtool

Call Stack (most recent call first):
  /Users/jt/projects/workspace/personal_projects/sniffer/vcpkg_installed/arm64-osx/share/vcpkg-make/vcpkg_make_configure.cmake:62 (vcpkg_run_autoreconf)
  ports/python3/portfile.cmake:283 (vcpkg_make_configure)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "packet-analyzer",
  "version-string": "1.0.0",
  "dependencies": [
    "benchmark",
    "boost-interprocess",
    "boost-container",
    "boost-container-hash",
    "boost-unordered",
    "boost-lockfree",
    "boost-json",
    "boost-asio",
    "pybind11"
  ]
}

```
</details>
