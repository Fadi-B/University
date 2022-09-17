import java.util.Objects;
/**
 * Title remove manager used to remove the book with the specified title from the library.
 *
 * Note: The removal is case sensitive and assumes that book titles within the library are unique.
 */
public class TitleRemoveManager extends RemoveManager {

    /** The user input containing the remove value to be considered during the book removal - In this case, it should be the title of the book*/
    private String argumentInput;

    /**
     * Creates a remove manager for titles
     *
     * @param data the library data that the book should be removed from
     * @param argumentInput the user input specifying the title of the book to be removed
     * @throws NullPointerException if any of the given parameters are null.
     */
    public TitleRemoveManager(LibraryData data, String argumentInput) {
        super(data, RemoveType.TITLE);

        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        this.argumentInput = argumentInput;
    }

    /**
     * This removes the book with the specified title from the library.
     *
     * Note: There are no duplicate book titles in the library
     */
    @Override
    public void remove() {
        while (bookIterator.hasNext()) {
            BookEntry book = bookIterator.next();
            if (book.getTitle().equals(argumentInput)) {
                removeBook(book);
            }
            if (!removedBooks.isEmpty()) { //Not empty means the book has successfully been removed.
                break;
            }

        }
    }

    /**
     * Prints the result of the attempting to remove the book with the specified title
     */
    @Override
    public void printResults() {
        if (removedBooks.isEmpty()) {
            System.out.println(argumentInput + ": not found.");
        } else {
            System.out.println(argumentInput + ": removed successfully.");
        }
    }
}

