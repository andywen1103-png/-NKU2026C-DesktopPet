#include "pet_menu.h"
#include <QComboBox>
#include <QCheckBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>

PetMenu::PetMenu(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(200);
    setStyleSheet("background: rgba(255, 255, 255, 200); border: 2px solid #FFB6C1; border-radius: 10px; padding: 8px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);

    QLabel *title = new QLabel("Character Menu", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font: bold 14pt 'Comic Sans MS'; color: #FF69B4;");
    mainLayout->addWidget(title);

    // 角色选择行
    QHBoxLayout *roleLayout = new QHBoxLayout();
    QLabel *roleLabel = new QLabel("Switch:", this);
    roleLabel->setStyleSheet("font: 10pt 'Comic Sans MS';");
    m_roleCombo = new QComboBox(this);
    m_roleCombo->addItem("miu");
    m_roleCombo->addItem("vva");
    m_roleCombo->addItem("prts");
    m_roleCombo->setStyleSheet("font: 10pt;");
    roleLayout->addWidget(roleLabel);
    roleLayout->addWidget(m_roleCombo);
    mainLayout->addLayout(roleLayout);

    // 音量调节行
    QHBoxLayout *volumeLayout = new QHBoxLayout();
    QLabel *volumeLabel = new QLabel("Volume:", this);
    volumeLabel->setStyleSheet("font: 10pt 'Comic Sans MS';");
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(50);
    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(m_volumeSlider);
    mainLayout->addLayout(volumeLayout);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background: #FFB6C1;");
    mainLayout->addWidget(line);

    // 功能网格
    QGridLayout *functionLayout = new QGridLayout();
    functionLayout->setContentsMargins(0, 5, 0, 5);
    functionLayout->setSpacing(8);

    m_timeCheck = new QCheckBox("Show Time", this);
    m_weatherCheck = new QCheckBox("Show Weather", this);
    m_todosCheck = new QCheckBox("Show Todos", this);
    m_addTodoBtn = new QPushButton("Add Todo", this);
    m_clearTodosBtn = new QPushButton("Clear All", this);

    QString checkStyle = "font: 6pt 'Comic Sans MS'; color: #8A2BE2;";
    m_timeCheck->setStyleSheet(checkStyle);
    m_weatherCheck->setStyleSheet(checkStyle);
    m_todosCheck->setStyleSheet(checkStyle);

    m_addTodoBtn->setStyleSheet("font: 10pt 'Comic Sans MS'; background: #FFE4E1; border-radius: 5px; padding: 3px;");
    m_clearTodosBtn->setStyleSheet("font: 10pt 'Comic Sans MS'; background: #FFE4E1; border-radius: 5px; padding: 3px;");

    functionLayout->addWidget(m_timeCheck, 0, 0);
    functionLayout->addWidget(m_weatherCheck, 0, 1);
    functionLayout->addWidget(m_todosCheck, 1, 0);
    functionLayout->addWidget(m_clearTodosBtn, 1, 1);
    functionLayout->addWidget(m_addTodoBtn, 2, 0, 1, 2, Qt::AlignCenter);

    mainLayout->addLayout(functionLayout);
    mainLayout->addStretch();

    connect(m_roleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PetMenu::onRoleChanged);
    connect(m_timeCheck, &QCheckBox::toggled, this, &PetMenu::onTimeToggled);
    connect(m_weatherCheck, &QCheckBox::toggled, this, &PetMenu::onWeatherToggled);
    connect(m_todosCheck, &QCheckBox::toggled, this, &PetMenu::onTodosToggled);
    connect(m_addTodoBtn, &QPushButton::clicked, this, &PetMenu::onAddTodoClicked);
    connect(m_clearTodosBtn, &QPushButton::clicked, this, &PetMenu::onClearAllClicked);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &PetMenu::onVolumeChanged);
}

void PetMenu::onRoleChanged(int index) { emit roleSelected(index); }
void PetMenu::onTimeToggled(bool checked) { emit timeDisplayToggled(checked); }
void PetMenu::onWeatherToggled(bool checked) { emit weatherDisplayToggled(checked); }
void PetMenu::onTodosToggled(bool checked) { emit todosDisplayToggled(checked); }
void PetMenu::onAddTodoClicked() { emit addTodoClicked(); }
void PetMenu::onClearAllClicked() { emit clearAllTodosClicked(); }
void PetMenu::onVolumeChanged(int value) { emit volumeChanged(value); }
