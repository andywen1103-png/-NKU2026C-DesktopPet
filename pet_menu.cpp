#include "pet_menu.h"
#include <QComboBox>
#include <QCheckBox>
#include <QSlider>
#include <QToolButton>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QSettings>    // 新增

PetMenu::PetMenu(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(240);
    setStyleSheet(R"(
        PetMenu {
            background: rgba(255, 248, 245, 220);
            border: 2px solid #FFB6C1;
            border-radius: 15px;
            padding: 8px;
        }
        QComboBox, QCheckBox, QToolButton, QPushButton {
            font: 10pt 'Comic Sans MS';
            background: rgba(255, 255, 255, 180);
            border: 1px solid #FFB6C1;
            border-radius: 8px;
            padding: 4px;
        }
        QComboBox:hover, QCheckBox:hover, QToolButton:hover, QPushButton:hover {
            background: #FFE4E1;
            border-color: #FF69B4;
        }
        QSlider::groove:horizontal {
            height: 6px;
            background: #FFB6C1;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #FF69B4;
            width: 12px;
            margin: -3px 0;
            border-radius: 6px;
        }
        QSlider::handle:horizontal:hover {
            background: #FF1493;
        }
    )");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setOffset(4, 4);
    shadow->setColor(QColor(0, 0, 0, 80));
    setGraphicsEffect(shadow);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // 标题
    QLabel *title = new QLabel("🎮 Character Menu", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font: bold 14pt 'Comic Sans MS'; color: #FF69B4; background: none; border: none;");
    mainLayout->addWidget(title);

    // 角色选择
    QHBoxLayout *roleLayout = new QHBoxLayout();
    QLabel *roleLabel = new QLabel("🐾 Switch:", this);
    roleLabel->setStyleSheet("font: 10pt 'Comic Sans MS'; background: none; border: none;");
    m_roleCombo = new QComboBox(this);
    m_roleCombo->addItem("Cat");
    m_roleCombo->addItem("Dog");
    m_roleCombo->addItem("Rabbit");
    roleLayout->addWidget(roleLabel);
    roleLayout->addWidget(m_roleCombo);
    mainLayout->addLayout(roleLayout);

    // 音量调节
    QHBoxLayout *volumeLayout = new QHBoxLayout();
    QLabel *volumeLabel = new QLabel("🎵 Volume:", this);
    volumeLabel->setStyleSheet("font: 10pt 'Comic Sans MS'; background: none; border: none;");
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(50);
    m_volumeSlider->setFixedWidth(100);
    m_volumeValueLabel = new QLabel("50%", this);
    m_volumeValueLabel->setStyleSheet("font: 9pt 'Comic Sans MS'; background: none; border: none;");
    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(m_volumeSlider);
    volumeLayout->addWidget(m_volumeValueLabel);
    mainLayout->addLayout(volumeLayout);

    // 城市选择行（增加 Go 按钮）
    QHBoxLayout *cityLayout = new QHBoxLayout();
    QLabel *cityLabel = new QLabel("🏙️ City:", this);
    cityLabel->setStyleSheet("font: 10pt 'Comic Sans MS'; background: none; border: none;");
    m_cityCombo = new QComboBox(this);
    m_cityCombo->setEditable(true);          // 允许手动输入
    m_cityCombo->setInsertPolicy(QComboBox::NoInsert); // 不自动插入
    // 加载历史记录（最近5个）
    QSettings settings("MyCompany", "DesktopPet");
    QStringList history = settings.value("cityHistory").toStringList();
    for (const QString &city : history) {
        m_cityCombo->addItem(city);
    }
    if (m_cityCombo->count() == 0) {
        m_cityCombo->addItem("Beijing");     // 默认城市
    }
    m_cityCombo->setCurrentText(history.isEmpty() ? "Beijing" : history.first());

    m_goBtn = new QPushButton("Go", this);
    m_goBtn->setFixedWidth(40);
    cityLayout->addWidget(cityLabel);
    cityLayout->addWidget(m_cityCombo);
    cityLayout->addWidget(m_goBtn);
    mainLayout->addLayout(cityLayout);

    // 分隔线
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background: #FFB6C1;");
    mainLayout->addWidget(line);

    // 功能网格（省略，与之前相同）
    QGridLayout *funcLayout = new QGridLayout();
    funcLayout->setContentsMargins(0, 5, 0, 5);
    funcLayout->setSpacing(10);

    m_timeCheck = new QCheckBox(" Show Time", this);
    m_weatherCheck = new QCheckBox(" Show Weather", this);
    m_todosCheck = new QCheckBox(" Show Todos", this);
    m_addTodoBtn = new QToolButton(this);
    m_addTodoBtn->setText("Add Todo");
    m_clearTodosBtn = new QToolButton(this);
    m_clearTodosBtn->setText("Clear All");

    QString checkStyle = "QCheckBox { background: rgba(255,255,255,180); border: 1px solid #FFB6C1; border-radius: 8px; padding: 4px; }";
    m_timeCheck->setStyleSheet(checkStyle);
    m_weatherCheck->setStyleSheet(checkStyle);
    m_todosCheck->setStyleSheet(checkStyle);

    funcLayout->addWidget(m_timeCheck, 0, 0);
    funcLayout->addWidget(m_weatherCheck, 0, 1);
    funcLayout->addWidget(m_todosCheck, 1, 0);
    funcLayout->addWidget(m_clearTodosBtn, 1, 1);
    funcLayout->addWidget(m_addTodoBtn, 2, 0, 1, 2, Qt::AlignCenter);

    mainLayout->addLayout(funcLayout);
    mainLayout->addStretch();

    // 信号连接
    connect(m_roleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PetMenu::onRoleChanged);
    connect(m_timeCheck, &QCheckBox::toggled, this, &PetMenu::onTimeToggled);
    connect(m_weatherCheck, &QCheckBox::toggled, this, &PetMenu::onWeatherToggled);
    connect(m_todosCheck, &QCheckBox::toggled, this, &PetMenu::onTodosToggled);
    connect(m_addTodoBtn, &QToolButton::clicked, this, &PetMenu::onAddTodoClicked);
    connect(m_clearTodosBtn, &QToolButton::clicked, this, &PetMenu::onClearAllClicked);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &PetMenu::onVolumeChanged);
    connect(m_goBtn, &QPushButton::clicked, this, &PetMenu::onGoClicked);
}

void PetMenu::onRoleChanged(int index) { emit roleSelected(index); }
void PetMenu::onTimeToggled(bool checked) { emit timeDisplayToggled(checked); }
void PetMenu::onWeatherToggled(bool checked) { emit weatherDisplayToggled(checked); }
void PetMenu::onTodosToggled(bool checked) { emit todosDisplayToggled(checked); }
void PetMenu::onAddTodoClicked() { emit addTodoClicked(); }
void PetMenu::onClearAllClicked() { emit clearAllTodosClicked(); }
void PetMenu::onVolumeChanged(int value)
{
    m_volumeValueLabel->setText(QString::number(value) + "%");
    emit volumeChanged(value);
}

void PetMenu::onGoClicked()
{
    QString city = m_cityCombo->currentText().trimmed();
    if (city.isEmpty()) return;

    // 更新历史记录
    QSettings settings("MyCompany", "DesktopPet");
    QStringList history = settings.value("cityHistory").toStringList();
    history.removeAll(city);
    history.prepend(city);
    while (history.size() > 5) history.removeLast();
    settings.setValue("cityHistory", history);

    // 刷新下拉框的选项
    m_cityCombo->clear();
    for (const QString &c : history) {
        m_cityCombo->addItem(c);
    }
    m_cityCombo->setCurrentText(city);

    emit citySelected(city);
}
