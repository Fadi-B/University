import java.util.Objects;
/**
 * Remove command used to remove books from the current library
 *
 * In this version, the removal only considers book titles and authors
 */
public class RemoveCmd extends LibraryCommand{

    /** The remove type specified by the user input - AUTHOR|TITLE*/
    private RemoveType argumentType;
    /**The remove value specified by the user input that the removal will be based on*/
    private String argumentValue;

    /**
     * Creates a remove command
     *
     * @param argumentInput the user input containing both the remove type and the remove value
     * @throws IllegalArgumentException if given argumentInput is invalid as specified by the parseArguments method
     * @throws NullPointerException if the argumentInput provided is null
     */
    public RemoveCmd(String argumentInput) {
        super(CommandType.REMOVE, argumentInput);
    }

    /**
     * Executes the remove command.
     *
     * @param data book data to remove books from.
     * @throws NullPointerException if data provided is null.
     */
    @Override
    public void execute(LibraryData data) {

        RemoveManager manager = RemoveManagerFactory.createManager(data, argumentType, argumentValue);

        manager.remove();
        manager.printResults();
    }

    /**
     * Parses the user input
     *
     * The user input for the remove command consists of a remove type followed by a remove value.
     * Therefore, the argumentInput must be at least of length 2.
     * The remove type must be one of the valid remove types - AUTHOR|TITLE.
     *
     * @param argumentInput the user input containing the remove- type and value
     * @return true if the remove type is a valid remove type and is followed by a remove value, false otherwise.
     * @throws NullPointerException if the given argumentInput is null.
     */
    @Override
    protected boolean parseArguments(String argumentInput) {
        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        String argument = argumentInput.trim();
        String[] arguments = argument.split(" ");

        if (arguments.length < RemoveManager.MIN_REMOVE_ARGUMENTS) {
            return false;
        }

        int splitIndex = argument.indexOf(' ');

        String argumentType = argument.substring(0, splitIndex);
        argumentValue = argument.substring(splitIndex + 1);

        if (argumentType.equals(RemoveType.AUTHOR.name()) || argumentType.equals(RemoveType.TITLE.name())) {
            this.argumentType = RemoveType.valueOf(argumentType);
            return true;
        }

        return false;
    }
}
