#include <QObject>

class Student;

class ClientDatabase : public QObject
{
    Q_OBJECT

public:
    static ClientDatabase& instance() {
        static ClientDatabase instance;
        return instance;
    }

    ClientDatabase(const ClientDatabase&) = delete;
    ClientDatabase& operator=(const ClientDatabase&) = delete;

    static bool createTables();
    static QVector<Student *> students(const QString &school);
    static bool add(Student *student);
    static bool update(Student *student);

private:
    explicit ClientDatabase(QObject *parent = nullptr);

    static void logToSync(const Student *student);
};
