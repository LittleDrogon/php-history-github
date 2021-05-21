/* 
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2015 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Vlad Krupin <phpdevel@echospace.com>                         |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef _PSPELL_H
#define _PSPELL_H
#if HAVE_PSPELL
extern zend_module_entry pspell_module_entry;
#define pspell_module_ptr &pspell_module_entry
#else
#define pspell_module_ptr NULL
#endif

#define phpext_pspell_ptr pspell_module_ptr

#endif /* _PSPELL_H */