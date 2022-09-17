import java.util.Arrays;
import java.util.Objects;
/**
 * Immutable class encapsulating data for a single book entry.
 */
public class BookEntry implements Comparable<BookEntry>{

    /** The title of the book entry*/
    private String title;
    /** The authors of the book entry - This will contain full names, such as Alex Jones */
    private String[] authors;
    /** The rating of the book entry*/
    private float rating;
    /** The International Standard Book Number of the book entry*/
    private String ISBN;
    /** The number of pages of the book entry*/
    private int pages;

    /**
     * Creates a book entry
     *
     * @param title string representing the title of the book.
     * @param authors string array representing the authors of the book.
     * @param rating float representing the rating of the book.
     * @param ISBN string representing the ISBN of the book.
     * @param pages int representing the number of pages in the book.
     * @throws NullPointerException if the book title, authors, or ISBN is null
     * @throws IllegalArgumentException if the book rating or the number of pages of the book violate the current book constraints.
     */
    public BookEntry(String title, String[] authors, float rating, String ISBN, int pages) {
        BookConstraint.bookValidityCheck(title, authors, rating, ISBN, pages);
        this.title = title;
        this.authors = authors;
        this.rating = rating;
        this.ISBN = ISBN;
        this.pages = pages;
    }

    /**
     * @return number of pages of this book entry
     */
    public int getPages() {
        return pages;
    }

    /**
     * @return the ISBN of this book entry
     */
    public String getISBN() {
        return ISBN;
    }

    /**
     * @return the rating of this book entry
     */
    public float getRating() {
        return rating;
    }

    /**
     * @return the authors of this book entry
     */
    public String[] getAuthors() {
        return authors;
    }

    /**
     * @return the title of this book entry
     */
    public String getTitle() {
        return title;
    }

    /**
     * This overrides the Objects class toString method.
     * It gives a string representing the book entry by its instance fields in the following format,
     *
     * <title>
     * by <author 1>, <author 2>....
     * Rating: <rating>
     * ISBN: <ISBN>
     * <pages> pages
     *
     * @return a string reflecting the properties of this book
     */
    @Override
    public String toString() {
        StringBuilder authors = new StringBuilder("\nby " + getAuthors()[0]);
        for (int i = 1; i < this.authors.length; i++) {
            authors.append(", ").append(this.authors[i]);
        }
        return title + authors +
                String.format("\nRating: %.2f\nISBN: %s\n%d pages", rating, ISBN, pages);
    }

    /**
     * This overrides the Objects class equals method.
     * It checks for equality by comparing all of the instance fields of this book entry and the entry to compare to.
     *
     * @param that the book entry to compare to
     *
     * @return boolean indicating if the two book entries are identical
     */
    @Override
    public boolean equals(Object that) {
        if (this == that) {
            return true;
        }
        if (!(that instanceof BookEntry)) {
            return false;
        }

        BookEntry book = (BookEntry) that;
        return Float.compare(book.rating, rating) == 0 &&
                pages == book.pages &&
                Objects.equals(title, book.title) &&
                Arrays.equals(authors, book.authors) &&
                Objects.equals(ISBN, book.ISBN);
    }

    /**
     * This overrides the Object class hashCode method.
     * It creates a hashcode based on all instance fields.
     *
     * @return unique hashCode for the book entry
     */
    @Override
    public int hashCode() {
        int result = Objects.hash(title, rating, ISBN, pages);
        result = 31 * result + Arrays.hashCode(authors);
        return result;
    }

    /**
     * This overrides the compareTo method in the comparable interface.
     *
     * This compareTo method compares the titles of two books lexicographically but
     * with some adjustments.
     *
     * 1. The comparison is not case sensitive.
     * 2. Any book title that start with a digit is considered larger than one that does not.
     *
     * In the current version of the program this method is not used, but it was included as it
     * is a useful method for future versions of the program.
     *
     * @param that the book entry to compare with
     * @return an int indicating which of the book titles is the largest
     */
    @Override
    public int compareTo(BookEntry that) {
        Objects.requireNonNull(that, "ERROR: The book entry to compare to cannot be null.");

        String title = this.title.toUpperCase();
        String thatTitle = that.getTitle().toUpperCase();

        if (Character.isDigit(title.charAt(0)) && !Character.isDigit(thatTitle.charAt(0))) {
            return 1;
        }
        if (!Character.isDigit(title.charAt(0)) && Character.isDigit(thatTitle.charAt(0))) {
            return -1;
        }
        return title.compareTo(thatTitle);
    }

}
