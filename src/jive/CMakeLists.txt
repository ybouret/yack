include_directories(. ../core)

#main
file(GLOB jive_main_decl yack/jive/*.hpp)
file(GLOB jive_main_srcs yack/jive/*.cpp)
set(jive_main ${jive_main_decl} ${jive_main_srcs})
SOURCE_GROUP(jive FILES ${jive_main})
install( FILES ${jive_main_decl} DESTINATION include/yack/jive )

#pattern
file(GLOB jive_pattern_decl yack/jive/pattern/*.hpp)
file(GLOB jive_pattern_srcs yack/jive/pattern/*.cpp)
set(jive_pattern ${jive_pattern_decl} ${jive_pattern_srcs})
SOURCE_GROUP(jive/pattern FILES ${jive_pattern})
install( FILES ${jive_pattern_decl} DESTINATION include/yack/jive/pattern )

#pattern/basic
file(GLOB jive_pattern_basic_decl yack/jive/pattern/basic/*.hpp)
file(GLOB jive_pattern_basic_srcs yack/jive/pattern/basic/*.cpp)
set(jive_pattern_basic ${jive_pattern_basic_decl} ${jive_pattern_basic_srcs})
SOURCE_GROUP(jive/pattern/basic FILES ${jive_pattern_basic})
install( FILES ${jive_pattern_basic_decl} DESTINATION include/yack/jive/pattern/basic )

#pattern/joker
file(GLOB jive_pattern_joker_decl yack/jive/pattern/joker/*.hpp)
file(GLOB jive_pattern_joker_srcs yack/jive/pattern/joker/*.cpp)
set(jive_pattern_joker ${jive_pattern_joker_decl} ${jive_pattern_joker_srcs})
SOURCE_GROUP(jive/pattern/joker FILES ${jive_pattern_joker})
install( FILES ${jive_pattern_joker_decl} DESTINATION include/yack/jive/pattern/joker )

#pattern/logic
file(GLOB jive_pattern_logic_decl yack/jive/pattern/logic/*.hpp)
file(GLOB jive_pattern_logic_srcs yack/jive/pattern/logic/*.cpp)
set(jive_pattern_logic ${jive_pattern_logic_decl} ${jive_pattern_logic_srcs})
SOURCE_GROUP(jive/pattern/logic FILES ${jive_pattern_logic})
install( FILES ${jive_pattern_logic_decl} DESTINATION include/yack/jive/pattern/logic )

#pattern/regexp
file(GLOB jive_pattern_regexp_decl yack/jive/pattern/regexp/*.hpp)
file(GLOB jive_pattern_regexp_srcs yack/jive/pattern/regexp/*.cpp)
set(jive_pattern_regexp ${jive_pattern_regexp_decl} ${jive_pattern_regexp_srcs})
SOURCE_GROUP(jive/pattern/regexp FILES ${jive_pattern_regexp})
#install( FILES ${jive_pattern_regex_decl} DESTINATION include/yack/jive/pattern/regex )

#pattern/quirks
file(GLOB jive_pattern_quirks_decl yack/jive/pattern/quirks/*.hpp)
file(GLOB jive_pattern_quirks_srcs yack/jive/pattern/quirks/*.cpp)
set(jive_pattern_quirks ${jive_pattern_quirks_decl} ${jive_pattern_quirks_srcs})
SOURCE_GROUP(jive/pattern/quirks FILES ${jive_pattern_quirks})
#install( FILES ${jive_pattern_regex_decl} DESTINATION include/yack/jive/pattern/regex )

 
#lexical
file(GLOB jive_lexical_decl yack/jive/lexical/*.hpp)
file(GLOB jive_lexical_srcs yack/jive/lexical/*.cpp)
set(jive_lexical ${jive_lexical_decl} ${jive_lexical_srcs})
SOURCE_GROUP(jive/lexical FILES ${jive_lexical})
install( FILES ${jive_lexical_decl} DESTINATION include/yack/jive/lexical )

#lexical/flow
file(GLOB jive_lexical_flow_decl yack/jive/lexical/flow/*.hpp)
file(GLOB jive_lexical_flow_srcs yack/jive/lexical/flow/*.cpp)
set(jive_lexical_flow ${jive_lexical_flow_decl} ${jive_lexical_flow_srcs})
SOURCE_GROUP(jive/lexical/flow FILES ${jive_lexical_flow})
install( FILES ${jive_lexical_flow_decl} DESTINATION include/yack/jive/lexical/flow )

#lexical/plugin
file(GLOB jive_lexical_plugin_decl yack/jive/lexical/plugin/*.hpp)
file(GLOB jive_lexical_plugin_srcs yack/jive/lexical/plugin/*.cpp)
set(jive_lexical_plugin ${jive_lexical_plugin_decl} ${jive_lexical_plugin_srcs})
SOURCE_GROUP(jive/lexical/plugin FILES ${jive_lexical_plugin})
install( FILES ${jive_lexical_plugin_decl} DESTINATION include/yack/jive/lexical/plugin )

#syntax
file(GLOB jive_syntax_decl yack/jive/syntax/*.hpp)
file(GLOB jive_syntax_srcs yack/jive/syntax/*.cpp)
set(jive_syntax ${jive_syntax_decl} ${jive_syntax_srcs})
SOURCE_GROUP(jive/syntax FILES ${jive_syntax})
install( FILES ${jive_syntax_decl} DESTINATION include/yack/jive/syntax )

#syntax/rule
file(GLOB jive_syntax_rule_decl yack/jive/syntax/rule/*.hpp)
file(GLOB jive_syntax_rule_srcs yack/jive/syntax/rule/*.cpp)
set(jive_syntax_rule ${jive_syntax_rule_decl} ${jive_syntax_rule_srcs})
SOURCE_GROUP(jive/syntax/rule FILES ${jive_syntax_rule})
install( FILES ${jive_syntax_rule_decl} DESTINATION include/yack/jive/syntax/rule )

 #syntax/grammar
file(GLOB jive_syntax_grammar_decl yack/jive/syntax/grammar/*.hpp)
file(GLOB jive_syntax_grammar_srcs yack/jive/syntax/grammar/*.cpp)
set(jive_syntax_grammar ${jive_syntax_grammar_decl} ${jive_syntax_grammar_srcs})
SOURCE_GROUP(jive/syntax/grammar FILES ${jive_syntax_grammar})
install( FILES ${jive_syntax_grammar_decl} DESTINATION include/yack/jive/syntax/grammar )

#syntax/rule/wildcard
file(GLOB jive_syntax_rule_wildcard_decl yack/jive/syntax/rule/wildcard/*.hpp)
file(GLOB jive_syntax_rule_wildcard_srcs yack/jive/syntax/rule/wildcard/*.cpp)
set(jive_syntax_rule_wildcard ${jive_syntax_rule_wildcard_decl} ${jive_syntax_rule_wildcard_srcs})
SOURCE_GROUP(jive/syntax/rule/wildcard FILES ${jive_syntax_rule_wildcard})
install( FILES ${jive_syntax_rule_wildcard_decl} DESTINATION include/yack/jive/syntax/rule/wildcard )


add_library(yack-jive STATIC
${jive_main}
${jive_pattern} ${jive_pattern_basic} ${jive_pattern_joker} ${jive_pattern_logic}
${jive_pattern_regexp}
${jive_pattern_quirks}
${jive_lexical} ${jive_lexical_flow} ${jive_lexical_plugin}
${jive_syntax} ${jive_syntax_rule} ${jive_syntax_grammar} ${jive_syntax_rule_wildcard}
)

install( TARGETS yack-jive ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(jive)
