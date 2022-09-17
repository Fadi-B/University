import java.util.Objects;
/**
 * List command used to display a list of all the books in the current library
 */
public class ListCmd extends LibraryCommand {

    /** This contains the list type provided by the user input - It is required for the execute method*/
    private ListType type;

    /**
     * Creates a list command
     *
     * @param argumentInput the list type specified by the user input.
     * @throws IllegalArgumentException if given argumentInput is invalid as specified by the parseArguments method
     * @throws NullPointerException if the argumentInput provided is null
     */
    public ListCmd(String argumentInput) {
        super(CommandType.LIST, argumentInput);

    }

    /**
     * Executes the list command.
     *
     * @param data book data that the book list should be based on.
     * @throws NullPointerException if data provided is null.
     */
    @Override
    public void execute(LibraryData data) {
        Objects.requireNonNull(data,"ERROR: The given book library must not be null!");

        if (data.getBookData().isEmpty()) {
            System.out.println(LibraryEntries.EMPTY_LIBRARY);
        } else {
            ListManager manager = new ListManager(data, type);
            System.out.println(manager.constructList());
        }
    }

    /**
     * Parses the user input
     *
     * The user input for the list command specifies the list to be constructed. In this version of the program,
     * the user input representing a valid list type is either "short" or "long". If no argumentInput is provided,
     * the list type will by default be assumed "short".
     *
     * Note: Only list types provided by the user input that are in lower case
     * are accepted. For example, SHORT or LONG are not considered valid input for this command even if the enum constants
     * in ListType are in upper case.
     *
     * @param argumentInput user input specifying the list type
     * @return true if the argumentInput is empty or if it is one of the allowed list types "short" or "long".
     * @throws NullPointerException if the given argumentInput is null.
     */
    @Override
    protected boolean parseArguments(String argumentInput) {
        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        String argument = argumentInput.trim();

        if (argumentInput.isBlank() || argument.equals(ListType.SHORT.name().toLowerCase())) {
            type = ListType.SHORT;
            return true;
        }

        if (argument.equals(ListType.LONG.name().toLowerCase())) {
            type = ListType.LONG;
            return true;
        }

        return false;
    }

}
