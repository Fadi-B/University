import java.nio.file.Paths;
import java.util.Objects;

/**
 * Add command used to add books to the library
 *
 * Note: In this version of the program, only the contents of csv files can be added to the library.
 *
 */

public class AddCmd extends LibraryCommand {

    /** This contains the path provided by the user input to the book file - It is required for the execute method*/
    private String argument;

    /**
     *
     * Creates an add command
     *
     * @param argumentInput the user input specifying the path of the book file that contains the books to be added.
     * @throws IllegalArgumentException if given argumentInput is invalid as specified by the parseArguments method
     * @throws NullPointerException if the argumentInput provided is null
     */
    public AddCmd(String argumentInput) {
        super(CommandType.ADD, argumentInput);
    }

    /**
     * Executes the add command.
     *
     * @param data the book data that the contents of the book file should be added to.
     * @throws NullPointerException if data provided is null.
     */
    @Override
    public void execute(LibraryData data) {
        Objects.requireNonNull(data, "ERROR: The given book library must not be null!");

        data.loadData(Paths.get(argument));

    }

    /**
     * Parses the user input
     *
     * The user input for the add command specifies the path of the
     * book file that contains the books to be added to the library.
     *
     * Only csv files are currently considered valid book files.
     * Hence, the user input is only valid if it specifies the path of a csv file.
     *
     * @param argumentInput the user input specifying the path to the csv file
     * @return true if the argumentInput specifies a csv file, i.e the user input ends with ".csv"
     * @throws NullPointerException if the given argumentInput is null.
     */
    @Override
    protected boolean parseArguments(String argumentInput) {
        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        final String SUFFIX = ".csv";

        this.argument = argumentInput.trim();

        if (argument.length() < SUFFIX.length()) {
            return false;
        }

        String suffix = argument.substring(argument.length() - SUFFIX.length());

        return suffix.equals(SUFFIX);

    }
}
