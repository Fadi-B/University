import java.util.LinkedHashMap;
import java.util.List;
import java.util.Objects;
/**
 * Group command used to group books that are in the current library
 *
 * In this version, the group command only considers lexicographical title grouping and author grouping
 */
public class GroupCmd extends LibraryCommand {

    /** The group type specified by the user input - AUTHOR|TITLE*/
    private GroupType type;

    /**
     * Creates a group command.
     *
     * @param argumentInput the user input determining the grouping of the books - AUTHOR|TITLE.
     * @throws IllegalArgumentException if given argumentInput is invalid as specified by the parseArguments method.
     * @throws NullPointerException if the argumentInput provided is null.
     */
    public GroupCmd(String argumentInput) {
        super(CommandType.GROUP, argumentInput);
    }

    /**
     * Executes the group command.
     *
     * @param data book data containing the books to be grouped.
     * @throws NullPointerException if data provided is null.
     */
    @Override
    public void execute(LibraryData data) {
        Objects.requireNonNull(data,"ERROR: The given book library must not be null!");

        if (data.getBookData().isEmpty()) {
            System.out.println(LibraryEntries.EMPTY_LIBRARY);
        } else {
            GroupManager manager = GroupManagerFactory.createManager(data, type);
            LinkedHashMap<String, List<BookEntry>> groups = manager.constructGroups();
            System.out.println(manager.constructGroupDisplay(groups));
        }

    }

    /**
     * Parses the user input
     *
     * The user input for the group command specifies the group type.
     * The group type must be one of the valid group types - AUTHOR|TITLE.
     *
     * @param argumentInput the user input specifying the group type.
     * @return true if the argumentInput is a valid group type and false otherwise.
     * @throws NullPointerException if the given argumentInput is null.
     */
    @Override
    protected boolean parseArguments(String argumentInput) {
        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        String argument = argumentInput.trim();

        if (argument.equals(GroupType.AUTHOR.name()) || argument.equals(GroupType.TITLE.name())) {
            type = GroupType.valueOf(argument);
            return true;
        }

        return false;
    }
}
