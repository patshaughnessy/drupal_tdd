<?php

require_once 'fluffer.inc';

class FlufferTests extends PHPUnit_Framework_TestCase
{  
  public function setup()
  {
  }

  public function teardown()
  {
  }

  /**
   * Given a source string produce a hashmap who's key's are the unique
   * charaters making up the string, and whose values are the number of times
   * each key occurred.
   */
  private function buildStringHistogram($source) {

    $histogram = array();
    foreach (str_split($source) as $character) {
      if (array_key_exists($character, $histogram)) {
        $histogram[$character] += 1;
      } else {
        $histogram[$character] = 1;
      }
    }

    return $histogram;
  }

  /**
   * Test that the characters in a given string all come from the provided
   * character set.
   */
  public function assertStringOnlyContainsCharset($generated_string, $charset) {

    // Build a histogram keyed off of the default characters
    // We'll use this to check the random string to make sure each character
    // appears in the hashmap, and therefore in the set of default characters.
    $histogram = $this->buildStringHistogram($charset);

    // Make sure each character in the string is in the character set
    foreach (str_split($generated_string) as $generated_char) {
      if (!array_key_exists($generated_char, $histogram)) {
        $this->fail("One of the generated characters wasn't from the character set.  (Bad character = " . $generated_char .")");
      }
    }


  }

  /**
   * Test creating a random string of characters of a given length.
   */
  public function testRandomStringOfLength()
  {
    // Generate a random string of ten characters
    $random_string = Fluffer::fluffString(10);
    $this->assertEquals(10, strlen($random_string), "The generated string should be ten characters long.");

    // It should contain a mix of the default characters (numbers, letters and symbols)
    $default_characters = Fluffer::getDefaultCharacters();

    $this->assertStringOnlyContainsCharset($random_string, $default_characters);
  }

  public function testRandomStringOfZeroLength() {

    // Try to generate a random string of zero length
    $random_string = Fluffer::fluffString(0);
    $this->assertEquals(0, strlen($random_string), "Asking for a zero length string should generate a string of zero length.");
  }

  /**
   * Randomly generating strings should be of the correct length and only
   * contain characters from the specified character set.
   */
  public function testRandomStringOfLengthAndCharacterSet() {

    // Generate a random string of a small character set
    $charset = '123';
    $random_string = Fluffer::fluffString(10, $charset);

    // Make sure the generated string only contains the right characters
    $this->assertStringOnlyContainsCharset($random_string, $charset);
  }

  /**
   * Randomly generating a character should only produce characters from the
   * provided character set.
   */
  public function testRandomCharacterFromCharacterSet() {

    // Generate a random character from a character set
    $charset = 'abdefg';
    $random_char = Fluffer::fluffChar($charset);

    // Make sure the generated character is from the character set
    $this->assertStringOnlyContainsCharset($random_char, $charset);
  }

  /**
   * Dates should be randomly generated in the format YYYY-MM-DD.
   *
   * We do this one hundred times to make sure the dates are valid.
   */
  public function testRandomDate() {

    for ($i = 0; $i < 100; $i++) {
      // Generate a random date
      $random_date = Fluffer::fluffDate();

      // Break the generated date into pieces
      $parts = explode("-", $random_date);
      $random_year = $parts[0];
      $random_month = $parts[1];
      $random_day = $parts[2];

      // Make sure the first four digits are a year (0-9999)
      $this->assertTrue(is_numeric($random_year), "The year is a number");
      $this->assertGreaterThan(0, $random_year, "The year is greater than zero");

      // Make sure the six and seventh digits a month (01-12)
      $this->assertTrue(is_numeric($random_month), "The month must be a number");
      $this->assertGreaterThan(0, $random_month, "The month is always greater than zero.");
      $this->assertLessThanOrEqual(12, $random_month, "The month is always less or equal to 12");

      // Make sure the ninth and tenth digits are a day
      // Use a lookup table to figure out how many days should be in each month
      $days_in_month = array(
        "01" => 31,
        "02" => 28,
        "03" => 31,
        "04" => 30,
        "05" => 31,
        "06" => 30,
        "07" => 31,
        "08" => 31,
        "09" => 30,
        "10" => 31,
        "11" => 30,
        "12" => 31,
      );
      $this->assertTrue(is_numeric($random_day), "The day must be a number");
      $this->assertGreaterThan(0, $random_day);
      $days_in_random_month = $days_in_month[$random_month];
      $this->assertLessThanOrEqual($days_in_random_month, $random_day, "$random_day is to high.  The $random_month month doesn\'t have more than $days_in_random_month days in it!");
    }
  }

  /**
   * Given a set of enumerated values, one should be randomly selected.
   */
  public function testFluffEnum() {

    // Create an enumeration of possible values
    $enum = array('value1', 'value2', 'value3');

    // Create fluff by selecting one of the values from the enum
    $random_value = Fluffer::fluffEnum($enum);

    // The random value should have come from the enumeration
    $this->assertContains($random_value, $enum);
  }

  /**
   * Generate a random integer greater than zero.
   */
  public function testFluffInteger() {

    // Generate a number
    $random_integer = Fluffer::fluffInt();

    // Make sure a generated number is a number!
    $this->assertTrue(is_numeric($random_integer), "The random integer should be a number.");
    $this->assertGreaterThan(0, $random_integer);
  }
}
?>
