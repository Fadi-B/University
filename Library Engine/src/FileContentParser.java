import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Class responsible for parsing the file content. It will convert the
 * book data into a list of book entries that can be used by the different
 * library commands.
 *
 * Note: The file content is not allowed to be null - The check for this is
 * done in the FileContentManager class
 *
 */

public class FileContentParser {

    /** This FileContentManager will allow the FileContentParser to perform operations
     * on the file content */
    private FileContentManager contentManager;

    /** The list containing the book data in the file content converted into book entries*/
    private List<BookEntry> bookEntries;

    /**
     * Creates a file content parser
     *
     * @param fileContent the file content read from the book files that should be parsed
     * @throws NullPointerException if file content is null
     */
    public FileContentParser(List<String> fileContent) {
        this.contentManager = new FileContentManager(fileContent);
        this.bookEntries = new ArrayList<>();
    }

    /**
     * Parses the file content.
     *
     * Note: The for loop starts at index 1 since the first line in the file content
     * is a header and not actual book data.
     *
     * @return the list containing the book data parsed into book entries
     */
    public List<BookEntry> parseFileContent() {

        int fileContentSize = contentManager.getFileContent().size();

        for (int i = 1; i < fileContentSize; i++) {
            String[] line = contentManager.getLine(i);

            if (contentManager.isEmptyLine(line)) {
                continue;
            }

            BookEntry book = createEntry(line);
            bookEntries.add(book);
        }
        return bookEntries;
    }

    /**
     * Creates a book entry based on the data values provided by a line in the file content
     *
     * @param line the array containing the data values of the book, which is obtained by the getLine method
     *             provided by the FileContentManager
     * @return the book entry parsed from the book data contained within a line
     * @throws NullPointerException if the line provided is null
     */
    private BookEntry createEntry(String[] line) {
        Objects.requireNonNull(line, "ERROR: The line containing book information cannot be null.");

        return new BookEntry(contentManager.getTitle(line), contentManager.getAuthors(line),
               contentManager.getRating(line), contentManager.getISBN(line), contentManager.getPages(line));
    }

}
