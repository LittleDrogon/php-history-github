bison -p php -v -d language-parser.y
flex -Pphp -olanguage-scanner.c -i language-scanner.lex
bison -p cfg -v -d configuration-parser.y
flex -Pcfg -oconfiguration-scanner.c -i configuration-scanner.lex
