import java.util.Arrays;
import java.util.Objects;

/**
 * Author remove manager used to remove the books in the library written by a specific author.
 *
 * Note: The removal is case sensitive.
 */
public class AuthorRemoveManager extends RemoveManager {

    /** The user input containing the remove value to be considered during the book removal - In this case, it should be the author name*/
    private String argumentInput;

    /**
     * Creates a remove manager for authors
     *
     * @param data the library data that the books should be removed from
     * @param argumentInput the user input specifying the author of the books to be removed
     * @throws NullPointerException if any of the given parameters are null.
     */
    public AuthorRemoveManager(LibraryData data, String argumentInput) {
        super(data, RemoveType.AUTHOR);

        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        this.argumentInput = argumentInput;
    }

    /**
     * This removes the the books written by the specified author from the library.
     */
    @Override
    public void remove() {
        while (bookIterator.hasNext()) {
            BookEntry entry = bookIterator.next();
            if (Arrays.asList(entry.getAuthors()).contains(argumentInput)) {
                removeBook(entry);
            }
        }
    }

    /**
     * Prints the result of attempting to remove the books written by the specified author.
     */
    @Override
    public void printResults() {
        System.out.println(removedBooks.size() + " books removed for author: " + argumentInput);
    }
}
