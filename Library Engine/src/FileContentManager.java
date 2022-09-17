import java.util.List;
import java.util.Objects;

/**
 * Class responsible for managing the file content loaded
 * by the LibraryFileLoader class.
 *
 * The file content from the LibraryFileLoader stores the book data in a list, in which each element represents
 * a line in the file. Each line contains information relating to one book and has the following
 * format: <title>,<Author1-Author2-Author3>,<Rating>,<ISBN>,<Pages>
 *
 * This class provides the functionality of obtaining the desired book information from the file content.
 *
 * Note: The first line of the file content will always be a header,
 * which means that the relevant book information starts at index 1 in the file content.
 */

public class FileContentManager {

    /** The index of the book title in each line of the file content */
    private final int BOOK_TITLE_INDEX = 0;
    /** The index of the authors of the book in each line of the file content */
    private final int BOOK_AUTHORS_INDEX = 1;
    /** The index of the book rating in each line of the file content */
    private final int BOOK_RATING_INDEX = 2;
    /** The index of book ISBN in each line of the file content */
    private final int BOOK_ISBN_INDEX = 3;
    /** The index of book pages in each line of the file content */
    private final int BOOK_PAGES_INDEX = 4;

    /** The data values of the book in each line are separated by a comma*/
    private final String NEW_LINE_INDICATOR = ",";
    /** If a book has multiple authors then each author will be separated by a hyphen*/
    private final String NEW_AUTHOR_INDICATOR = "-";

    /** The file content loaded from the libraryFileLoader that contains the book data - This should not be edited*/
    private final List<String> fileContent;

    /**
     * Creates a file content manager.
     *
     * @param fileContent the file content read from the book files that the manager should operate on.
     * @throws NullPointerException if the file content is null.
     */
    public FileContentManager(List<String> fileContent) {
        Objects.requireNonNull(fileContent, "ERROR: File content cannot be null.");

        this.fileContent = fileContent;
    }

    /**
     * Gets the specified line in the file content. The first line is a header so the specified
     * must be larger than 0. The specified information must also be less than the size of the file content.
     *
     * @param index the index of the line in the file content that needs retrieval.
     * @return an array containing the data values of the book
     * @throws IllegalArgumentException if a line in the file content is null and if the index is less than 0 or larger than the size of the file content.
     */
    public String[] getLine(int index) {
        if (index < fileContent.size() && index > 0) {
            if (fileContent.get(index) == null) {
                throw new IllegalArgumentException("ERROR: The file content cannot contain null lines.");
            } else {
                return fileContent.get(index).split(NEW_LINE_INDICATOR);
            }
        } else {
            throw new IllegalArgumentException("ERROR: The index provided cannot exceed the size of the fileContent and must be less than zero.");
        }
    }

    /**
     * Checks if one of the lines contains empty information.
     *
     * @param line the array containing the data values of the book
     * @return a boolean indicating whether the line contains any empty elements
     * @throws NullPointerException if the line provided is null
     */
    public boolean isEmptyLine(String[] line) {
        Objects.requireNonNull(line, "ERROR: The line containing book information cannot be null.");

        for (String bookInfo : line) {
            if (bookInfo.isBlank()) {
                return true;
            }
        }
        return false;
    }

    /**
     * Retrieves the information for the book title from the specified line.
     *
     * @param line the array containing the data values of the book.
     * @return the title of the book.
     * @throws NullPointerException if the line provided is null.
     */
    public String getTitle(String[] line) {
        Objects.requireNonNull(line, "ERROR: The line containing book information cannot be null.");
        return line[BOOK_TITLE_INDEX];
    }

    /**
     * Retrieves the information for the authors of the book from the specified line.
     *
     * @param line the array containing the data values of the book.
     * @return the authors of the book.
     * @throws NullPointerException if the line provided is null.
     */
    public String[] getAuthors(String[] line) {
        Objects.requireNonNull(line, "ERROR: The line containing book information cannot be null.");
        return line[BOOK_AUTHORS_INDEX].split(NEW_AUTHOR_INDICATOR);
    }

    /**
     * Retrieves the information for the book rating from the specified line.
     *
     * @param line the array containing the data values of the book.
     * @return the rating of the book.
     * @throws NullPointerException if the line provided is null.
     */
    public float getRating(String[] line) {
        Objects.requireNonNull(line, "ERROR: The line containing book information cannot be null.");
        return Float.parseFloat(line[BOOK_RATING_INDEX]);
    }

    /**
     * Retrieves the information for the book ISBN from the specified line.
     *
     * @param line the array containing the data values of the book.
     * @return the ISBN of the book.
     * @throws NullPointerException if the line provided is null.
     */
    public String getISBN(String[] line) {
        Objects.requireNonNull(line, "ERROR: The line containing book information cannot be null.");
        return line[BOOK_ISBN_INDEX];
    }

    /**
     * Retrieves the information for the number of book pages from the specified line.
     *
     * @param line the array containing the data values of the book.
     * @return the number of pages of the book.
     * @throws NullPointerException if the line provided is null.
     * */
    public int getPages(String[] line) {
        Objects.requireNonNull(line, "ERROR: The line containing book information cannot be null.");
        return Integer.parseInt(line[BOOK_PAGES_INDEX]);
    }

    /**
     * The file content being used by this FileContentManager.
     * @return the file content being used.
     */
    public List<String> getFileContent() { return fileContent; }

}
