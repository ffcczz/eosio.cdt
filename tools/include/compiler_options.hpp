#pragma once
#include <eosio/utils.hpp>
#include <eosio/whereami/whereami.hpp>
#include <vector>
#include <string>
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#ifdef ONLY_LD
#define LD_CAT EosioLdToolCategory
#else
#define LD_CAT EosioCompilerToolCategory
#endif

#ifndef ONLY_LD
// OptionCategory  位置  eosio.cdt/eosio_llvm/include/llvm/Support/CommandLine.h
static llvm::cl::OptionCategory EosioCompilerToolCategory("compiler options");
#else
static llvm::cl::OptionCategory EosioLdToolCategory("ld options");
#endif

/// begin ld options
// 此处为定义，不是声明，即定义一个cl::opt类型   
static cl::opt<bool> fquery_opt(
    "fquery",
    cl::desc("Produce binaries for wasmql"),
    cl::cat(LD_CAT));
static cl::opt<bool> fquery_server_opt(
    "fquery-server",
    cl::desc("Produce binaries for wasmql"),
    cl::cat(LD_CAT));
static cl::opt<bool> fquery_client_opt(
    "fquery-client",
    cl::desc("Produce binaries for wasmql"),
    cl::cat(LD_CAT));
static cl::opt<bool> use_old_malloc_opt(
    "use-freeing-malloc",
    cl::desc("Set the malloc implementation to the old freeing malloc"),
    cl::Hidden,
    cl::cat(LD_CAT));
static cl::opt<std::string> eosio_imports_opt(
    "eosio-imports",
    cl::desc("Set the file for eosio.imports"),
    cl::Hidden,
    cl::cat(LD_CAT));
static cl::opt<std::string> pp_path_opt(
    "eosio-pp-dir",
    cl::desc("Set the directory for eosio.pp"),
    cl::Hidden,
    cl::cat(LD_CAT));
static cl::opt<bool> use_rt_opt(
    "use-rt",
    cl::desc("Use software compiler-rt"),
    cl::Hidden,
    cl::cat(LD_CAT));
static cl::opt<bool> fno_lto_opt(
    "fno-lto",
    cl::desc("Disable LTO"),
    cl::cat(LD_CAT));
static cl::opt<bool> fno_cfl_aa_opt(
      "fno-cfl-aa",
      cl::desc("Disable CFL Alias Analysis"),
      cl::cat(LD_CAT));
static cl::opt<bool> fno_stack_first_opt(
      "fno-stack-first",
      cl::desc("Don't set the stack first in memory"),
      cl::cat(LD_CAT));
static cl::opt<bool> fno_post_pass_opt(
      "fno-post-pass",
      cl::desc("Don't run post processing pass"),
      cl::cat(LD_CAT));
static cl::opt<std::string> lto_opt_opt(
      "lto-opt",
      cl::desc("LTO Optimization level (O0-O3)"),
      cl::cat(LD_CAT));
static cl::list<std::string> L_opt(
    "L",
    cl::desc("Add directory to library search path"),
    cl::cat(LD_CAT),
    cl::Prefix,
    cl::ZeroOrMore);
static cl::list<std::string> l_opt(
    "l",
    cl::desc("Root name of library to link"),
    cl::cat(LD_CAT),
    cl::Prefix,
    cl::ZeroOrMore);
static cl::opt<std::string> o_opt(
    "o",
    cl::desc("Write output to <file>"),
    cl::cat(LD_CAT));
static cl::list<std::string> input_filename_opt(
      cl::Positional,
      cl::desc("<input file> ..."),
      cl::cat(LD_CAT),
      cl::OneOrMore);
static cl::opt<bool> fasm_opt(
    "fasm",
    cl::desc("Assemble file for x86-64"),
    cl::ZeroOrMore,
    cl::cat(LD_CAT));
static cl::opt<bool> fnative_opt(
    "fnative",
    cl::desc("Compile and link for x86-64"),
    cl::ZeroOrMore,
    cl::cat(LD_CAT));
static cl::opt<bool> fuse_main_opt(
    "fuse-main",
    cl::desc("Use main as entry"),
    cl::cat(LD_CAT));
static cl::opt<bool> allow_sse_opt(
    "allow-sse",
    cl::desc("Should not be used, except for build libc"),
    cl::Hidden,
    cl::cat(LD_CAT));
/// End of ld options

#ifndef ONLY_LD
/// begin c/c++ options/
static cl::opt<bool> llvm_ir_opt(
    "emit-ir",
    cl::desc("Emit llvm ir"),
    cl::Hidden,
    cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> MF_opt(
    "MF",
    cl::desc("Write depfile output"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> MT_opt(
    "MT",
    cl::desc("Specify name of main file output in depfile"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> MD_opt(
    "MD",
    cl::desc("Write depfile containing user and system headers"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> MMD_opt(
    "MMD",
    cl::desc("Write depfile containing user"),
    cl::cat(EosioCompilerToolCategory));
static cl::list<std::string> resources(
     "R",
     cl::Prefix,
     cl::desc("Add a resource path for inclusion"),
     cl::ZeroOrMore,
     cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> abigen_opt(
    "abigen",
    cl::desc("Generate ABI"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> abigen_output_opt(
    "abigen_output",
    cl::desc("ABIGEN output"),
    cl::cat(EosioCompilerToolCategory));
// ignore for now
static cl::opt<bool> g_opt(
      "g",
      cl::desc("debug build <ignored>"),
      cl::Hidden,
      cl::ZeroOrMore,
      cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> x_opt(
      "x",
      cl::desc("<ignored>"),
      cl::Hidden,
      cl::Prefix,
      cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> pch_opt(
      "fpch-preprocess",
      cl::desc("<ignored>"),
      cl::Hidden,
      cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> CC_opt(
    "CC",
    cl::desc("Include comments from within macros in preprocessed output"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> C_opt(
    "C",
    cl::desc("Include comments in preprocessed output"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> c_opt(
    "c",
    cl::desc("Only run preprocess, compile, and assemble steps"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> color_diag_opt(
    "fcolor-diagnostics",
    cl::desc("Use colors in diagnostics"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> dD_opt(
    "dD",
    cl::desc("Print macro definitions in -E mode in addition to normal output"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> dI_opt(
    "dI",
    cl::desc("Print include directives in -E mode in addition to normal output"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> dM_opt(
    "dM",
    cl::desc("Print macro definitions in -E mode instead to normal output"),
    cl::cat(EosioCompilerToolCategory));
static cl::list<std::string> D_opt(
    "D",
    cl::desc("Define <macro> to <value> (or 1 if <value> omitted)"),
    cl::ZeroOrMore,
    cl::Prefix,
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> emit_ast_opt(
    "emit-ast",
    cl::desc("Emit Clang AST files for source inputs"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> emit_llvm_opt(
    "emit-llvm",
    cl::desc("Use the LLVM representation for assembler and object files"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> E_opt(
    "E",
    cl::desc("Only run the preprocessor"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> finline_functions_opt(
    "finline-functions",
    cl::desc("Inline suitable functions"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> finline_hint_functions_opt(
    "finline-hint-functions",
    cl::desc("Inline functions which are (explicitly or implicitly) marked inline"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fmerge_all_constants_opt(
    "fmerge-all-constants",
    cl::desc("Allow merging of constants"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fno_elide_constructors_opt(
    "fno-elide-constructors",
    cl::desc("Disable C++ copy constructor elision"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fstack_protector_all_opt(
    "fstack-protector-all",
    cl::desc("Force the usage of stack protectors for all functions"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fstack_protector_strong_opt(
    "fstack-protector-strong",
    cl::desc("Use a strong heuristic to apply stack protectors to functions"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fstack_protector_opt(
    "fstack-protector",
    cl::desc("Enable stack protectors for functions potentially vulnerable to stack smashing"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fstrict_enums_opt(
    "fstrict-enums",
    cl::desc("Enable optimizations based on the strict definition of an enum's value range"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fstrict_return_opt(
    "fstrict-return",
    cl::desc("Always treat control flow paths that fall off the end of a non-void function as unreachable"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fstrict_vtable_pointers_opt (
    "fstrict-vtable-pointers",
    cl::desc("Enable optimizations based on the strict rules for overwriting polymorphic C++ objects"),
    cl::cat(EosioCompilerToolCategory));
static cl::list<std::string> include_opt(
    "include",
    cl::desc("Include file before parsing"),
    cl::cat(EosioCompilerToolCategory),
    cl::ZeroOrMore);
static cl::list<std::string> I_opt(
    "I",
    cl::desc("Add directory to include search path"),
    cl::cat(EosioCompilerToolCategory),
    cl::Prefix,
    cl::ZeroOrMore);
static cl::opt<std::string> O_opt(
    "O",
    cl::desc("Optimization level s, 0-3"),
    cl::Prefix,
    cl::ZeroOrMore,
    cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> sysroot_opt(
    "sysroot",
    cl::desc("Set the system root directory"),
    cl::Prefix,
    cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> isysroot_opt(
    "isysroot",
    cl::desc("Set the system root directory (usually /)"),
    cl::Hidden,
    cl::Prefix,
    cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> isystem_opt(
    "isystem",
    cl::desc("Add directory to SYSTEM include search path"),
    cl::Prefix,
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> S_opt(
    "S",
    cl::desc("Only run preprocess and compilation steps"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<std::string> U_opt(
    "U",
    cl::desc("Undefine macro <macro>"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> v_opt(
    "v",
    cl::desc("Show commands to run and use verbose output"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> w_opt(
    "w",
    cl::desc("Suppress all warnings"),
    cl::cat(EosioCompilerToolCategory));
static cl::list<std::string> W_opt(
    "W",
    cl::desc("Enable the specified warning"),
    cl::cat(EosioCompilerToolCategory),
    cl::Prefix,
    cl::ZeroOrMore);
static cl::opt<std::string> contract_name(
    "contract",
    cl::desc("Contract name"),
    cl::cat(EosioCompilerToolCategory));

/// end c/c++ options

/// begin c++ options
#ifdef CPP_COMP
static cl::opt<std::string> std_opt(
    "std",
    cl::desc("Language standard to compile for"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> faligned_allocation_opt(
    "faligned-allocation",
    cl::desc("Enable C++17 aligned allocation functions"),
    cl::cat(EosioCompilerToolCategory));
static cl::opt<bool> fcoroutine_ts_opt(
    "fcoroutine-ts",
    cl::desc("Enable support for the C++ Coroutines TS"),
    cl::cat(EosioCompilerToolCategory));
#endif
/// end c++ options
#endif

struct Options {
   std::string output_fn;
   std::vector<std::string> inputs;  //指输入的文件
   bool link;
   bool abigen;
   std::string eosio_pp_dir;
   std::string abigen_output;
   std::string abigen_contract;
   std::vector<std::string> comp_options;
   std::vector<std::string> ld_options;
   std::vector<std::string> abigen_options;
   std::vector<std::string> abigen_resources;
   bool debug;
   bool native;
};

static void GetCompDefaults(std::vector<std::string>& copts) {
   const char* eosio_apply_suff = "${CMAKE_SHARED_LIBRARY_SUFFIX}";
   std::string apply_lib;
   if (!fnative_opt) {
       //emplace_back()和push_back()方法类似
      copts.emplace_back("--target=wasm32");  //应该是将C++的编译目标设置为wasm  
      copts.emplace_back("-ffreestanding");   // Assert that the compilation takes place in a freestanding environment
      copts.emplace_back("-nostdlib");
      copts.emplace_back("-fno-builtin");    // Disable implicit builtin knowledge of functions
   } else {
      copts.emplace_back("-Wunused-command-line-argument");
#ifdef __APPLE__
      copts.emplace_back("--target=x86_64-unknown-darwin-macho");
      copts.emplace_back("-mmacosx-version-min=10.13");    // Set Mac OS X deployment target
      copts.emplace_back("-fno-stack-protector");    // Disable the use of stack protectors
#else
      copts.emplace_back("--target=x86_64-unknown-linux-gnu");
#endif
      copts.emplace_back("-m64");
      if (!fasm_opt) {
         copts.emplace_back("-fno-builtin");   // 同上
         copts.emplace_back("-mstackrealign");  //   Force realign the stack at entry to every function
         copts.emplace_back("-DEOSIO_NATIVE");
         copts.emplace_back("-DLLP64");
      }
   }

   if (!fasm_opt) {
      copts.emplace_back("-fno-threadsafe-statics");   // Do not emit code to make initialization of local statics thread safe
#ifdef CPP_COMP
      copts.emplace_back("-fno-exceptions");
      copts.emplace_back("-fno-rtti");    // Disable generation of rtti information
      copts.emplace_back("-fmodules-ts");  //  Enable support for the C++ Modules TS
#endif
      copts.emplace_back("-DBOOST_DISABLE_ASSERTS");
      copts.emplace_back("-DBOOST_EXCEPTION_DISABLE");
      if (!fnative_opt) {
         copts.emplace_back("-Xclang");   //  -Xclang <arg> :  Pass <arg> to the clang compiler
         copts.emplace_back("-load");
         copts.emplace_back("-Xclang");
         if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/LLVMEosioApply"+eosio_apply_suff))
            copts.emplace_back(eosio::cdt::whereami::where()+"/LLVMEosioApply"+eosio_apply_suff);
         else if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/../lib/LLVMEosioApply"+eosio_apply_suff))
            copts.emplace_back(eosio::cdt::whereami::where()+"/../lib/LLVMEosioApply"+eosio_apply_suff);
      }
      if (false && fnative_opt) {
         copts.emplace_back("-Xclang");
         copts.emplace_back("-load");
         copts.emplace_back("-Xclang");
         if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/LLVMEosioSoftfloat"+eosio_apply_suff))
            copts.emplace_back(eosio::cdt::whereami::where()+"/LLVMEosioSoftfloat"+eosio_apply_suff);
         else if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/../lib/LLVMEosioSoftfloat"+eosio_apply_suff))
            copts.emplace_back(eosio::cdt::whereami::where()+"/../lib/LLVMEosioSoftfloat"+eosio_apply_suff);
      }
      if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/eosio_plugin"+eosio_apply_suff))
         copts.emplace_back("-fplugin="+eosio::cdt::whereami::where()+"/eosio_plugin"+eosio_apply_suff);
      else if (llvm::sys::fs::exists(eosio::cdt::whereami::where()+"/../lib/eosio_plugin"+eosio_apply_suff))
         copts.emplace_back("-fplugin="+eosio::cdt::whereami::where()+"/../lib/eosio_plugin"+eosio_apply_suff);
   }
}

#ifdef ONLY_LD
static void GetLdDefaults(std::vector<std::string>& ldopts) {
   if (!fnative_opt) {
      ldopts.emplace_back("--gc-sections");
      ldopts.emplace_back("--strip-all");
      ldopts.emplace_back("-zstack-size="+std::string("${EOSIO_STACK_SIZE}"));
      ldopts.emplace_back("--merge-data-segments");
      if (fquery_opt || fquery_server_opt || fquery_client_opt) {
         ldopts.emplace_back("--export-table");
         ldopts.emplace_back("-other-model");
         ldopts.emplace_back("--only-export \"*:table\"");
         ldopts.emplace_back("--only-export \"*:memory\"");
         if (fuse_main_opt) {
            ldopts.emplace_back("-e main");
            ldopts.emplace_back("--only-export \"main:function\"");
         }
         else {
            ldopts.emplace_back("-e initialize ");
            ldopts.emplace_back("--only-export \"initialize:function\"");
         }
         if (fquery_server_opt)
            ldopts.emplace_back("-export run_query ");
         else {
            ldopts.emplace_back("-export create_query_request ");
            ldopts.emplace_back("-export decode_query_response ");
            ldopts.emplace_back("-export describe_query_request ");
            ldopts.emplace_back("-export describe_query_response ");
         }
         if (fquery_server_opt)
            ldopts.emplace_back("--only-export \"run_query:function\"");
         else {
            ldopts.emplace_back("--only-export \"create_query_request:function\"");
            ldopts.emplace_back("--only-export \"decode_query_response:function\"");
            ldopts.emplace_back("--only-export \"describe_query_request:function\"");
            ldopts.emplace_back("--only-export \"describe_query_response:function\"");
         }
      } else {
         if (fuse_main_opt)
            ldopts.emplace_back("-e main");
         else
            ldopts.emplace_back("-e apply");
         ldopts.emplace_back("--only-export \"apply:function\"");
      }
      ldopts.emplace_back("-lc++ -lc -leosio");
      if (use_old_malloc_opt)
         ldopts.emplace_back("-leosio_malloc");
      else
         ldopts.emplace_back("-leosio_dsm");

      if (use_rt_opt || fquery_opt || fquery_server_opt || fquery_client_opt)
         ldopts.emplace_back("-lrt -lsf");
      if (fquery_opt || fquery_server_opt || fquery_client_opt)
         ldopts.emplace_back("-leosio_cmem");

   } else {
#ifdef __APPLE__
      ldopts.emplace_back("-arch x86_64 -macosx_version_min 10.13 -framework Foundation -framework System");
#endif
      ldopts.emplace_back("-static");
      ldopts.emplace_back("-lnative_c++ -lnative_c -lnative_eosio -lnative");
   }
}
#endif

// 主要参数在copts和agopts ldopts的配置上
// copts
// ldopts 
// agopts 生成abi的参数
static Options CreateOptions(bool add_defaults=true) {
   std::string output_fn;    // 
   std::vector<std::string> inputs;
   std::vector<std::string> agresources;
   std::vector<std::string> copts;  // 编译参数   copts参数在clang信息中找
   std::vector<std::string> ldopts;  // 链接参数  ld参数在eosio-ld --help信息中找
   std::vector<std::string> agopts;  // abi参数
   bool link = true;
   bool debug = false;
   std::string pp_dir;
   std::string abigen_output;
   std::string abigen_contract;

#ifdef ONLY_LD
   bool abigen = false;
#else
   bool abigen = abigen_opt;
   // 此处不明白
   abigen_output = abigen_output_opt;
   debug = g_opt;
#endif

   if (add_defaults) {
      GetCompDefaults(copts);
      GetCompDefaults(agopts);
#ifdef ONLY_LD
      GetLdDefaults(ldopts);
#endif
   }
  
#ifndef ONLY_LD 
   if (fquery_opt)
      ldopts.emplace_back("-fquery");   // Produce binaries for wasmql
   if (fquery_server_opt)
      ldopts.emplace_back("-fquery-server");   // Produce binaries for wasmql
   if (fquery_client_opt)
      ldopts.emplace_back("-fquery-client");   // Produce binaries for wasmql
#endif

   if (!pp_path_opt.empty())
      pp_dir = pp_path_opt;
   else
      pp_dir = eosio::cdt::whereami::where();

   if (!fno_cfl_aa_opt) {
      copts.emplace_back("-mllvm");  // -mllvm <value>   :   Additional arguments to forward to LLVM's option processing
      copts.emplace_back("-use-cfl-aa-in-codegen=both");
      agopts.emplace_back("-mllvm");
      agopts.emplace_back("-use-cfl-aa-in-codegen=both");
#ifdef ONLY_LD
      ldopts.emplace_back("-mllvm");
      ldopts.emplace_back("-use-cfl-aa-in-codegen=both");
#else
      ldopts.emplace_back("-fno-cfl-aa");   //  Disable CFL Alias Analysis
#endif
   }
   for ( auto input_filename : input_filename_opt ) {
#ifdef ONLY_LD
      ldopts.push_back(input_filename);
#else
      inputs.push_back(input_filename);
#endif
   }

#ifdef ONLY_LD
   ldopts.emplace_back("-L"+eosio::cdt::whereami::where()+"/../lib");  // Add directory to library search path
#ifndef __APPLE__
   ldopts.emplace_back("-L"+eosio::cdt::whereami::where()+"/../lib64");
#endif
#endif
#ifndef ONLY_LD
   if (!sysroot_opt.empty()) {
      copts.emplace_back("--sysroot="+sysroot_opt);
      copts.emplace_back("-I"+sysroot_opt+"/include/libcxx");  //   Add directory to include search path
      copts.emplace_back("-I"+sysroot_opt+"/include/libc");

      // only allow capi for native builds and for eosio-cc
      if (fnative_opt) {
        copts.emplace_back("-I"+sysroot_opt+"/include/eosiolib/capi");
        copts.emplace_back("-I"+sysroot_opt+"/include/eosiolib/native");
      }
#ifndef CPP_COMP
        copts.emplace_back("-I"+sysroot_opt+"/include/eosiolib/capi");
#endif
      copts.emplace_back("-I"+sysroot_opt+"/include/eosiolib/core");
      copts.emplace_back("-I"+sysroot_opt+"/include/eosiolib/contracts");

      ldopts.emplace_back("-L"+sysroot_opt+"/lib");
#ifndef __APPLE__
      ldopts.emplace_back("-L"+sysroot_opt+"/lib64");
#endif
   }
   else {
      copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/libcxx");
      copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/libc");
      copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include");
      copts.emplace_back("--sysroot="+eosio::cdt::whereami::where()+"/../");
      agopts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/libcxx");
      agopts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/libc");
      agopts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include");
      agopts.emplace_back("--sysroot="+eosio::cdt::whereami::where()+"/../");
      ldopts.emplace_back("-L"+eosio::cdt::whereami::where()+"/../lib");

      if (fnative_opt) {
        copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/eosiolib/capi");
        copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/eosiolib/native");
      }
#ifndef CPP_COMP
      copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/eosiolib/capi");
#endif
      copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/eosiolib/core");
      copts.emplace_back("-I"+eosio::cdt::whereami::where()+"/../include/eosiolib/contracts");

#ifndef __APPLE__
      ldopts.emplace_back("-L"+eosio::cdt::whereami::where()+"/../lib64");
#endif
   }

   if (!isystem_opt.empty()) {
      copts.emplace_back("-isystem="+isystem_opt);   // Add directory to SYSTEM include search path
   }
   if (CC_opt) {
      copts.emplace_back("-CC");   // Include comments from within macros in preprocessed output
   }
   if (C_opt) {
      copts.emplace_back("-C");  // Include comments in preprocessed output
   }

   copts.emplace_back("-c");  // Only run preprocess, compile, and assemble steps

   if (c_opt) {
      link = false;
   }
   if (dD_opt) {
      copts.emplace_back("-dD");  //  Print macro definitions in -E mode in addition to normal output
   }
   if (dI_opt) {
      copts.emplace_back("-dI");  // Print include directives in -E mode in addition to normal output
   }
   if (dM_opt) {
      copts.emplace_back("-dM");  // Print macro definitions in -E mode instead of normal output
   }
   for ( auto define : D_opt ) {
      copts.emplace_back("-D"+define);   // -D <macro>=<value>   :   Define <macro> to <value> (or 1 if <value> omitted)
   }
   if (emit_ast_opt) {
      copts.emplace_back("-emit-ast");  // Emit Clang AST files for source inputs
   }
   if (emit_llvm_opt) {
      copts.emplace_back("-emit-llvm");  // Use the LLVM representation for assembler and object files
   }
   if (E_opt) {
      link = false;
      copts.emplace_back("-E");  // Only run the preprocessor
   }
   if(g_opt)
      copts.emplace_back("-g");  // Generate source-level debug information
   if (color_diag_opt) {
      copts.emplace_back("-fcolor-diagnostics");  // Use colors in diagnostics
   }
   if (!MF_opt.empty()) {
      copts.emplace_back("-MF "+MF_opt);   // -MF <file>   :  Write depfile output from -MMD, -MD, -MM, or -M to <file>
   }
   if (MD_opt) {
      copts.emplace_back("-MD");  // Write a depfile containing user and system headers
   }
   if (MMD_opt) {
      copts.emplace_back("-MMD");  // Write a depfile containing user headers
   }
   if (!MT_opt.empty()) {
      copts.emplace_back("-MT "+MT_opt);  // -MT <value>  :  Specify name of main file output in depfile
   }
   if (finline_functions_opt) {
      copts.emplace_back("-finline-functions");  // Inline suitable functions
   }
   if (finline_hint_functions_opt) {
      copts.emplace_back("-finline-hint-functions");  // Inline functions which are (explicitly or implicitly) marked inline
   }
   if (fmerge_all_constants_opt) {
      copts.emplace_back("-fmerge-all-constants");  // 
   }
   if (fstack_protector_all_opt) {
      copts.emplace_back("-fstack-protector-all");  // Force the usage of stack protectors for all functions
   }
   if (fstack_protector_strong_opt) {
      copts.emplace_back("-fstack-protector-strong");  // Use a strong heuristic to apply stack protectors to functions
   }
   if (fstack_protector_opt) {
      copts.emplace_back("-fstack-protector");  //  Enable stack protectors for functions potentially vulnerable to stack smashing
   }
   if (fstrict_enums_opt) {
      copts.emplace_back("-fstrict-enums");  // Enable optimizations based on the strict definition of an enum's value range
   }
   if (fstrict_return_opt) {
      copts.emplace_back("-fstrict-return");  //  Always treat control flow paths that fall off the end of a non-void function as unreachable
   }
   for ( auto include : include_opt ) {
      copts.emplace_back("-include="+include);  // -include <file>   :   Include file before parsing
      agopts.emplace_back("-include="+include);
   }
   for ( auto inc_dir : I_opt ) {
      copts.emplace_back("-I"+inc_dir);
      agopts.emplace_back("-I"+inc_dir);
   }
   if (O_opt.empty() && !g_opt) {
      copts.emplace_back("-O3");
   }
   else if (!O_opt.empty() && !g_opt) {
      copts.emplace_back("-O"+O_opt);
   }
   if (S_opt) {
      copts.emplace_back("-S");  //  Only run preprocess and compilation steps
      link = false;
   }
   if (llvm_ir_opt) {
      copts.emplace_back("-S");
      copts.emplace_back("-emit-llvm");   //   Use the LLVM representation for assembler and object files
      link = false;
   }
   if (v_opt) {
      copts.emplace_back("-v");   //  Show commands to run and use verbose output
   }
   if (w_opt) {
      copts.emplace_back("-w");   //  Suppress all warnings
   }
   for ( auto warn : W_opt ) {
      copts.emplace_back("-W"+warn);  // -W<warning>   :  Enable the specified warning
   }

#endif
   if (!fnative_opt) {
#ifdef ONLY_LD
      if (!fno_stack_first_opt) {
         ldopts.emplace_back("-stack-first");
      }
      if (fno_lto_opt) {
         ldopts.emplace_back("--lto-O0");   // --lto-O<opt-level>    :  Optimization level for LTO
         if (!lto_opt_opt.empty())
            std::cerr << "Warning : lto disabled but lto optimization level given\n";
      }
      else if (!lto_opt_opt.empty()) {
         ldopts.emplace_back("--lto-"+lto_opt_opt);
      }
      else {
         ldopts.emplace_back("--lto-O3");
      }
#else
      if (fno_stack_first_opt) {
         ldopts.emplace_back("-fno-stack-first");  // Don't set the stack first in memory
      }
      if (fno_lto_opt) {
         ldopts.emplace_back("-fno-lto-opt");
      }
      else if (!lto_opt_opt.empty()) {
         ldopts.emplace_back("-lto-opt="+lto_opt_opt);  //  -lto-opt=<string>  : - LTO Optimization level (O0-O3)
      }
#endif
   }

   for ( auto lib_dir : L_opt ) {
      ldopts.emplace_back("-L"+lib_dir);   //  -L <dir>  :  Add a directory to the library search path
   }
   for ( auto library : l_opt ) {
      ldopts.emplace_back("-l"+library);   //  -l=<string>   :  - Root name of library to link
   }
   if (o_opt.empty()) {
#ifndef ONLY_LD
      if (inputs.size() == 1) {
         llvm::SmallString<256> fn  = llvm::sys::path::filename(inputs[0]);
         llvm::SmallString<256> fn2 = fn;
         llvm::sys::path::replace_extension(fn, ".wasm");
         //  output_fn即为  {输入文件名}.wasm 
         output_fn = fn.str();
         llvm::SmallString<256> res;
         llvm::sys::path::system_temp_directory(true, res);
         ldopts.emplace_back(std::string(std::string(res.str())+"/"+std::string(fn2.str())+".o"));
      } else {
         ldopts.emplace_back("a.out");
      }
#endif
      if (inputs.size() == 1) {      
         llvm::SmallString<256> fn = llvm::sys::path::filename(inputs[0]);
         llvm::sys::path::replace_extension(fn, ".wasm");
         ldopts.emplace_back("-o "+output_fn);
         output_fn = fn.str();
      } else {
         ldopts.emplace_back("-o a.out");
         output_fn = "a.out";
      }
   }
   else {
      ldopts.emplace_back("-o "+o_opt);
      output_fn = o_opt;
   }

   if (!fnative_opt) {
#ifndef ONLY_LD
      if (!eosio_imports_opt.empty()) {
         ldopts.emplace_back("-eosio-imports="+eosio_imports_opt);   //  -eosio-imports=<string>     :    Set the file for eosio.imports
      }
#else
      if (!eosio_imports_opt.empty()) {
         ldopts.emplace_back("--allow-undefined-file="+eosio_imports_opt);  //  --allow-undefined-file=<value>   :   Allow symbols listed in <file> to be undefined in linked binary
      }
      else {
         ldopts.emplace_back("--allow-undefined-file="+eosio::cdt::whereami::where()+"/../eosio.imports");
      }
#endif
   }

#ifndef ONLY_LD
#ifdef CPP_COMP
   if (! std_opt.empty()) {
      copts.emplace_back("--std="+std_opt);
      agopts.emplace_back("--std="+std_opt);
   } else {
      copts.emplace_back("--std=c++17");
      agopts.emplace_back("--std=c++17");

   }

   if (faligned_allocation_opt) {
      copts.emplace_back("-faligned-allocation");  //  Enable C++17 aligned allocation functions
      agopts.emplace_back("-faligned-allocation");
   }
   if (fcoroutine_ts_opt) {
      copts.emplace_back("-fcoroutine-ts");
      agopts.emplace_back("-fcoroutine-ts");
   }
   if (fno_elide_constructors_opt) {
      copts.emplace_back("-fno-elide-constructors");   // Disable C++ copy constructor elision
      agopts.emplace_back("-fno-elide-constructors");
   }
   if (fstrict_vtable_pointers_opt) {
      copts.emplace_back("-fstrict-vtable-pointers");  //  Enable optimizations based on the strict rules for overwriting polymorphic C++ objects
      agopts.emplace_back("-fstrict-vtable-pointers");
   }
#endif
   if (!contract_name.empty())
      abigen_contract = contract_name;
   else {
      llvm::SmallString<256> fn = llvm::sys::path::filename(output_fn);
      llvm::sys::path::replace_extension(fn, "");
      abigen_contract = fn.str();
   }

   for ( auto resource : resources ) {
      agresources.emplace_back(resource);
   }
   if (fnative_opt)
      ldopts.emplace_back("-fnative");  //  Compile and link for x86-64
   if (fuse_main_opt)
      ldopts.emplace_back("-fuse-main");  // Use main as entry
#endif
   return {output_fn, inputs, link, abigen, pp_dir, abigen_output, abigen_contract, copts, ldopts, agopts, agresources, debug, fnative_opt};
}
