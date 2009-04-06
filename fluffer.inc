<?php

class Fluffer
{

  static $UPPER_CASE_LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
  static $LOWER_CASE_LETTERS = 'abcdefghijklmnopqrstuvwxyz';
  static $NUMBERS = '1234567890';
  static $SYMBOLS = '~!@#$%^&*()_+{}|:"<>?`-=[]\\;\',./';
  static $DAYS_IN_MONTH = array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);

  /**
   * Produce a random character.
   */
  public static function fluffChar($charset) {
    $string_length = strlen($charset);
    $rand_string_pos = rand(0, $string_length -1);

    return $charset[$rand_string_pos];
  }

  /**
   * Return a random string of the specified length.
   *
   * @param length of the string to generate.
   */
  public static function fluffString($length, $charset = NULL) {

    // Are we actually generating anything?
    if ($length == 0) {
      return "";
    }

    // If the character set isn't provided use the default
    if (NULL == $charset) {
      $charset = Fluffer::getDefaultCharacters();
    }

    for ($i = 0; $i < $length; $i++) {
      $generated_chars[] = Fluffer::fluffChar($charset);
    }

    return join($generated_chars);
  }

  public static function getDefaultCharacters() {
    return "" . Fluffer::$UPPER_CASE_LETTERS . Fluffer::$LOWER_CASE_LETTERS . Fluffer::$NUMBERS . Fluffer::$SYMBOLS;
  }

  /**
   * Generate a random date.
   */
  public static function fluffDate() {

    $random_year = rand(1, 9999);
    $random_month = rand(1, 12);
    $random_day = rand(1, Fluffer::$DAYS_IN_MONTH[$random_month-1]);

    $random_date = sprintf("%04d-%02d-%02d", $random_year, $random_month, $random_day);

    return $random_date;
  }

  /**
   * Given a list of possible values, return one selected at random.
   */
  public static function fluffEnum($enum) {
    $rand_index = rand(0, sizeof($enum) - 1);

    return ($enum[$rand_index]);
  }

  /**
   * Generate a random positive integer.
   */
  public static function fluffInt() {
    return rand();
  }

  /**
   * Generate some random currency.
   *
   * Currency values are always rounded to two significant digits (pennies).
   */
  public static function fluffCurrency() {

    $random_int = rand();
    $random_float = ((float) $random_int) / 1000.0;
    $random_currency = round($random_float, 2);

    return $random_currency;
  }
}

 ?>
