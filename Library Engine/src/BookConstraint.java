import java.util.Objects;

/**
 * Utility class responsible for checking the validity of
 * book entry parameters
 *
 * In this current version, the book entry constraints are as follows,
 *
 * 1. The book title, book authors, and book ISBN must not be null.
 * 2. The book rating must be between 0 and 5.
 * 3. The number of book pages must not be negative - I.e, must be larger than 0.
 */
public final class BookConstraint {

    /** The maximum rating a book entry is allowed to have */
    private static final int MAX_RATING = 5;
    /** The minimum rating a book entry is allowed to have */
    private static final int MIN_RATING = 0;
    /** The minimum pages a book entry is allowed to have */
    private static final int MIN_PAGES = 0;

    /** Not to be used*/
    private BookConstraint() {
        //Nothing to do
        throw new UnsupportedOperationException("This constructor should never be used.");
    }

    /**
     * Checks the validity of the book entry parameters
     *
     * @param title the title of the book entry
     * @param authors the authors of the book entry
     * @param rating the rating of the book entry
     * @param ISBN the ISBN of the book entry
     * @param pages the pages of the book entry
     * @throws NullPointerException if the book title, authors, or ISBN is null
     * @throws IllegalArgumentException if the rating or the number of pages of the book violate the current book constraints
     */
    public static void bookValidityCheck(String title, String[] authors, float rating, String ISBN, int pages) {
        titleValidityCheck(title);
        authorsValidityCheck(authors);
        ratingValidityCheck(rating);
        ISBNValidityCheck(ISBN);
        pageValidityCheck(pages);
    }

    /**
     * Checks the validity of the book title
     *
     * @param title book title
     */
    private static void titleValidityCheck(String title) {
        Objects.requireNonNull(title, "ERROR: The title of the book cannot be null!");
    }

    /**
     * Checks the validity of the book authors
     *
     * @param authors an array containing the book authors
     */
    private static void authorsValidityCheck(String[] authors) {
        Objects.requireNonNull(authors, "ERROR: The authors of the book cannot be null!");

        for (String author : authors) {
            Objects.requireNonNull(author, "ERROR: The authors of the book cannot be null!");
        }
    }

    /**
     * Checks the validity of the book rating
     *
     * @param rating the rating of the book
     */
    private static void ratingValidityCheck(float rating) {
        if (!(rating >= MIN_RATING && rating <= MAX_RATING)) {
            throw new IllegalArgumentException("ERROR: The specified rating must be between 0 and 5!");
        }
    }

    /**
     * Checks the validity of the book ISBN
     *
     * @param ISBN the ISBN of the book
     */
    private static void ISBNValidityCheck(String ISBN) {
        Objects.requireNonNull(ISBN, "ERROR: The ISBN number of the book cannot be null!");
    }

    /**
     * Checks the validity of the number of book pages
     *
     * @param pages number of book pages
     */
    private static void pageValidityCheck(int pages) {
        if (pages < MIN_PAGES) {
            throw new IllegalArgumentException("ERROR: The number of pages cannot be negative!");
        }
    }
}
