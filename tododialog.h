#ifndef TODODIALOG_H
#define TODODIALOG_H

#include <QDialog>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QDateTimeEdit;
class QPushButton;
QT_END_NAMESPACE

class TodoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TodoDialog(QWidget *parent = nullptr);
    QString getTodoName() const;
    QDateTime getDueDateTime() const;

private:
    QLineEdit *m_nameEdit;
    QDateTimeEdit *m_dateTimeEdit;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif // TODODIALOG_H
