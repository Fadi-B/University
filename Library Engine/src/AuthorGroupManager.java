import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Objects;
import java.util.Arrays;
import java.util.TreeSet;

/**
 * Author group manager used to group books by the lexicographical order of full author names.
 *
 * Note: The grouping is case sensitive, meaning the lexicographical order will be A-Z followed by a-z if
 *       author names start with lower case letters.
 */

public class AuthorGroupManager extends GroupManager {

    /**
     * Creates a group manager for authors
     *
     * @param data the book data that the grouping should be based on.
     * @throws NullPointerException if the data provided is null.
     */
    public AuthorGroupManager(LibraryData data) {
        super(data, GroupType.AUTHOR);
    }

    /**
     * Constructs the author grouping.
     *
     * @return a map that maps the author to the books he or she has written.
     */
    @Override
    public LinkedHashMap<String, List<BookEntry>> constructGroups() {
        LinkedHashMap<String, List<BookEntry>> groups = new LinkedHashMap<>();
        TreeSet<String> authors = getLibraryAuthors(); //Sorted set from the LibraryEntries class

        for (String author : authors) {
             List<BookEntry> books = getBooksWritten(author);
             groups.put(author, books);
            }
        return groups;
    }

    /**
     * Gets all the books written by a certain author
     *
     * @param author the author of the books that should be retrieved.
     * @return list containing all the books written by a certain author.
     * @throws NullPointerException If the author provided is null.
     */
    private List<BookEntry> getBooksWritten(String author) {
        Objects.requireNonNull(author, "ERROR: The author of a book cannot be null.");

        List<BookEntry> books = new ArrayList<>();

        for (BookEntry book : bookEntries) {
            if (Arrays.asList(book.getAuthors()).contains(author)) {
                books.add(book);
            }
        }
        return books;
    }

}