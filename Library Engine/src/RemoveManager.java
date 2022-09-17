import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Objects;

/**
 * Super class for the classes responsible for managing a certain type of book removal from the library.
 *
 * It ties their creation with an iterator for the books within the library data, but also a list
 * containing the books that have been removed.
 */

public abstract class RemoveManager extends LibraryEntries {

    /** This is the iterator responsible for iterating through the library during the removal*/
    protected Iterator<BookEntry> bookIterator;
    /** This list contains the removed books*/
    protected List<BookEntry> removedBooks;

    /** The minimum user input arguments for the remove command - One specifying the type AUTHOR|TITLE and at least one for the removal value*/
    public static final int MIN_REMOVE_ARGUMENTS = 2;

    /**
     * Creates the appropriate remove manager as specified by
     * the remove type.
     *
     * @param data the library data containing the books that the removal should be based on.
     * @param type the remove manager type.
     * @throws NullPointerException if any of the given parameters are null.
     */
    public RemoveManager(LibraryData data, RemoveType type) {
        super(data);

        Objects.requireNonNull(type, "ERROR: Given type must not be null.");

        this.removedBooks = new ArrayList<>();
        this.bookIterator = bookEntries.iterator();
    }

    /**
     * Removes the specified book from the library and updates the
     * removed books list.
     *
     * @param book the book entry to be removed from the library
     * @throws NullPointerException if the book provided is null
     */
    protected void removeBook(BookEntry book) {
        Objects.requireNonNull(book, "ERROR: Book cannot be null.");

        bookIterator.remove();
        removedBooks.add(book);
    }

    /**
     * Gets the books that have been removed from the library.
     *
     * This method is currently not being used in this version of the program, but is a useful
     * functionality for future versions
     *
     * @return list containing the removed books.
     */
    public List<BookEntry> getRemovedBooks() {
        return removedBooks;
    }

    /**
     * Removes the books from the Library
     *
     * Subclasses must override this method to specify corresponding behaviour.
     */
    public abstract void remove();

    /**
     * Prints the result of the removal
     *
     * Subclasses must override this method to specify corresponding behaviour.
     */
    public abstract void printResults();

}
