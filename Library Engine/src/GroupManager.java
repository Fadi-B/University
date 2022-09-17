import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

/**
 * Super class for the classes responsible for managing a certain type of book grouping.
 */

public abstract class GroupManager extends LibraryEntries {

    /** The prefix shown next to each group on the display - Can be edited to suit needs*/
    protected final String GROUP_PREFIX = "## ";
    /** The amount that each book title in the group should be indented by on the display - Can be edited to suit needs*/
    protected final String INDENT = "    ";

    /** The type of this group manager*/
    private GroupType type;

    /**
     * Creates the appropriate group manager as specified by the group type.
     *
     * @param data the library data containing the books that the grouping should be based on.
     * @param type the group manager type.
     * @throws NullPointerException if any of the given parameters are null.
     */
    public GroupManager(LibraryData data, GroupType type) {
        super(data);

        Objects.requireNonNull(type, "ERROR: Given type must not be null.");

        this.type = type;
    }

    /**
     * Constructs the specific groups.
     * The map returned will hold the groups. The key will store the group and will map to a list of books belonging to that group.
     *
     * Subclasses must override this method to specify the way in which
     * the specific groups are to be constructed.
     *
     * Note: The use of the linkedHashMap is so that the group construction will be in the control of the insertion order. Considerations
     * of using a TreeMap were made, but the idea was that controlling the insertion order provides more freedom for constructing more
     * complex groups in future versions of the program.
     *
     * @return map containing the groups.
     */
    public abstract LinkedHashMap<String, List<BookEntry>> constructGroups();

    /**
     * Constructs the group display that should be printed to the user.
     *
     * @param groups a linkedHashMap containing the groups that the display should be based on.
     * @return a string builder containing the group display to be printed.
     * @throws NullPointerException If the groups provided is null.
     */
    public StringBuilder constructGroupDisplay(LinkedHashMap<String, List<BookEntry>> groups) {
        Objects.requireNonNull(groups, "ERROR: The groups cannot be null.");

        StringBuilder groupDisplay = new StringBuilder("Grouped data by " + type.toString());

        for (Map.Entry<String, List<BookEntry>> group : groups.entrySet()) {

            if (!group.getValue().isEmpty()) { //Not empty means there are books in that group
                groupDisplay.append("\n").append(GROUP_PREFIX).append(group.getKey());
                addBookTitles(groupDisplay, group.getValue());
            }
        }
        return groupDisplay;
    }

    /**
     * Adds the book titles of the books within a list to a string builder.
     *
     * @param groupDisplay the string builder that the book titles should be appended to.
     * @param books the list containing the books whose titles are to be added to the group display.
     * @throws NullPointerException If any of the parameters provided are null.
     */
    private void addBookTitles(StringBuilder groupDisplay, List<BookEntry> books) {
        Objects.requireNonNull(books, "ERROR: The books cannot be null.");
        Objects.requireNonNull(groupDisplay, "ERROR: The string builder cannot be null.");

        for (BookEntry book : books) {
            groupDisplay.append("\n").append(INDENT).append(book.getTitle());
        }
    }
}
