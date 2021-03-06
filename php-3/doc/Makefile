# Generated automatically from Makefile.in by configure.
# Generated automatically from Makefile.in by configure.
# +----------------------------------------------------------------------+
# | PHP HTML Embedded Scripting Language Version 3.0                     |
# +----------------------------------------------------------------------+
# | Copyright (c) 1997,1998 PHP Development Team (See Credits file)      |
# +----------------------------------------------------------------------+
# | This program is free software; you can redistribute it and/or modify |
# | it under the terms of the GNU General Public License as published by |
# | the Free Software Foundation; either version 2 of the License, or    |
# | (at your option) any later version.                                  |
# |                                                                      |
# | This program is distributed in the hope that it will be useful,      |
# | but WITHOUT ANY WARRANTY; without even the implied warranty of       |
# | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        |
# | GNU General Public License for more details.                         |
# |                                                                      |
# | You should have received a copy of the GNU General Public License    |
# | along with this program; if not, write to the Free Software          |
# | Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.            |
# +----------------------------------------------------------------------+
# | Authors: Stig Bakken <ssb@guardian.no>                               |
# +----------------------------------------------------------------------+

#
# $Id: Makefile.in,v 1.44 1998/03/02 13:59:02 ssb Exp $
#

VERSION="3.0b6"
HTML_STYLESHEET=html.dsl
PHPWEB_STYLESHEET=phpweb.dsl
HOWTO_STYLESHEET=howto.dsl
PRINT_STYLESHEET=/usr/lib/sgml/docbook/print/docbook.dsl

all: html
status: status.txt missing.txt

PREFACE=preface.sgml \
	bookinfo.sgml

APPENDICES=appendices/migration.sgml \
           appendices/escaping.sgml \
           appendices/regexp.sgml \
           appendices/http-stuff.sgml \
           appendices/history.sgml \
           appendices/phpdevel.sgml

CHAPTERS=chapters/copyright.sgml \
         chapters/intro.sgml \
         chapters/config.sgml \
         chapters/install.sgml \
         chapters/features.sgml \
         chapters/lang-syntax.sgml \
         chapters/lang-expr.sgml \
         chapters/functions.sgml \
         chapters/lang-const.sgml

FUNCREF= \
	functions/array.sgml \
	functions/adabas.sgml \
	functions/datetime.sgml \
	functions/dir.sgml \
	functions/filesystem.sgml \
	functions/image.sgml \
	functions/ldap.sgml \
	functions/math.sgml \
	functions/msql.sgml \
	functions/mysql.sgml \
	functions/network.sgml \
	functions/oracle.sgml \
	functions/pgsql.sgml \
	functions/regex.sgml \
	functions/strings.sgml \
	functions/uodbc.sgml \
	functions/url.sgml

FILES=$(PREFACE) $(APPENDICES) $(CHAPTERS) $(FUNCREF)
ONLINE_MANUAL=*.php3
DIST_FILES=manual.zip manual.tar.gz manual.rtf bigmanual.htm
MIRROR_TARGETS=manual.php3 $(DIST_FILES)

manual.sgml: .manual.sgml
.manual.sgml: $(FILES)
	touch .manual.sgml

html: manual.htm
phpweb: manual.php3
rtf: manual.rtf
dvi: manual.dvi
ps print: manual.ps
howto: howto.htm

mirror-files: $(MIRROR_TARGETS)

sync-no: mirror-files
	rsync -Cavuz -e 'ssh -x -a' $(ONLINE_MANUAL) no.php.net:/local/www/php/phpweb/manual/
	rsync -Cavuz -e 'ssh -x -a' $(DIST_FILES) no.php.net:/local/www/php/phpweb/distributions/
	touch $@

commit-no: sync-no
	ssh -x -a no.php.net "cd /local/www/php/phpweb/manual;cvs commit -m '$(MESSAGE)'"
	touch commit-no

status.txt: manual.sgml checkdoc
	./checkdoc -s > $@

missing.txt: manual.sgml checkdoc
	./checkdoc -m > $@

bigmanual.htm: manual.sgml $(HTML_STYLESHEET)
	jade -V nochunks -d $(HTML_STYLESHEET) -t sgml manual.sgml > $@

manual.htm: manual.sgml $(HTML_STYLESHEET)
	jade -d $(HTML_STYLESHEET) -t sgml manual.sgml

manual.php3: manual.sgml $(PHPWEB_STYLESHEET)
	jade -d $(PHPWEB_STYLESHEET) -t sgml manual.sgml

manual.zip: manual.htm
	-rm -f $@ && zip -q $@ *.htm

manual.tar.gz: manual.htm
	-rm -f $@ && tar -cf - *.htm | gzip > manual.tar.gz

howto.htm: howto.sgml $(HOWTO_STYLESHEET) global.ent
	jade -d $(HOWTO_STYLESHEET) -t sgml howto.sgml

tex latex: manual.tex

# File endings we are going to define general rules for:
.SUFFIXES: .html .sgml .tex .dvi .ps .rtf

# General rules:
.sgml.tex:
	jade -d $(PRINT_STYLESHEET) -t tex $<

.sgml.rtf:
	jade -d $(PRINT_STYLESHEET) -t rtf $<

.tex.dvi:
	jadetex $<

.dvi.ps:
	dvips -o $@ $<

test: manual.sgml
	nsgmls -s $<

clean:
	-rm -f manual.txt manual*.html manual.rtf manual.info
	-rm -f manual.zip manual.tar.gz sync-no commit-no .manual.sgml
	-rm -f *.aux *.tex *.log *.dvi *.toc *.ps
	-rm -f status.txt missing.txt
	-rm -f *.htm *.php3 $(GENREF)
