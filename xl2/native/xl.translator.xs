// ****************************************************************************
//  xl.translator.xs                (C) 1992-2003 Christophe de Dinechin (ddd) 
//                                                                 XL2 project 
// ****************************************************************************
// 
//   File Description:
// 
//     The basic XL translator
// 
//     NOTE: 'translation' statements introduce new entries into that
//     namespace. For instance a 'translation Semantics' will add a
//     'Semantics' function into that namespace for Semantcs rewrites
//
// 
// 
// 
// 
// ****************************************************************************
// This program is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html for details
// ****************************************************************************
// * File       : $RCSFile$
// * Revision   : $Revision$
// * Date       : $Date$
// ****************************************************************************

import PT = XL.PARSER.TREE
import IO = XL.TEXT_IO
import SYM = XL.SYMBOLS
import BC = XL.BYTECODE


module XL.TRANSLATOR with
// ----------------------------------------------------------------------------
//   The transator deals with global translation phases and data structures
// ----------------------------------------------------------------------------

    procedure Compile(input : PT.tree)
    procedure Compile(input : PT.tree; output : text)
    procedure Compile(input : PT.tree; output : IO.output_file)

    context           : SYM.symbol_table := nil
    global_context    : SYM.symbol_table := nil
    function_context  : SYM.symbol_table := nil
    main_context      : SYM.symbol_table := nil
    top_context       : SYM.symbol_table := nil
    full_compile      : boolean := false
    verbose           : boolean := false

    nop               : BC.bytecode

    procedure InitializeTranslator

    // Invokation of Semantics on scopes
    // In XL, all symbols in a scope are visible within that scope
    // This function deals with the two-pass declaration process
    type scope_kind is enumeration
        scopeMain, scopeModule, scopeGlobal,
        scopeFunction, scopeLocal,
        scopeField, scopeArgs

    procedure CopyScopeProperties(toTable   : SYM.symbol_table;
                                  fromTable : SYM.symbol_table)
    function ScopeSemantics (input   : PT.tree;
                             scope   : scope_kind) return BC.bytecode
    function ScopeSemantics (input   : PT.tree;
                             scope   : scope_kind;
                             modname : PT.tree) return BC.bytecode
    function PostScopeSemantics (input   : PT.tree;
                                 scope   : scope_kind;
                                 modname : PT.tree) return BC.bytecode

    // Recursive implementation of something
    type recurse_fn is function(input : PT.tree) return BC.bytecode
    function Recurse(input : PT.tree;
                     what  : recurse_fn) return BC.bytecode

    // Append two trees
    function Append (t1 : PT.tree; t2 : PT.tree) return PT.tree


    // The following functions populate the local context
    procedure AddScopeDecl(table: SYM.symbol_table; decl : PT.tree)
    procedure AddScopeInit(table: SYM.symbol_table; init : PT.tree)
    procedure AddScopeTerm(table: SYM.symbol_table; term : PT.tree)
    procedure AddGlobalDecl(term : PT.tree)
    procedure AddGlobalInit(term : PT.tree)
    procedure AddGlobalTerm(term : PT.tree)


    // The following functions are automatically generated

    // PluginsInit contains code initializing translation entries in
    // the symbols table
    // XLSemantics and XLOptimization contain the code generated by
    // 'translation XLSemantics' and 'translation XLOptimization'
    procedure PluginsInit                                      // Generated
    function XLEvaluateType(input: PT.tree) return BC.bytecode // Generated
    function XLDeclarations(input: PT.tree) return BC.bytecode // Generated
    function XLSemantics(input: PT.tree) return BC.bytecode    // Generated
    function XLOptimizations(input: PT.tree) return BC.bytecode// Generated

    function XLEnterFunction(input: PT.tree) return PT.tree    // Generated
    function XLEnterIterator(input: PT.tree) return PT.tree    // Generated
    function XLEnterGeneric(input: PT.tree) return PT.tree     // Generated
    function XLConstant(input : PT.tree) return PT.tree        // Generated
    function XLMacros(input : PT.tree) return PT.tree          // Generated
    function XLNormalizeGeneric(input : PT.tree) return PT.tree// Generated
