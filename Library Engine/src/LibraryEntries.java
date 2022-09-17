import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.TreeSet;

/**
 * Class responsible for the books within the library.
 *
 * In this current version, this class provides the functionality of obtaining all the books and book authors
 * in the library.
 *
 * In later versions other useful functionality such as obtaining all the book ratings, book titles or even book pages might be provided.
 *
 */

public class LibraryEntries {

    /** This list will contain all the books in the Library*/
    protected List<BookEntry> bookEntries;

    /** This is the console output that should be displayed for some commands if the LibraryData is empty*/
    public static final String EMPTY_LIBRARY = "The library has no book entries.";

    /**
     * Creates a library entries
     *
     * @param data the library data that contains the books this class should operate on.
     * @throws NullPointerException if the library data provided is null.
     */
    public LibraryEntries(LibraryData data) {
        Objects.requireNonNull(data,"ERROR: The given book library must not be null!");

        this.bookEntries = data.getBookData();
    }

    /**
     * Gets all the authors in the library and
     * ensures that they are sorted in an ascending order by name.
     * The sorting is case sensitive. A-Z is followed by author names starting with a-z.
     *
     * A TreeSet is used to ensure that the authors are sorted and do not come in
     * duplicates if they have written multiple books.
     *
     * @return a set containing the library authors in a sorted manner
     */
    public TreeSet<String> getLibraryAuthors() {
        TreeSet<String> uniqueAuthors = new TreeSet<>();

        for (BookEntry book : bookEntries) {
            List<String> authors = Arrays.asList(book.getAuthors());
            uniqueAuthors.addAll(authors);
        }
        return uniqueAuthors;
    }

    /**
     * @return the list containing all the books in the library.
     */
    public List<BookEntry> getBookEntries() {
        return bookEntries;
    }

}
