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
  public static function fluffDate($max_year=9999, $max_month=12) {

    // Generate a random year
    $random_year = rand(1, $max_year);

    // Generate a random month
    // If we're at the max year, make sure we don't go over the max month!
    if ($random_year == $max_year)
    {
      $random_month = rand(1,$max_month);
    }
    else
    {
      $random_month = rand(1, 12);
    }

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

  public static function fluffTime()
  {
    $random_hour = rand(0, 23);
    $random_minute = rand(0, 59);
    $random_seconds = rand(0, 59);

    return "$random_hour:$random_minute:$random_seconds";
  }

  /**
   * Return a random UNIX timestamp.
   */
  public static function fluffTimestamp()
  {
    // Time stamps are in seconds since the epoch (1970-01-01 00:00:00 UTC)
    $random_timestamp = rand(0, 10000000000);

    return $random_timestamp;
  }
}

 ?>
