import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Class responsible for searching the LibraryData for the String provided by the user input.
 *
 * The search value provided by the user can only be a single word and is not allowed to be entirely empty.
 *
 * In this version of the program, only the titles of the book entries will be considered in the search.
 *
 * Note: The search is not case sensitive.
 *
 */

public class SearchManager extends LibraryEntries {

    /**This list will contain all the book titles that the search value is a substring of in the order that they were found*/
    private List<String> searchResult;

    /**
     * Creates a search manager.
     *
     * @param data the library data that contains the book entries to search.
     * @throws NullPointerException if the data provided is null.
     */
    public SearchManager(LibraryData data) {
        super(data);
        this.searchResult = new ArrayList<>();
    }

    /**
     * Searches through all the book titles in the library data and updates
     * the searchResult if a hit is found.
     *
     * @param argumentInput the search value specified by the user input that will be used in the search
     * @return the searchResult list
     */
    public List<String> searchLibrary(String argumentInput) {
        Objects.requireNonNull(argumentInput, "ERROR: Given input argument must not be null.");

        for (BookEntry book : bookEntries) {
            String targetTitle = book.getTitle();
            boolean found = targetTitle.toLowerCase().contains(argumentInput.toLowerCase()); //Not case sensitive
            if (found) {
                searchResult.add(targetTitle);
            }
        }
        return searchResult;
    }

    /**
     * This prints the searchResult to the console output
     * in the order that they were found such that each hit is shown
     * on a separate line.
     */
    public void printSearchResults() {
        for (String result : searchResult) {
            System.out.println(result);
        }
    }
}
