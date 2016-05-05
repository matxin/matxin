# -*- sh-basic-offset: 2 -*-

# Copyright (C) 2005 Universitat d'Alacant / Universidad de Alicante
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.


message ()
{
  echo "USAGE: $(basename $0) [-d datadir] [-f format] [-u] <direction> [in [out]]"
  echo " -d datadir       directory of linguistic data"
  echo " -f format        one of: txt (default), html, rtf, odt, docx, wxml, xlsx, pptx,"
  echo "                  xpresstag, html-noent, latex, latex-raw"
  echo " -a               display ambiguity"
  echo " -u               don't display marks '*' for unknown words"
  echo " -n               don't insert period before possible sentence-ends"
  echo " -o direction     translation direction using the translation memory,"
  echo "                  by default 'direction' is used instead"
  echo " -l               lists the available translation directions and exits"
  echo " direction        typically, LANG1-LANG2, but see modes.xml in language data"
  echo " in               input file (stdin by default)"
  echo " out              output file (stdout by default)"
  exit 1
}

list_directions ()
{
  for mode in "$DATADIR"/modes/*.mode; do
    echo "  $(basename "${mode%%.mode}")"
  done
}

locale_utf8 ()
{
  export LC_CTYPE=$(locale -a|grep -i "utf[.]*8"|head -1);
  if [ LC_CTYPE = "" ]; then
    echo "Error: Install an UTF-8 locale in your system";
    exit 1;
  fi
}

locale_latin1 ()
{
  export LC_CTYPE=$(locale -a|grep -i -e "8859-1" -e "@euro"|head -1);
  if [ LC_CTYPE = "" ]; then
    echo "Error: Install a Latin-1 locale in your system";
    exit 1;
  fi
}

test_zip ()
{
  if [ "$(which zip)" = "" ]; then
    echo "Error: Install 'zip' command in your system";
    exit 1;
  fi

  if [ "$(which unzip)" = "" ]; then
    echo "Error: Install 'unzip' command in your system";
    exit 1;
  fi
}

test_gawk ()
{
  GAWK=$(which gawk)
  if [ "$GAWK" = "" ]; then
    echo "Error: Install 'gawk' in your system"
    exit 1
  fi
}


translate_htmlnoent ()
{
  "$MATXIN_PATH/matxin-deshtml" ${FORMAT_OPTIONS} "$INFILE" | \
    if [ "$TRANSLATION_MEMORY_FILE" = "" ]; then
    cat
  else "$MATXIN_PATH/lt-tmxproc" "$TMCOMPFILE";
  fi | if [ ! -x "$DATADIR/modes/$PAIR.mode" ]; then
    sh "$DATADIR/modes/$PAIR.mode" "$OPTION" "$OPTION_TAGGER"
  else "$DATADIR/modes/$PAIR.mode" "$OPTION" "$OPTION_TAGGER"
  fi | if [ "$FORMAT" = "none" ]; then
    if [ "$REDIR" == "" ]; then cat; else cat > "$SALIDA"; fi
  else if [ "$REDIR" == "" ]; then "$MATXIN_PATH/matxin-rehtml-noent"; else "$MATXIN_PATH/matxin-rehtml-noent" > "$SALIDA"; fi
  fi

  rm -Rf "$TMCOMPFILE"
}





##########################################################
# Option and argument parsing, setting globals variables #
##########################################################
PATH="${MATXIN_PATH}:${PATH}"
[[ -z $TMPDIR ]] && TMPDIR=/tmp
TMCOMPFILE=$(mktemp "$TMPDIR/matxin.XXXXXXXX")
TMPFORMATFILE=$(mktemp "$TMPDIR/matxin.XXXXXXXX")
trap 'rm -Rf "$TMCOMPFILE"' EXIT

# Default values, may be overridden below:
PAIR=""
INFILE="/dev/stdin"
FORMAT="txt"
DATADIR=$DEFAULT_DIRECTORY
TRANSLATION_MEMORY_DIRECTION=$PAIR
LIST_MODES_AND_EXIT=false
FORMAT_OPTIONS=""

# Skip (but store) non-option arguments that come before options:
declare -a ARGS_PREOPT
declare -i OPTIND=1
while [[ $OPTIND -le $# ]]; do
  arg=${@:$OPTIND:1}
  case $arg in
    -*) break ;;
    *) ARGS_PREOPT+=($arg); (( OPTIND++ )) ;;
  esac
done


while getopts ":uahlf:d:m:o:n" opt; do
  case "$opt" in
    f) FORMAT=$OPTARG ;;
    d) DATADIR=$OPTARG ;;
    m) TRANSLATION_MEMORY_FILE=$OPTARG ;;
    o) TRANSLATION_MEMORY_DIRECTION=$OPTARG ;;
    u) UWORDS="no" ;;
    n) FORMAT_OPTIONS="-n" ;;
    a) OPTION_TAGGER="-m" ;;
    l) LIST_MODES_AND_EXIT=true ;;
    h) message ;;
    \?) echo "ERROR: Unknown option $OPTARG"; message ;;
    :) echo "ERROR: $OPTARG requires an argument"; message ;;
  esac
done
shift $(($OPTIND-1))

if $LIST_MODES_AND_EXIT; then list_directions; exit 0; fi

# Restore non-option arguments that came before options back into arg list:
set -- "${ARGS_PREOPT[@]}" "$@"

case "$#" in
  3)
    SALIDA=$3
    REDIR=">"
    INFILE=$2
    PAIR=$1
    if [[ ! -e "$INFILE" ]]; then
      echo "Error: file '$INFILE' not found."
      message
    fi
    ;;
  2)
    INFILE=$2
    PAIR=$1
    if [[ ! -e "$INFILE" ]]; then
      echo "Error: file '$INFILE' not found."
      message
    fi
    ;;
  1)
    PAIR=$1
    ;;
  *)
    message
    ;;
esac


if [[ -n $TRANSLATION_MEMORY_FILE ]]; then
  "$MATXIN_PATH/lt-tmxcomp" "$TRANSLATION_MEMORY_DIRECTION" "$TRANSLATION_MEMORY_FILE" "$TMCOMPFILE" >/dev/null
  if [ "$?" != "0" ]; then
    echo "Error: Cannot compile TM '$TRANSLATION_MEMORY_FILE'"
    echo"   hint: use -o parameter"
    message
  fi
fi

if [[ ! -d "$DATADIR/modes" ]]; then
  echo "Error: Directory '$DATADIR/modes' does not exist."
  message
fi

if [[ ! -e "$DATADIR/modes/$PAIR.mode" ]]; then
  echo -n "Error: Mode $PAIR does not exist"
  c=$(find "$DATADIR/modes"|wc -l)
  if [ "$c" -le 1 ]; then
    echo "."
  else
    echo ". Try one of:"
    list_directions
  fi
  exit 1
fi

#Parametro opcional, de no estar, lee de la entrada estandar (stdin)

case "$FORMAT" in
  none)
    if [ "$UWORDS" = "no" ]; then
      OPTION="-n";
    else OPTION="-g";
    fi
    ;;
  txt|rtf|html|xpresstag|mediawiki)
    if [ "$UWORDS" = "no" ]; then OPTION="-n";
    else OPTION="-g";
    fi;
    ;;
  rtf)
    if [ "$UWORDS" = "no" ]; then OPTION="-n";
    else OPTION="-g";
    fi;
    MILOCALE=$(locale -a|grep -i -v "utf\|^C$\|^POSIX$"|head -1);
    if [ "$MILOCALE" = "" ]; then
      echo "Error: Install a ISO-8859-1 compatible locale in your system";
      exit 1;
    fi
    export LC_CTYPE=$MILOCALE
    ;;

  html-noent)
    if [ "$UWORDS" = "no" ]; then OPTION="-n";
    else OPTION="-g";
    fi;
    translate_htmlnoent
    exit 0
    ;;
  
  txtu)
    FORMAT="txt";
    OPTION="-n"
    ;;
  htmlu)
    FORMAT="html";
    OPTION="-n";
    ;;
  rtfu)
    FORMAT="rtf";
    OPTION="-n";
    MILOCALE=$(locale -a|grep -i -v "utf\|^C$\|^POSIX$"|head -1);
    if [ "$MILOCALE" = "" ]; then
      echo "Error: Install a ISO-8859-1 compatible locale in your system";
      exit 1;
    fi
    export LC_CTYPE=$MILOCALE
    ;;



  *) # Por defecto asumimos txt
    FORMAT="txt"
    OPTION="-g"
    ;;
esac

if [ -z "$REF" ]
then
    REF=$FORMAT
fi

set -e -o pipefail

if [ "$FORMAT" = "none" ]; then
    cat "$INFILE"
else
  "$MATXIN_PATH/matxin-des$FORMAT" ${FORMAT_OPTIONS} "$TMPFORMATFILE" "$INFILE"
fi | if [ "$TRANSLATION_MEMORY_FILE" = "" ];
     then
         cat
     else
       "$MATXIN_PATH/lt-tmxproc" "$TMCOMPFILE"
     fi | if [ ! -x "$DATADIR/modes/$PAIR.mode" ]; then
              sh "$DATADIR/modes/$PAIR.mode" "$OPTION" "$OPTION_TAGGER"
          else
            "$DATADIR/modes/$PAIR.mode" "$OPTION" "$OPTION_TAGGER"
          fi | if [ "$FORMAT" = "none" ]; then
                   if [ "$REDIR" = "" ]; then
                       cat
                   else
                     cat > "$SALIDA"
                   fi
               else
                 if [ "$REDIR" = "" ]; then
                     "$MATXIN_PATH/matxin-reformat" "$TMPFORMATFILE"
                 else
                   "$MATXIN_PATH/matxin-reformat" "$TMPFORMATFILE" > "$SALIDA"
                 fi
               fi

rm $TMPFORMATFILE
