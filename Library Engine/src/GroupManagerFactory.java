import java.util.Objects;
/**
 * Static factory for creating group managers responsible for different grouping operations.
 */
public class GroupManagerFactory {

    /** Not to be used. */
    private GroupManagerFactory() {
        //Nothing to do
        throw new UnsupportedOperationException("This constructor should never be used.");
    }

    /**
     * Creates a group manager for the given library data and type.
     *
     * @param data the book data that the group manager will operate on.
     * @param type the type of the group manager to be created.
     * @return group manager of the given type initialised with the given data.
     * @throws NullPointerException If any of the given parameters are null.
     * @throws IllegalArgumentException If an illegal group type is provided.
     */
    public static GroupManager createManager(LibraryData data, GroupType type) {
        Objects.requireNonNull(type, "ERROR: Given command type must not be null.");

        switch(type) {
            case AUTHOR: return new AuthorGroupManager(data);
            case TITLE: return new TitleGroupManager(data);
            default:
                throw new IllegalArgumentException("Command type not supported: " + type);
        }

    }


}
