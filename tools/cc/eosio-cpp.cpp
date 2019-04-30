// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/Builtins.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/FileSystem.h"

#include <eosio/abigen.hpp>
#include <eosio/codegen.hpp>

#include <iostream>
#include <sstream>

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

using namespace clang::tooling;
using namespace llvm;

#define CPP_COMP 1
#define COMPILER_NAME "eosio-cpp"
#include <compiler_options.hpp>

#include <set>
#include <sstream>

using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;
using namespace eosio;
using namespace eosio::cdt;

namespace eosio { namespace cdt {
   DeclarationMatcher function_decl_matcher = cxxMethodDecl().bind("eosio_tool");
   DeclarationMatcher record_decl_matcher   = cxxRecordDecl().bind("eosio_tool");
   DeclarationMatcher typedef_decl_matcher  = typedefDecl().bind("eosio_tool");
   auto               class_tmp_matcher     = classTemplateSpecializationDecl().bind("eosio_tool");
   abigen_exception  abigen_ex;
   codegen_exception codegen_ex;
   Rewriter          codegen_rewriter;
   CompilerInstance* codegen_ci;
   std::set<FileID>  codegen_rewritten;

   abigen& get_abigen_ref() {
      static abigen ag;
      return ag;
   }

   class EosioMethodMatcher : public MatchFinder::MatchCallback {
      public:
         virtual void run( const MatchFinder::MatchResult& res ) {
            if (const clang::CXXMethodDecl* decl = res.Nodes.getNodeAs<clang::CXXMethodDecl>("eosio_tool")->getCanonicalDecl()) {
               abi abi;
               if (decl->isEosioAction() && abigen::is_eosio_contract(decl, get_abigen_ref().get_contract_name())) {
                  get_abigen_ref().add_struct(decl);
                  get_abigen_ref().add_action(decl);
                  auto params = decl->parameters();
                  for (auto param : params) {
                     get_abigen_ref().add_type(param->getType());
                  }
               }
            }
         }
   };

   class EosioRecordMatcher : public MatchFinder::MatchCallback {
      public:
         bool has_added_clauses = false;
         virtual void run( const MatchFinder::MatchResult& res ) {
            if (const clang::CXXRecordDecl* decl = res.Nodes.getNodeAs<clang::CXXRecordDecl>("eosio_tool")) {
               if (!has_added_clauses) {
                  get_abigen_ref().add_clauses(get_abigen_ref().parse_clauses());
                  get_abigen_ref().add_contracts(get_abigen_ref().parse_contracts());
                  has_added_clauses = true;
               }
               if (decl->isEosioAction() && abigen::is_eosio_contract(decl, get_abigen_ref().get_contract_name())) {
                  get_abigen_ref().add_struct(decl);
                  get_abigen_ref().add_action(decl);
                  for (auto field : decl->fields()) {
                     get_abigen_ref().add_type( field->getType() );
                  }
               }
               if (decl->isEosioTable() && abigen::is_eosio_contract(decl, get_abigen_ref().get_contract_name())) {
                  get_abigen_ref().add_struct(decl);
                  get_abigen_ref().add_table(decl);
                  for (auto field : decl->fields())
                     get_abigen_ref().add_type( field->getType() );
               }
            }

            if (const clang::ClassTemplateSpecializationDecl* decl = res.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("eosio_tool")) {
               if ( decl->getName() == "multi_index" ) {
                  get_abigen_ref().add_table(decl->getTemplateArgs()[0].getAsIntegral().getExtValue(),
                                          (clang::CXXRecordDecl*)((clang::RecordType*)decl->getTemplateArgs()[1].getAsType().getTypePtr())->getDecl());
               }
            }
         }
   };
}} // ns eosio::cdt

//
void generate(const std::vector<std::string>& base_options, std::string input, std::string contract_name, const std::vector<std::string>& resource_paths, bool abigen) {
   std::vector<std::string> options;
   options.push_back("eosio-cpp");
   options.push_back(input); // don't remove oddity of CommonOptionsParser?
   options.push_back(input);
   options.push_back("--");
   for (size_t i=1; i < base_options.size(); i++) {
      options.push_back(base_options[i]);
   }
   options.push_back("--target=wasm32");
   options.push_back("-nostdlib");
   options.push_back("-ffreestanding");
   options.push_back("-fno-builtin");
   options.push_back("-fno-rtti");
   options.push_back("-fno-exceptions");
   options.push_back("-I${Boost_INCLUDE_DIRS}");
   options.push_back("-DBOOST_DISABLE_ASSERTS");
   options.push_back("-DBOOST_EXCEPTION_DISABLE");
   options.push_back("-Wno-everything");
   options.push_back("-std=c++17");
   options.push_back(std::string("-I")+eosio::cdt::whereami::where()+"/../include/libcxx");
   options.push_back(std::string("-I")+eosio::cdt::whereami::where()+"/../include/libc");
   options.push_back(std::string("-I")+eosio::cdt::whereami::where()+"/../include");
   options.push_back(std::string("-I")+eosio::cdt::whereami::where()+"/../../../../../libraries/libc++/libcxx/include");
   options.push_back(std::string("-I")+eosio::cdt::whereami::where()+"/../../../../../libraries/libc/musl/include");
   options.push_back(std::string("-I")+eosio::cdt::whereami::where()+"/../../../../../libraries");
   options.push_back(std::string("-I")+eosio::cdt::whereami::where()+"/../../../../../libraries/boost/include");

   int size = options.size();
   const char** new_argv = new const char*[size];
   for (size_t i=0; i < size; i++)
      new_argv[i] = options[i].c_str();

   // EosioCompilerToolCategory 位置  eosio.cdt/tools/include/compiler_options.hpp.in
   CommonOptionsParser opts( size, new_argv, EosioCompilerToolCategory, 0 );
   ClangTool ctool(opts.getCompilations(), opts.getSourcePathList());

   get_abigen_ref().set_contract_name(contract_name);
   get_abigen_ref().set_resource_dirs(resource_paths);
   codegen::get().set_contract_name(contract_name);

   EosioMethodMatcher eosio_method_matcher;
   EosioRecordMatcher eosio_record_matcher;
   MatchFinder finder;

   finder.addMatcher(function_decl_matcher, &eosio_method_matcher);
   finder.addMatcher(record_decl_matcher, &eosio_record_matcher);
   finder.addMatcher(class_tmp_matcher, &eosio_record_matcher);

   int tool_run = -1;
   tool_run = ctool.run(newFrontendActionFactory(&finder).get());
   if (tool_run != 0) {
      throw std::runtime_error("abigen error");
   }
   std::string abi_s;
   if (get_abigen_ref().is_empty())
      return;
   get_abigen_ref().to_json().dump(abi_s);
   codegen::get().set_abi(abi_s);
   tool_run = ctool.run(newFrontendActionFactory<eosio_codegen_frontend_action>().get());
   if (tool_run != 0) {
      throw std::runtime_error("codegen error");
   }
}

int main(int argc, const char **argv) {

   // fix to show version info without having to have any other arguments
   // 有-v参数则直接打印出版本信息然后退出
   for (int i=0; i < argc; i++) {
     if (argv[i] == std::string("-v")) {
       eosio::cdt::environment::exec_subprogram("clang-7", {"-v"});
       return 0;
     }
   }

   //设置版本打印信息
   cl::SetVersionPrinter([](llvm::raw_ostream& os) {
         os << COMPILER_NAME << " version " << "${VERSION_FULL}" << "\n";
   });
   //解析命令行选项
   cl::ParseCommandLineOptions(argc, argv, std::string(COMPILER_NAME)+" (Eosio C++ -> WebAssembly compiler)");
   //Options 结构位置 eosio.cdt/tools/include/compiler_options.hpp.in
   // CreateOptions 根据配置生成各种编译选项
   Options opts = CreateOptions();

   std::vector<std::string> outputs;
   try {
      for (auto input : opts.inputs) {
         std::vector<std::string> new_opts = opts.comp_options;
         SmallString<64> res;
         //获取系统临时目录
         llvm::sys::path::system_temp_directory(true, res);
         std::string tmp_file = std::string(res.c_str())+"/"+llvm::sys::path::filename(input).str();
         std::string output;
         
         generate(opts.comp_options, input, opts.abigen_contract, opts.abigen_resources, opts.abigen);

         if (llvm::sys::fs::exists(tmp_file)) {
            input = tmp_file;
         }
         output = tmp_file+".o";

         new_opts.insert(new_opts.begin(), input);
         
         if (!opts.link) {
            output = opts.output_fn.empty() ? "a.out" : opts.output_fn;
         }

         new_opts.insert(new_opts.begin(), "-o "+output);
         outputs.push_back(output);

         if (!eosio::cdt::environment::exec_subprogram("clang-7", new_opts)) {
            llvm::sys::fs::remove(tmp_file);
            return -1;
         }
         llvm::sys::fs::remove(tmp_file);
      }
   } catch (std::runtime_error& err) {
      llvm::errs() << err.what() << '\n';
      return -1;
   }

   if (opts.link) {
      std::vector<std::string> new_opts = opts.ld_options;
      for (auto input : outputs) {
         new_opts.insert(new_opts.begin(), std::string(" ")+input+" ");
      }
   
      if (!eosio::cdt::environment::exec_subprogram("eosio-ld", new_opts)) {
         for (auto input : outputs) {
            llvm::sys::fs::remove(input);
         }
         return -1;
      }
      for (auto input : outputs) {
         llvm::sys::fs::remove(input);
      }
      if ( !llvm::sys::fs::exists( opts.output_fn ) ) {
         return -1;
      }

// need to generate dSYM for mac OSX
#ifdef __APPLE__
      if (opts.native) {
         if (opts.debug) {
            if (!eosio::cdt::environment::exec_subprogram("dsymutil", std::vector<std::string>{opts.output_fn}, true)) {
               return -1;
            }
         }
      }
#endif
   }

  return 0;
}
