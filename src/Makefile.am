EXTRA_DIST = config.h data_manager.h matxin_string_utils.h simpleregex.h string_utils.h utf_converter.h xml_reader.h gen_fst2regex.sh gen_matxin_translator.sh deformat.xsl txt-format.xml wiki-format.xml rtf-format.xml html-format.xml

bin_PROGRAMS = analyser lex_trans st_intra st_inter st_prep st_verb sg_inter sg_intra sg_move \
               morph_gen reformat txt-deformat wiki-deformat html-deformat rtf-deformat

bin_SCRIPTS = matxin fst2regex.pl

analyser_SOURCES = analyser.cc io_redirect_handler.cc io_redirect_handler.hpp
analyser_LDADD = -lfreeling -lboost_system -lboost_program_options

lex_trans_SOURCES = lex_trans.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
lex_trans_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

st_intra_SOURCES = st_intra.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
st_intra_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

st_inter_SOURCES = st_inter.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
st_inter_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

st_prep_SOURCES = st_prep.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
st_prep_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

st_verb_SOURCES = st_verb.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
st_verb_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

sg_inter_SOURCES = sg_inter.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
sg_inter_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

sg_intra_SOURCES = sg_intra.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
sg_intra_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

sg_move_SOURCES = sg_move.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
sg_move_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

morph_gen_SOURCES = morph_gen.cc data_manager.cc utf_converter.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
morph_gen_LDADD = -lfreeling -lfoma -lboost_system -lboost_program_options

reformat_SOURCES = reformat.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h io_redirect_handler.cc io_redirect_handler.hpp
reformat_LDADD = -lfoma

txt_deformat_SOURCES = txt-deformat.cc
wiki_deformat_SOURCES = wiki-deformat.cc
html_deformat_SOURCES = html-deformat.cc
rtf_deformat_SOURCES = rtf-deformat.cc

txt-deformat.cc: txt-format.xml Makefile.am deformat.xsl
	$(XSlex_transPROC) --stringparam mode matxin deformat.xsl txt-format.xml | $(FLEX) -Cfer -t >$@

wiki-deformat.cc: wiki-format.xml Makefile.am deformat.xsl
	$(XSlex_transPROC) --stringparam mode matxin deformat.xsl wiki-format.xml | $(FLEX) -Cfer -t >$@

html-deformat.cc: html-format.xml Makefile.am deformat.xsl
	$(XSlex_transPROC) --stringparam mode matxin deformat.xsl html-format.xml | $(FLEX) -Cfer -t >$@

rtf-deformat.cc: rtf-format.xml Makefile.am deformat.xsl
	$(XSlex_transPROC) --stringparam mode matxin deformat.xsl rtf-format.xml | $(FLEX) -Cfer -t >$@

matxin: Makefile.am gen_matxin_translator.sh
	@echo "Creating $@ script"
	@echo "#!$(BASH)" > $@
	@echo "MATXIN_DIR=\"$(prefix)\"" >> $@
	@cat gen_matxin_translator.sh >> $@
	@chmod a+x $@

fst2regex.pl: Makefile.am gen_fst2regex.sh
	@echo "Creating $@ script"
	@echo "#!$(PERL)" > $@
	@cat gen_fst2regex.sh >> $@
	@chmod a+x $@
