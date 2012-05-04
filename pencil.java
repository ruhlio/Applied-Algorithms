// Jeff Ruhl
// Pencil Pushing

import java.io.*;
import java.util.*;

public class pencil {

  private static class PencilPosition {
    /**
     * The row in the maze
     */
    public final int row;
    /**
     * The column in the maze
     */
    public final int column;
    /**
     * Whether the pencil tip is facing upwards or not
     */
    public final boolean isUp;

    /**
     * The previous position used to reach the current one. Forms a linked list that forms the entire route
     * taken to this position
     */
    public final PencilPosition previous;

    public PencilPosition( int row, int column, boolean isUp, PencilPosition previous ) {
      this.row = row;
      this.column = column;
      this.isUp = isUp;
      this.previous = previous;
    }

    @Override
    public boolean equals( Object other ) {
      boolean equal = false;

      if ( null != other && other instanceof PencilPosition ) {
        PencilPosition otherPos = (PencilPosition)other;
        equal = (otherPos.row == this.row && otherPos.column == this.column && otherPos.isUp == this.isUp);
      }

      return equal;
    }

    @Override
    public int hashCode() {
      int result = row;
      result = 31 * result + column;
      result = 31 * result + (isUp ? 1 : 0);
      return result;
    }

  }

  private static class PencilPositionComparator implements Comparator<PencilPosition> {
    @Override
    public int compare(PencilPosition lhs, PencilPosition rhs) {
      return Integer.compare( getPencilPositionLength(lhs), getPencilPositionLength(rhs) );
    }

    private int getPencilPositionLength( PencilPosition position ) {
      int length;

      if ( null == position.previous ) {
        length = 1;
      }
      else {
        length = 1 + getPencilPositionLength(position.previous);
      }

      return length;
    }

  }

  public static void main(String[] args) {
    try {
      Scanner fin = new Scanner(new File("pencil.in"));
      BufferedWriter fout = new BufferedWriter(new FileWriter("pencil.out"));

      int mazeSize = 0;
      int caseNumber = 0;
      while ( 0 != (mazeSize = fin.nextInt()) ) {
        ++caseNumber;

        int[][] maze = new int[mazeSize][mazeSize];
        for ( int row = 0; row < mazeSize; ++row ) {
          for ( int col = 0; col < mazeSize; ++col ) {
            maze[row][col] = fin.nextInt();
          }
        }

        PencilPosition shortestRoute = findShortestRoute(maze);
        if ( 1 != caseNumber ) {
          fout.write("\n");
        }
        fout.write("Case Number #" + caseNumber + "\n");
        if ( null == shortestRoute ) {
          fout.write("No exit found!\n");
        }
        else {
          fout.write( formatRoute( shortestRoute ) );
          fout.write("->EXIT!\n");
        }

      }

      fout.close();
      fin.close();
    }
    catch (IOException ex) {
      System.err.println("Error reading/writing file 'pencil.(in|out)': " + ex.getMessage());
    }

  }

  private static String formatRoute( PencilPosition route ) {
    String formattedPosition = "(" + route.row + "," + route.column + "," + (route.isUp ? "Up" : "Dn") + ")";

    if ( null != route.previous ) {
      return formatRoute( route.previous ) + "->" + formattedPosition;
    }

    return formattedPosition;
  }

  private static PencilPosition findShortestRoute( int[][] maze ) {
    // all found solutions to the maze
    PriorityQueue<PencilPosition> solutions = new PriorityQueue<PencilPosition>( 5, new PencilPositionComparator() );
    // bread-first search queue
    Queue<PencilPosition> routes = new LinkedList<PencilPosition>();
    // set of already visited positions
    Set<PencilPosition> visitedPositions = new HashSet<PencilPosition>();

    // add the starting positions, which is always (0,0)
    routes.add( new PencilPosition(0, 0, false, null) );

    while ( !routes.isEmpty() ) {
      PencilPosition position = routes.poll();

      // if this is the destinations position then we've found a solution
      if ( 0 == maze[position.row][position.column] ) {
        solutions.add( position );
        continue;
      }

      // if we haven't already visited this position
      if ( !visitedPositions.contains(position) ) {
        routes.addAll( findPossibleRoutes( position, maze ) );
        visitedPositions.add( position );
      }
    }

    return solutions.poll();
  }

  /**
   * Finds all routes that can be taken in a maze from a position
   * @param from the starting position to look for routes from
   * @return all possible routes in the maze from the starting position
   */
  private static List<PencilPosition> findPossibleRoutes( PencilPosition from, int[][] maze ) {
    List<PencilPosition> possibleRoutes = new ArrayList<PencilPosition>(4);
    int fromValue = maze[from.row][from.column];
    boolean isUp = from.isUp;
    // if the from position inverts the orientation of the pencil (which determines which directions can be taken)
    if ( 0 > fromValue ) {
      isUp = !isUp;
      fromValue = Math.abs( fromValue );
    }

    boolean canMoveUp = (0 <= from.row - fromValue);
    boolean canMoveDown = (maze.length > from.row + fromValue);
    boolean canMoveLeft = (0 <= from.column - fromValue);
    boolean canMoveRight = (maze.length > from.column + fromValue);

    if ( isUp ) {
      // move diagonally

      if ( canMoveUp && canMoveLeft ) {
        possibleRoutes.add( new PencilPosition(from.row - fromValue, from.column - fromValue, isUp, from) );
      }
      if ( canMoveUp && canMoveRight ) {
        possibleRoutes.add( new PencilPosition(from.row - fromValue, from.column + fromValue, isUp, from) );
      }
      if ( canMoveDown && canMoveLeft ) {
        possibleRoutes.add( new PencilPosition(from.row + fromValue, from.column - fromValue, isUp, from) );
      }
      if ( canMoveDown && canMoveRight ) {
        possibleRoutes.add( new PencilPosition(from.row + fromValue, from.column + fromValue, isUp, from) );
      }
    }
    else {
      // move horizontally

      if ( canMoveUp ) {
        possibleRoutes.add( new PencilPosition(from.row - fromValue, from.column, isUp, from) );
      }
      if ( canMoveDown ) {
        possibleRoutes.add( new PencilPosition(from.row + fromValue, from.column, isUp, from) );
      }
      if ( canMoveLeft ) {
        possibleRoutes.add( new PencilPosition(from.row, from.column - fromValue, isUp, from) );
      }
      if ( canMoveRight ) {
        possibleRoutes.add( new PencilPosition(from.row, from.column + fromValue, isUp, from) );
      }
    }

    return possibleRoutes;
  }

}
