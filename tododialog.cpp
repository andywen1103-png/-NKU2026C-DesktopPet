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

    QLabel *nameLabel = new QLabel("Todo Name:", this);
    m_nameEdit = new QLineEdit(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(m_nameEdit);

    QLabel *timeLabel = new QLabel("Due Time:", this);
    m_dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    m_dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_dateTimeEdit->setCalendarPopup(true);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(m_dateTimeEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString TodoDialog::getTodoName() const
{
    return m_nameEdit->text();
}

QDateTime TodoDialog::getDueDateTime() const
{
    return m_dateTimeEdit->dateTime();
}
