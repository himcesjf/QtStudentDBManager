#include <QSqlTableModel>

class StudentModel : public QSqlTableModel {
    Q_OBJECT

    public:
        explicit StudentModel(QObject *parent = nullptr);
        ~StudentModel();

        QHash<int, QByteArray> roleNames() const override;

    private:
        void setRolesFromHeader();
        void createTable();
        void seedInitialStudents();
};