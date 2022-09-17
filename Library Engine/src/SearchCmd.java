import java.util.List;
import java.util.Objects;
/**
 * Search command used to search the library for books.
 *
 * In this version of the program, it only considers search by title.
 */

public class SearchCmd extends LibraryCommand {

    /** This contains the search value provided by the user input - It is required for the execute method*/
    private String argument;

    /**
     * Creates a search command
     *
     * @param argumentInput the search value specified by the user input.
     * @throws IllegalArgumentException if given argumentInput is invalid as specified by the parseArguments method
     * @throws NullPointerException if the argumentInput provided is null
     */
    public SearchCmd(String argumentInput) { super(CommandType.SEARCH, argumentInput); }

    /**
     * Executes the search command.
     *
     * @param data book data containing the books to be considered for the search.
     * @throws NullPointerException if data provided is null.
     */
    @Override
    public void execute(LibraryData data) {
        SearchManager searcher = new SearchManager(data);
        List<String> searchResult = searcher.searchLibrary(argument);

        if (searchResult.isEmpty()) {
            System.out.println("No hits found for search term: " + argument);
        } else {
            searcher.printSearchResults();
        }
    }

    /**
     * Parses the user input.
     *
     * The user input for the search command is only allowed to be a single word
     * and it is not allowed to be entirely empty.
     *
     * @param argumentInput user input specifying the search value.
     * @return false if the given argument is blank or not a single word, true otherwise.
     * @throws NullPointerException if the given argumentInput is null.
     */
    @Override
    protected boolean parseArguments(String argumentInput) {
        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        this.argument = argumentInput.trim();

        if (argument.isBlank()) {
            return false;
        } else {
            return !(argument.contains(" "));
        }

    }
}
