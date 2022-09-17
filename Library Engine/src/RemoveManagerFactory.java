import java.util.Objects;
/**
 * Static factory for creating remove managers responsible for different removal operations.
 */
public final class RemoveManagerFactory {

    /** Not to be used. */
    private RemoveManagerFactory() {
        //Nothing to do
        throw new UnsupportedOperationException("This constructor should never be used.");
    }

    /**
     * Creates a remove manager for the given library data, type and argument.
     *
     * @param data the book data that the remove manager will operate on.
     * @param type the type of the remove manager to be created.
     * @param argumentInput remove manager argument to be used during the initialisation of the manager.
     * @return remove manager of the given type initialised for the given argument and data.
     * @throws NullPointerException If any of the given parameters are null.
     * @throws IllegalArgumentException If an illegal remove type is provided.
     */
    public static RemoveManager createManager(LibraryData data, RemoveType type, String argumentInput) {
        Objects.requireNonNull(type, "ERROR: Given command type must not be null.");
        Objects.requireNonNull(argumentInput, "Given argument input must not be null.");

        switch(type) {
            case AUTHOR: return new AuthorRemoveManager(data, argumentInput);
            case TITLE: return new TitleRemoveManager(data, argumentInput);
            default:
                throw new IllegalArgumentException("Command type not supported: " + type);
        }
    }

}
