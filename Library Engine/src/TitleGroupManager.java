import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Objects;

/**
 * Title group manager used to group books by the lexicographical order of their titles.
 *
 * The books are grouped into groups A-Z and [0-9] with the first letter in the book title indicating which
 * group a book belongs to. If a book title begins with a digit it belongs to the digit group [0-9].
 *
 * Note: The grouping is not case sensitive and the digit group is the last group in the grouping.
 */

public class TitleGroupManager extends GroupManager {

    /** The number of groups - A-Z and [0-9]*/
    private final int TOTAL_GROUPS = 27;
    /** The string representing the group for all the books that start with a digit*/
    private final String DIGIT_GROUP = "[0-9]";
    /** The index at which the digit group is located - It is the last group*/
    private final int DIGIT_GROUP_INDEX = 26;
    /**The lexicographical grouping will start from group A*/
    private final char START_GROUP = 'A';

    /**
     * Creates a group manager for titles.
     *
     * @param data the book data that the grouping should be based on.
     * @throws NullPointerException if the data provided is null.
     */
    public TitleGroupManager(LibraryData data) {
        super(data, GroupType.TITLE);
    }

    /**
     * Constructs the title grouping.
     *
     * @return a map that maps the group to its corresponding books.
     */
    @Override
    public LinkedHashMap<String, List<BookEntry>> constructGroups() {
        LinkedHashMap<String, List<BookEntry>> groups = new LinkedHashMap<>();

        for (int i = 0; i < TOTAL_GROUPS; i++) {
            String group = i == DIGIT_GROUP_INDEX ? DIGIT_GROUP : Character.toString(START_GROUP + i);
            List<BookEntry> books = getGroupBooks(group);
            groups.put(group, books);
        }
        return groups;
    }

    /**
     * Gets all the books that belong to certain group.
     *
     * @param group the group of the books that should be obtained.
     * @return list containing all the books corresponding to a certain group.
     * @throws NullPointerException if the group provided is null.
     */
    private List<BookEntry> getGroupBooks(String group) {
        Objects.requireNonNull(group, "ERROR: The specified group cannot be null.");

        List<BookEntry> books = new ArrayList<>();

        for (BookEntry book : bookEntries) {
            String titleInitial = book.getTitle().substring(0, 1).toUpperCase(); //Not case sensitive

            if (group.equals(DIGIT_GROUP)) {
                if (Character.isDigit(titleInitial.charAt(0))) {
                    books.add(book);
                }
            } else {
                if (titleInitial.equals(group)) {
                    books.add(book);
                }
            }
        }
        return books;
    }
}