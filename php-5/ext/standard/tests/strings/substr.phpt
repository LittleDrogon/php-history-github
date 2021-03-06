--TEST--
Testing substr() function
--FILE--
<?php

/* Prototype: string substr( string str, int start[, int length] )
 * Description: Returns the portion of string specified by the start and length parameters.
 */

$strings_array = array( NULL, "", 12345, "abcdef", "123abc", "_123abc");


/* Testing for error conditions */
echo "*** Testing for error conditions ***\n";

/* Zero Argument */
var_dump( substr() );

/* NULL as Argument */
var_dump( substr(NULL) );

/* Single Argument */
var_dump( substr("abcde") );

/* Scalar Argument */
var_dump( substr(12345) );

/* more than valid number of arguments ( valid are 2 or 3 ) */
var_dump( substr("abcde", 2, 3, 4) );

$counter = 1;
foreach ($strings_array as $str) {
  /* variations with two arguments */
  /* start values >, < and = 0    */

  echo ("\n--- Iteration ".$counter." ---\n");
  echo ("\n-- Variations for two arguments --\n");
  var_dump ( substr($str, 1) );
  var_dump ( substr($str, 0) );
  var_dump ( substr($str, -2) );

  /* variations with three arguments */
  /* start value variations with length values  */

  echo ("\n-- Variations for three arguments --\n");
  var_dump ( substr($str, 1, 3) );
  var_dump ( substr($str, 1, 0) );
  var_dump ( substr($str, 1, -3) );
  var_dump ( substr($str, 0, 3) );
  var_dump ( substr($str, 0, 0) );
  var_dump ( substr($str, 0, -3) );
  var_dump ( substr($str, -2, 3) );
  var_dump ( substr($str, -2, 0 ) );
  var_dump ( substr($str, -2, -3) );

  $counter++;
}

/* variation of start and length to point to same element */
echo ("\n*** Testing for variations of start and length to point to same element ***\n");
var_dump (substr("abcde" , 2, -2) );
var_dump (substr("abcde" , -3, -2) );

/* Testing to return empty string when start denotes the position beyond the truncation (set by negative length) */
echo ("\n*** Testing for start > truncation  ***\n");
var_dump (substr("abcdef" , 4, -4) );

/* String with null character */
echo ("\n*** Testing for string with null characters ***\n");
var_dump (substr("abc\x0xy\x0z" ,2) );

/* String with international characters */
echo ("\n*** Testing for string with international characters ***\n");
var_dump (substr('\xI??t??rn??ti??n??liz??ti??n',3) );

/* start <0 && -start > length */
echo "\n*** Start before the first char ***\n";
var_dump (substr("abcd" , -8) );
 
echo"\nDone";

?>
--EXPECTF--
*** Testing for error conditions ***

Warning: substr() expects at least 2 parameters, 0 given in %s on line %d
NULL

Warning: substr() expects at least 2 parameters, 1 given in %s on line %d
NULL

Warning: substr() expects at least 2 parameters, 1 given in %s on line %d
NULL

Warning: substr() expects at least 2 parameters, 1 given in %s on line %d
NULL

Warning: substr() expects at most 3 parameters, 4 given in %s on line %d
NULL

--- Iteration 1 ---

-- Variations for two arguments --
bool(false)
bool(false)
bool(false)

-- Variations for three arguments --
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)

--- Iteration 2 ---

-- Variations for two arguments --
bool(false)
bool(false)
bool(false)

-- Variations for three arguments --
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)

--- Iteration 3 ---

-- Variations for two arguments --
string(4) "2345"
string(5) "12345"
string(2) "45"

-- Variations for three arguments --
string(3) "234"
string(0) ""
string(1) "2"
string(3) "123"
string(0) ""
string(2) "12"
string(2) "45"
string(0) ""
string(0) ""

--- Iteration 4 ---

-- Variations for two arguments --
string(5) "bcdef"
string(6) "abcdef"
string(2) "ef"

-- Variations for three arguments --
string(3) "bcd"
string(0) ""
string(2) "bc"
string(3) "abc"
string(0) ""
string(3) "abc"
string(2) "ef"
string(0) ""
string(0) ""

--- Iteration 5 ---

-- Variations for two arguments --
string(5) "23abc"
string(6) "123abc"
string(2) "bc"

-- Variations for three arguments --
string(3) "23a"
string(0) ""
string(2) "23"
string(3) "123"
string(0) ""
string(3) "123"
string(2) "bc"
string(0) ""
string(0) ""

--- Iteration 6 ---

-- Variations for two arguments --
string(6) "123abc"
string(7) "_123abc"
string(2) "bc"

-- Variations for three arguments --
string(3) "123"
string(0) ""
string(3) "123"
string(3) "_12"
string(0) ""
string(4) "_123"
string(2) "bc"
string(0) ""
string(0) ""

*** Testing for variations of start and length to point to same element ***
string(1) "c"
string(1) "c"

*** Testing for start > truncation  ***
bool(false)

*** Testing for string with null characters ***
string(6) "c xy z"

*** Testing for string with international characters ***
string(26) "??t??rn??ti??n??liz??ti??n"

*** Start before the first char ***
string(4) "abcd"

Done
