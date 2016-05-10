#include <lttoolbox/fst_processor.h>
#include <lttoolbox/ltstr.h>
#include <locale>

#include <string>
#include <iostream>

//#include "config.h"
#include "matxin_string_utils.h"
//#include "io_redirect_handler.h"

#include <data_manager.h>
#include <xml_reader.h>

using namespace std;

FSTProcessor fstp_generation;

bool doGenTrace = false ;

// Hiztegi elebidunaren euskarazko ordainetik lehenengoa lortzen du.
// IN:  Euskarazko ordainak ( ordain1[/ordain2]* )
// OUT: lehenengoa          ( oradin1            )
wstring disambiguate(wstring &full)
{
  wstring output = full;

  for (size_t i = 0; i < output.size(); i++)
  {
    if (output[i] == L'/')
      output = output.substr(0, i);

    if (output[i] == L'\\')
      output.erase(i, 1);
  }

  if (output[output.size() - 1] == L'-')
    output.erase(output.size() - 1, 1);

  return output;
}


wstring keep_case(wstring form, wstring UpCase)
{
  wstring out_form = form;

  if (UpCase == L"first" || UpCase == L"title" || UpCase == L"all" )
    out_form[0] = toupper(out_form[0]);

  for (size_t i = 1; i<form.size(); i++)
  {
//    if (UpCase == L"title" && form[i - 1] == L' ')
    if (UpCase == L"all" || UpCase == L"title" && form[i - 1] == L'_')
      out_form[i] = toupper(out_form[i]);
  }

  return out_form;
}

wstring generation(wstring lemma, wstring pos, wstring suf, wstring cas,
                   wstring mi, wstring head_sem, bool &flexioned)
{
  wstring analisis, form, prefix, postposizio, pre_lemma, lemma_osoa;
  lemma_osoa = lemma;

  //if (cfg.DoGenTrace) {
  if (doGenTrace) {
    wcerr << lemma << L" " << pos << L" " << suf << L" " << mi << L" "
          << cas << L" " << head_sem << endl;
  }

  // Output blanks instead of _
  for (size_t i = 0; i < lemma.size(); i++)
  {
    if (lemma[i] == L'_') 
    {
      lemma[i]= L' ';
    }
  }

  flexioned = true;

  wstring pre_gen = L"^" + lemma_osoa + L"|" + mi + L"$";
  //if (cfg.DoGenTrace) {
  if (doGenTrace) {
    wcerr << pre_gen << endl;
  }
  wstring lemmaMorf = fstp_generation.biltrans(pre_gen);
  //if (cfg.DoGenTrace) {
  if (doGenTrace) {
    wcerr << lemmaMorf << endl;
  }

  if (lemmaMorf[0] == L'^' and lemmaMorf[1] == L'@')
  {
    form = L"#" + lemma_osoa;
    if(mi != L"")
    {
      form = form + L"|" + mi;
    }
  }
  else
  {
    form = lemmaMorf.substr(1, lemmaMorf.size() - 2);
  }

  for (size_t i = 0; i < lemmaMorf.size(); i++)
  {
    if (lemmaMorf[i] == L'\\')
    {
      lemmaMorf.erase(i, 1);
      i--;
    }
  }


  //if (cfg.DoGenTrace) {
  if (doGenTrace) {
    wcerr << lemmaMorf << L" " << mi << L" " << cas << L" " << head_sem << endl;
  }

  form = disambiguate(form);

  if(form == L"")
  {
    form = L"#" + lemma_osoa + L"|" + mi;
  }

  //if (cfg.DoGenTrace)
  if (doGenTrace) {
    wcerr << form << endl << endl;
  }
  return form;
}


wstring procSYN (xmlTextReaderPtr reader)
{
  wstring syns;

  wstring tagName = getTagName(reader);
  int tagType = xmlTextReaderNodeType(reader);

  if (tagName == L"SYN" and tagType == XML_READER_TYPE_ELEMENT)
  {
    syns += L"<SYN" + write_xml(allAttrib(reader));
    if (xmlTextReaderIsEmptyElement(reader) == 1)
    {
      syns += L"/>\n";
      return syns;
    }
    else
    {
      syns += L">\n";
    }
  }
  else
  {
    wcerr << L"ERROR: invalid tag: <" << tagName
          << L"> when <SYN> was expected..." << endl;
    exit(-1);
  }

  if (tagName == L"SYN" and tagType == XML_READER_TYPE_END_ELEMENT)
  {
    syns += L"</SYN>\n";
  }
  else
  {
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when </SYN> was expected..." << endl;
    exit(-1);
  }

  return syns;
}

wstring procNODE(xmlTextReaderPtr reader, wstring chunk_type)
{
  wstring nodes;
  wstring tagName = getTagName(reader);
  int tagType = xmlTextReaderNodeType(reader);

  if (tagName == L"NODE" and tagType != XML_READER_TYPE_END_ELEMENT)
  {
    wstring lem = attrib(reader, "lem");
    wstring pos = attrib(reader, "pos");
    wstring suf = attrib(reader, "suf");
    wstring cas = attrib(reader, "cas");
    wstring mi = attrib(reader, "mi");
    wstring head_sem = attrib(reader, "headsem");

    wstring form;
    bool flexioned = false;

    lem = keep_case(lem, attrib(reader, "UpCase"));

      form = generation(lem, pos, suf, cas, mi, head_sem, 
			flexioned);

    for (size_t i = 0; i<form.size(); i++) 
    {
      if (form[i] == L'_') 
      {
        form[i] = L' ';
      }
    }
//    form = keep_case(form, attrib(reader, "UpCase"));

    nodes += L"<NODE form='" + write_xml(form) + L"'" + write_xml(allAttrib(reader));

    if (xmlTextReaderIsEmptyElement(reader) == 1)
    {
      nodes += L"/>\n";
      return nodes;
    }
    else
    {
      nodes += L">\n";
    }
  }
  else
  {
    wcout << nodes;
    wcerr << L"ERROR: invalid tag: <"<< tagName
          << L"> when <NODE> was expected..." << endl;
    exit(-1);
  }

  int ret = nextTag(reader);
  tagName = getTagName(reader);
  tagType = xmlTextReaderNodeType(reader);

  // Process possible synonyms if any
  while (ret == 1 and tagName == L"SYN" and tagType == XML_READER_TYPE_ELEMENT)
  {
    nodes += procSYN(reader);

    ret = nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  }

  while (ret == 1 and tagName == L"NODE" and tagType == XML_READER_TYPE_ELEMENT)
  {
    nodes += procNODE(reader, chunk_type);

    nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  }

  if (tagName == L"NODE" and tagType == XML_READER_TYPE_END_ELEMENT)
  {
    nodes += L"</NODE>\n";
  }
  else
  {
    wcout << nodes;
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when </NODE> was expected..." << endl;
    exit(-1);
  }

  return nodes;
}


wstring procCHUNK(xmlTextReaderPtr reader)
{
  wstring tagName = getTagName(reader);
  int tagType = xmlTextReaderNodeType(reader);
  wstring tree, type;

  if (tagName == L"CHUNK" and tagType == XML_READER_TYPE_ELEMENT)
  {
    type = attrib(reader, "type");
    tree = L"<CHUNK" + write_xml(allAttrib(reader)) + L">\n";
  }
  else
  {
    wcerr << L"ERROR: invalid tag: <" << tagName
          << L"> when <CHUNK> was expected..." << endl;
    exit(-1);
  }

  int ret = nextTag(reader);
  tagName = getTagName(reader);
  tagType = xmlTextReaderNodeType(reader);

  tree += procNODE(reader, type);

  ret = nextTag(reader);
  tagName = getTagName(reader);
  tagType = xmlTextReaderNodeType(reader);
  while (ret == 1 and tagName == L"CHUNK" and tagType == XML_READER_TYPE_ELEMENT)
  {
    tree += procCHUNK(reader);

    ret = nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  }


  if (tagName == L"CHUNK" and tagType == XML_READER_TYPE_END_ELEMENT)
  {
    tree += L"</CHUNK>\n";
  }
  else
  {
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when </CHUNK> was expected..." << endl;
    exit(-1);
  }

  return tree;
}


wstring procSENTENCE (xmlTextReaderPtr reader)
{
  wstring tree;
  wstring tagName = getTagName(reader);
  int tagType = xmlTextReaderNodeType(reader);

  if (tagName == L"SENTENCE" and tagType != XML_READER_TYPE_END_ELEMENT)
  {
    tree = L"<SENTENCE" + write_xml(allAttrib(reader)) + L">\n";
  }
  else
  {
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when <SENTENCE> was expected..." << endl;
    exit(-1);
  }

  int ret = nextTag(reader);
  tagName = getTagName(reader);
  tagType = xmlTextReaderNodeType(reader);

  while (ret == 1 and tagName == L"CHUNK")
  {
    tree += procCHUNK(reader);

    ret = nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  }

  if (ret == 1 and tagName == L"SENTENCE" and tagType == XML_READER_TYPE_END_ELEMENT)
  {
    tree += L"</SENTENCE>\n";
  }
  else
  {
    wcerr << L"ERROR: invalid document: found <" << tagName
          << L"> when </SENTENCE> was expected..." << endl;
    exit(-1);
  }

  return tree;
}


int main(int argc, char *argv[])
{
  // Set locale information
  locale::global(locale(""));

  // Initialisation of the transducer for morphological generation.

  string morphGenFile = string(argv[1]);
  
  FILE *transducer = fopen(morphGenFile.c_str(), "r");
  fstp_generation.load(transducer);
  fclose(transducer);
  fstp_generation.initBiltrans();

  //cerr << "Fitxategi guztiak irekita." << endl;

  while (true)
  {
    // redirect io
//    Fd0WcoutRedirectHandler ioredirect(cfg);
    xmlTextReaderPtr reader;
    reader = xmlReaderForFd(0,"", NULL, 0);
  
    int ret = nextTag(reader);
    wstring tagName = getTagName(reader);
    int tagType = xmlTextReaderNodeType(reader);
  
    if (tagName == L"corpus" and tagType != XML_READER_TYPE_END_ELEMENT)
    {
      wcout << L"<?xml version='1.0' encoding='UTF-8'?>" << endl;
      wcout << L"<corpus " << write_xml(allAttrib(reader)) << L">\n";
    }
    else
    {
      wcerr << L"ERROR: invalid document: found <" << tagName
            << L"> when <corpus> was expected..." << endl;
      exit(-1);
    }
  
    ret = nextTag(reader);
    tagName = getTagName(reader);
    tagType = xmlTextReaderNodeType(reader);
  
    int i = 0;
    // corpus barruan dauden SENTENCE guztietarako
    while (ret == 1 and tagName == L"SENTENCE")
    {
      //SENTENCE irakurri eta prozesatzen du.
      wstring tree = procSENTENCE(reader);
      wcout << tree << endl;
      wcout.flush();
  
      //if (cfg.DoTrace)
      if (false)
      {
        ostringstream log_fileName_osoa;
        wofstream log_file;
  
	log_fileName_osoa.imbue(std::locale("C"));
        //log_fileName_osoa << cfg.Trace_File << i++ << ".xml";
  
        log_file.open(log_fileName_osoa.str().c_str(), ofstream::out | ofstream::app);
	log_file << L"<!-- Morphological generation (final tree) -->" << endl;
        log_file << tree << L"</corpus>\n";
        log_file.close();
      }
  
      ret = nextTag(reader);
      tagName = getTagName(reader);
      tagType = xmlTextReaderNodeType(reader);
    }
    xmlFreeTextReader(reader);
    xmlCleanupParser();
  
    if (ret == 1 and tagName == L"corpus" and tagType == XML_READER_TYPE_END_ELEMENT)
    {
      wcout << L"</corpus>\n";
      break;
    }
    else
    {
      wcerr << L"ERROR: invalid document: found <" << tagName
            << L"> when </corpus> was expected..." << endl;
      exit(-1);
    }
//    if (!ioredirect.serverOK()) {
//      break;
//    }
  }
}
