import java.util.Objects;

/**
 * Class responsible for constructing a book list for the LibraryData.
 *
 * The list type of this class can either be SHORT or LONG.
 *
 * Both list types will have a header display that displays the number of books in the library.
 *
 * The SHORT list will only display a list of the book titles, whereas the LONG list
 * will display the entire book information provided by the toString method in the
 * BookEntry class.
 *
 */
public class ListManager extends LibraryEntries {

    /** The list type that should be constructed*/
    private ListType type;
    /** The Header display for the list - It will display the number of books in the library*/
    private final String HEADER_DISPLAY = bookEntries.size() + " books in library:";;

    /**
     * Creates a list manager.
     *
     * @param data the book data that the list manager will operate on.
     * @param type the type of list that should be constructed.
     * @throws NullPointerException if the data or type is null.
     * @throws IllegalArgumentException if the type provided is not a valid list type.
     */
    public ListManager(LibraryData data, ListType type) {
        super(data);

        Objects.requireNonNull(type, "ERROR: The list type cannot be null.");

        if (!(type == ListType.SHORT || type == ListType.LONG)) {
            throw new IllegalArgumentException("ERROR: Invalid list type provided.");
        }

        this.type = type;
    }

    /**
     * Constructs the book list.
     *
     * Note: Considerations were made into creating two subclasses for the ListManager,
     * one for the short and long list respectively. In this current version of the
     * program this has not been done as the two lists are quite similar.
     *
     * @return A StringBuilder containing the list to be displayed.
     */
    public StringBuilder constructList() {
        StringBuilder list = new StringBuilder(HEADER_DISPLAY);

        for(BookEntry book : bookEntries) {
            if (type == ListType.LONG) {
                list.append("\n").append(book.toString()).append("\n");
            } else {
                list.append("\n").append(book.getTitle());
            }
        }
        return list;
    }

}
