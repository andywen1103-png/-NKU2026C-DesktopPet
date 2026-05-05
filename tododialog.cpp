#include "tododialog.h"
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

TodoDialog::TodoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add New Todo");
    setModal(true);
    setFixedSize(300, 150);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("Todo Name:"));
    m_nameEdit = new QLineEdit(this);
    mainLayout->addWidget(m_nameEdit);
    mainLayout->addWidget(new QLabel("Due Time:"));
    m_dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    m_dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_dateTimeEdit->setCalendarPopup(true);
    mainLayout->addWidget(m_dateTimeEdit);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);
    btnLayout->addWidget(m_okButton);
    btnLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(btnLayout);
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString TodoDialog::getTodoName() const { return m_nameEdit->text(); }
QDateTime TodoDialog::getDueDateTime() const { return m_dateTimeEdit->dateTime(); }
