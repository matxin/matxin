EXTRA_DIST = data_manager.h matxin_string_utils.h simpleregex.h string_utils.h utf_converter.h xml_reader.h gen_fst2regex.sh gen_matxin_translator.sh deformat.xsl txt-format.xml wiki-format.xml rtf-format.xml html-format.xml

matxin_DATA = modes.rng modes2bash.xsl matxin-createmodes.awk

matxindir = $(prefix)/share/matxin

bin_PROGRAMS = matxin-chunker matxin-xfer-lex matxin-xfer-intra matxin-xfer-inter matxin-xfer-adpos \
	       matxin-xfer-verb matxin-gen-inter matxin-gen-intra matxin-gen-move \
	       matxin-gen-morf matxin-reformat matxin-destxt matxin-deswiki matxin-deshtml matxin-desrtf

bin_SCRIPTS = matxin matxin-translator fst2regex.pl matxin-gen-modes matxin-validate-modes

matxin_chunker_SOURCES = chunker.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h #  #io_redirect_handler.cc io_redirect_handler.h
matxin_chunker_LDADD =  -lboost_system -lboost_program_options

matxin_xfer_lex_SOURCES = lex_trans.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_xfer_lex_LDADD = -lfoma -lboost_system -lboost_program_options

matxin_xfer_intra_SOURCES = st_intra.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_xfer_intra_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_xfer_inter_SOURCES = st_inter.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_xfer_inter_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_xfer_adpos_SOURCES = st_prep.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_xfer_adpos_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_xfer_verb_SOURCES = st_verb.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_xfer_verb_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_gen_inter_SOURCES = sg_inter.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_gen_inter_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_gen_intra_SOURCES = sg_intra.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_gen_intra_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_gen_move_SOURCES = sg_move.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_gen_move_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_gen_morf_SOURCES = morph_gen.cc data_manager.cc utf_converter.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_gen_morf_LDADD =  -lfoma -lboost_system -lboost_program_options

matxin_reformat_SOURCES = reformat.cc utf_converter.cc data_manager.cc xml_reader.cc simpleregex.cc matxin_string_utils.cc matxin_string_utils.h  #io_redirect_handler.cc io_redirect_handler.h
matxin_reformat_LDADD = -lfoma

matxin_destxt_SOURCES = txt-deformat.cc
matxin_deswiki_SOURCES = wiki-deformat.cc
matxin_deshtml_SOURCES = html-deformat.cc
matxin_desrtf_SOURCES = rtf-deformat.cc

txt-deformat.cc: txt-format.xml Makefile.am deformat.xsl
	$(XSLTPROC) --stringparam mode matxin deformat.xsl txt-format.xml | $(FLEX) -Cfer -t >$@

wiki-deformat.cc: wiki-format.xml Makefile.am deformat.xsl
	$(XSLTPROC) --stringparam mode matxin deformat.xsl wiki-format.xml | $(FLEX) -Cfer -t >$@

html-deformat.cc: html-format.xml Makefile.am deformat.xsl
	$(XSLTPROC) --stringparam mode matxin deformat.xsl html-format.xml | $(FLEX) -Cfer -t >$@

rtf-deformat.cc: rtf-format.xml Makefile.am deformat.xsl
	$(XSLTPROC) --stringparam mode matxin deformat.xsl rtf-format.xml | $(FLEX) -Cfer -t >$@

matxin: Makefile.am matxin-header.sh
	@echo "Creating matxin script"
	@echo "#!$(BASH)" > $@
	@echo "MATXIN_PATH=\"$(prefix)/bin\"" >>$@
	@echo "LTTOOLBOX_PATH=\"$(prefix)/bin\"" >>$@
	@echo "DEFAULT_DIRECTORY=\"$(prefix)/share/matxin\"" >>$@
	@cat matxin-header.sh >>$@
	@chmod a+x $@

matxin-translator: Makefile.am gen_matxin_translator.sh
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

matxin-gen-modes: Makefile.am modes-header.sh
	@echo "Creating matxin-gen-modes script"
	@echo "#!$(BASH)" > $@
	@echo "MATXINDIR="$(matxindir) >> $@
	@cat modes-header.sh >> $@
	@echo "$(XMLLINT) --relaxng $(matxindir)/modes.rng --noout \$$FILE1 && \\" >> $@
	@if [ `basename $(XSLTPROC)` == xsltproc ]; \
	  then echo "$(XSLTPROC) --stringparam prefix $(prefix)/bin --stringparam dataprefix \$$FULLDIRNAME  $(matxindir)/modes2bash.xsl \$$FILE1 | awk -f $(matxindir)/matxin-createmodes.awk PARAM=\$$FULLDIRNAME"; \
	  else echo "$(XSLTPROC) $(matxindir)/modes2bash.xsl \$$FILE1 \\\$$prefix=$(prefix)/bin \\\$$dataprefix=\$$FULLDIRNAME| awk -f $(matxindir)/matxin-createmodes.awk PARAM=\$$FULLDIRNAME"; \
	  fi >> $@
	@chmod a+x $@

matxin-validate-modes: Makefile.am validate-header.sh
	@echo "Creating matxin-validate-modes script"
	@echo "#!$(BASH)" > $@
	@cat validate-header.sh >> $@
	@echo "$(XMLLINT) --relaxng $(matxindir)/modes.rng --noout \"\$$FILE1\"" >>$@
	@chmod a+x $@

