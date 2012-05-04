import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class mayan {

  private static final String ZeroSymbol = "@";
  private static final char OneSymbol = '*';
  private static final String FiveSymbol = "-----";

  enum Operator {
    None,
    Add,
    Subtract,
  }

  public static void main(String[] args) {
    File inFile = new File("mayan.in");
    if (inFile.exists()) {
      try {
        BufferedReader fin = new BufferedReader(new FileReader(inFile));
        BufferedWriter fout = new BufferedWriter(new FileWriter("mayan.out"));

        String line;
        List<Long> operands = new ArrayList<Long>(2);
        Operator operator = Operator.None;
        // loop until end of file
        while ( !"0".equals(line = fin.readLine()) ) {

          if ( "+".equals(line)) {
            operator = Operator.Add;
          }
          else if ( "-".equals(line) ) {
            operator = Operator.Subtract;
          }
          // a mayan number
          else {
            List<String> mayanNumber = new ArrayList<String>(4);
            // add an extra blank line to represent the end of the topmost digit
            mayanNumber.add("");
            // loop until end of numeral
            while ( !"#".equals(line) ) {
              mayanNumber.add(line);
              line = fin.readLine();
            }

            operands.add(convertMayanToConventional(mayanNumber));
          }

          // if we've got everything we need to perform an operation
          if ( operator != Operator.None && 2 == operands.size() ) {
            long result = 0;
            switch ( operator ) {
              case Add:
                result = operands.get(0) + operands.get(1);
                break;
              case Subtract:
                result = operands.get(0) - operands.get(1);
                break;
            }

            fout.write(convertConventionalToMayan(result) + "#\n");
            operator = Operator.None;
            operands.clear();
          }

        }

        fout.close();
        fin.close();
      } catch (IOException ex) {
        System.out.println("File IO failed: " + ex.getMessage());
      }
    } else {
      System.out.println("Couldn't open file 'mayan.in'");
    }

  }

  /**
   * Converts a Mayan base-20 numeral into a conventional numeral
   * @param numeral
   * An array of Mayan symbols starting from the top at the zero index to the last symbol at the n-th index
   * @return the numeral as an integer
   */
  private static long convertMayanToConventional(List<String> numeral) {
    long total = 0;
    // the cumulative value of the digit currently being parsed
    int digit = 0;
    // the 20's place of the digit currently being parsed
    int place = 0;

    // iter in reverse order to start at least significant digit
    for ( int i = numeral.size() - 1; i >= 0; --i ) {
      String symbol = numeral.get(i);

      if ( symbol.equals(FiveSymbol) ) {
        digit += 5;
      }
      else if ( symbol.length() > 0 && symbol.charAt(0) == OneSymbol ) {
        digit += countMatches( OneSymbol, symbol );
      }
      else if ( symbol.equals(ZeroSymbol)) {
      }
      // blank, signifies the end of the current digit, start of the next digit
      else {
        total += (place == 0 ? digit : digit * Math.pow( 20, place ));
        place++;
        digit = 0;
      }

    }

    return total;
  }

  /**
   * Converts a conventional base-20 numeral to a Mayan numeral
   * @param number the decimal number
   * @return the equivalent base-20 number in Mayan form
   */
  private static String convertConventionalToMayan(long number) {
    if ( 0 == number ) {
      return ZeroSymbol + "\n";
    }

    StringBuilder builder = new StringBuilder();

    // the amount remaining to convert
    long remaining = number;
    // flag to track & prevent leading zeroes from being printed out
    boolean trailingZeroes = true;

    // loop through each digit
    for ( int place = 4; place >= 0; --place ) {
      double div = (place == 0 ? 1 : Math.pow( 20, place ));
      int numberForPlace = (int)(remaining / div);
      remaining = (long)(remaining % div);

      // if nothing was printed for the current digit we shouldn't append a newline
      boolean nothingWasPrinted = false;
      // print the digit
      if ( 0 == numberForPlace ) {
        if ( !trailingZeroes ) {
          builder.append(ZeroSymbol).append("\n");
        }
        else {
          nothingWasPrinted = true;
        }
      }
      else {
        trailingZeroes = false;
        if ( 0 < numberForPlace % 5 ) {
          for ( int i = 0; i < numberForPlace % 5; ++i ) {
            builder.append(OneSymbol);
          }

          builder.append("\n");
        }
        while ( numberForPlace >= 5 ) {
          numberForPlace -= 5;
          builder.append(FiveSymbol).append("\n");
        }
      }


      // don't append trailing newline
      if ( 0 != place && !nothingWasPrinted ) {
        builder.append("\n");
      }
    }

    return builder.toString();
  }

  /**
   * Counts the number of times a characters appears within another string
   * @param needle the characters to search for
   * @param haystack the string to search in
   * @return the number of times that the searched character was found
   */
  private static int countMatches(char needle, String haystack) {
    int count = 0;

    for ( int i = 0; i < haystack.length(); ++i ) {
      if ( needle == haystack.charAt(i) ) {
        count++;
      }
    }

    return count;
  }
}
