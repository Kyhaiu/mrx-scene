set_languages("c++20")
set_toolchains("clang")
set_toolset("cc", "clang")
set_toolset("cxx", "clang")

add_rules("mode.debug", "mode.release")

set_warnings("all", "error")

set_optimize("fastest")
add_cxflags("-std=c++20", { force = true })

add_includedirs("include")

-- add libraries
local project_libs = { "cxxopts", "fmt", "sfml", "opengl" }
local test_libs = { "gtest" }

add_requires(table.unpack(project_libs))
add_requires(table.unpack(test_libs))

-- librarys
target("core")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("math")
  set_kind("static")
  add_files("src/math/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("gui/imgui")
  set_kind("static")
  add_files("include/gui/imgui/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("gui/imgui-sfml")
  set_kind("static")
  add_deps("gui/imgui")
  add_files("include/gui/imgui-sfml/*.cpp")
  add_packages(table.unpack(project_libs))
  add_links("GL")
  set_targetdir("./app")

  
-- target("gui/imguizmo")
--   set_kind("static")
--   add_cxflags("-Wno-unused-function", "-Wno-unused-variable")
--   add_files("include/gui/imguizmo/*.cpp")
--   add_packages(table.unpack(project_libs))
--   set_targetdir("./app")

target("gui/view")
  set_kind("static")
  add_files("src/gui/view/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("models")
  set_kind("static")
  add_files("src/models/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("shapes")
  set_kind("static")
  add_files("src/shapes/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("utils")
  set_kind("static")
  add_files("src/utils/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

-- main project executable
target("app")
  set_kind("binary")
  add_files("src/main.cpp")
  add_packages(table.unpack(project_libs))
  add_deps("core")
  add_deps("math")
  add_deps("models")
  add_deps("gui/imgui")
  add_deps("gui/imgui-sfml")
  -- add_deps("gui/imguizmo")
  add_deps("gui/view")
  add_deps("shapes")
  add_deps("utils")
  set_targetdir("./app")

-- test suites
target("app_test")
  set_kind("binary")
  add_files("tests/**/*.cpp", "tests/main.cpp")
  add_packages(table.unpack(test_libs))
  add_deps("core")
  add_deps("math")
  add_deps("models")
  add_deps("shapes")
  add_deps("utils")
  set_targetdir("./app")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--