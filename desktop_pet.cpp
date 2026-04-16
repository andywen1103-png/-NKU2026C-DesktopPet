#include "desktop_pet.h"
#include "pet_menu.h"
#include "tododialog.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>

DesktopPet::DesktopPet(QWidget *parent)
    : QWidget(parent)
    , m_mousePressed(false)
    , m_longPressTimer(new QTimer(this))
    , m_currentRoleIndex(0)
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_musicEnabled(true)
    , m_timeTimer(new QTimer(this))
    , m_weatherTimer(new QTimer(this))
    , m_todoTimer(new QTimer(this))
    , m_resetTimer(new QTimer(this))
    , m_todosVisible(true)
    , m_currentCity("Beijing")
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    m_longPressTimer->setInterval(LONG_PRESS_INTERVAL);
    m_longPressTimer->setSingleShot(true);
    connect(m_longPressTimer, &QTimer::timeout, this, &DesktopPet::onLongPress);

    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.5);
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, &DesktopPet::onMediaStatusChanged);

    m_resetTimer->setSingleShot(true);
    m_resetTimer->setInterval(2000);
    connect(m_resetTimer, &QTimer::timeout, this, [this]() {
        setPetImage("normal");
    });

    connect(m_todoTimer, &QTimer::timeout, this, &DesktopPet::checkTodos);
    m_todoTimer->start(60000);

    m_weatherNetManager = new QNetworkAccessManager(this);
    connect(m_weatherNetManager, &QNetworkAccessManager::finished,
            this, &DesktopPet::onWeatherReplyFinished);

    // ⚠️ 请替换为你的 OpenWeatherMap API Key
    m_apiKey = "7b2b160e0d0939c698c2d43650fbeb79";

    loadRoles();
    setupUI();
    setCurrentRole(0);

    connect(m_timeTimer, &QTimer::timeout, this, &DesktopPet::updateDateTime);
    m_timeTimer->start(1000);
    updateDateTime();

    connect(m_weatherTimer, &QTimer::timeout, this, &DesktopPet::updateWeather);
    m_weatherTimer->setInterval(30 * 60 * 1000);

    m_timeLabel->hide();
    m_weatherLabel->hide();
    m_todoTitleLabel->hide();

    requestWeatherForCity(m_currentCity);
}

DesktopPet::~DesktopPet() {}

void DesktopPet::setupUI()
{
    m_mainFrame = new QFrame(this);
    m_mainFrame->setObjectName("mainFrame");
    m_mainFrame->setStyleSheet("#mainFrame { background: rgba(255, 240, 245, 200); border: 3px solid #FFB6C1; border-radius: 15px; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(m_mainFrame);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 左侧面板
    m_leftPanel = new QWidget(m_mainFrame);
    m_leftPanel->setFixedWidth(180);
    m_leftPanel->setStyleSheet("background: rgba(255, 255, 255, 150); border-radius: 10px; padding: 5px;");
    QVBoxLayout *leftLayout = new QVBoxLayout(m_leftPanel);
    leftLayout->setSpacing(8);

    m_timeLabel = new QLabel(m_leftPanel);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setStyleSheet("font: bold 12pt 'Comic Sans MS'; color: #333; background: rgba(255,255,255,100); border-radius: 5px; padding: 5px;");
    m_timeLabel->setWordWrap(true);
    leftLayout->addWidget(m_timeLabel);

    m_weatherLabel = new QLabel(m_leftPanel);
    m_weatherLabel->setAlignment(Qt::AlignCenter);
    m_weatherLabel->setStyleSheet("font: 10pt 'Comic Sans MS'; color: #2E8B57; background: rgba(255,255,255,100); border-radius: 5px; padding: 5px;");
    m_weatherLabel->setWordWrap(true);
    leftLayout->addWidget(m_weatherLabel);

    m_todoTitleLabel = new QLabel("📋 Todos:", m_leftPanel);
    m_todoTitleLabel->setStyleSheet("font: bold 11pt 'Comic Sans MS'; color: #8A2BE2; margin-top: 5px;");
    leftLayout->addWidget(m_todoTitleLabel);

    QWidget *todoContainer = new QWidget(m_leftPanel);
    m_todoLayout = new QVBoxLayout(todoContainer);
    m_todoLayout->setContentsMargins(0, 0, 0, 0);
    m_todoLayout->setSpacing(2);
    leftLayout->addWidget(todoContainer);
    leftLayout->addStretch();

    mainLayout->addWidget(m_leftPanel);

    // 宠物区域
    QWidget *petArea = new QWidget(m_mainFrame);
    petArea->setFixedSize(200, 200);
    petArea->setStyleSheet("background: transparent;");

    m_petLabel = new QLabel(petArea);
    m_petLabel->setGeometry(0, 0, 200, 200);
    m_petLabel->setScaledContents(true);
    m_petLabel->setStyleSheet("background: transparent;");

    m_bubbleLabel = new QLabel(petArea);
    m_bubbleLabel->setGeometry(130, 10, 60, 40);
    m_bubbleLabel->setStyleSheet("background: rgba(255,255,255,200); border: 2px solid pink; border-radius: 10px; padding: 5px; font: 8pt;");
    m_bubbleLabel->setAlignment(Qt::AlignCenter);
    m_bubbleLabel->hide();

    mainLayout->addWidget(petArea);

    // 右侧菜单
    m_menu = new PetMenu(m_mainFrame);
    connect(m_menu, &PetMenu::roleSelected, this, &DesktopPet::switchRole);
    connect(m_menu, &PetMenu::timeDisplayToggled, this, &DesktopPet::toggleTimeDisplay);
    connect(m_menu, &PetMenu::weatherDisplayToggled, this, &DesktopPet::toggleWeatherDisplay);
    connect(m_menu, &PetMenu::todosDisplayToggled, this, &DesktopPet::toggleTodosDisplay);
    connect(m_menu, &PetMenu::addTodoClicked, this, &DesktopPet::onAddTodo);
    connect(m_menu, &PetMenu::clearAllTodosClicked, this, &DesktopPet::onClearAllTodos);
    connect(m_menu, &PetMenu::volumeChanged, this, &DesktopPet::setVolume);
    connect(m_menu, &PetMenu::citySelected, this, &DesktopPet::onCitySelected);
    mainLayout->addWidget(m_menu);

    // 右上角按钮
    QVBoxLayout *btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(5);
    btnLayout->setContentsMargins(0, 5, 5, 0);

    m_toggleMenuBtn = new QPushButton("☰", m_mainFrame);
    m_toggleMenuBtn->setFixedSize(25, 25);
    m_toggleMenuBtn->setStyleSheet("background: #FFB6C1; border: none; border-radius: 5px; font: bold 12pt; color: white;");
    connect(m_toggleMenuBtn, &QPushButton::clicked, this, &DesktopPet::toggleMenu);

    m_minimizeBtn = new QPushButton("—", m_mainFrame);
    m_closeBtn = new QPushButton("✕", m_mainFrame);
    m_minimizeBtn->setFixedSize(25, 25);
    m_closeBtn->setFixedSize(25, 25);
    m_minimizeBtn->setStyleSheet("background: #FFB6C1; border: none; border-radius: 5px; font: bold 12pt; color: white;");
    m_closeBtn->setStyleSheet("background: #FF69B4; border: none; border-radius: 5px; font: bold 12pt; color: white;");

    btnLayout->addWidget(m_toggleMenuBtn);
    btnLayout->addWidget(m_minimizeBtn);
    btnLayout->addWidget(m_closeBtn);
    btnLayout->addStretch();

    QWidget *btnWidget = new QWidget(m_mainFrame);
    btnWidget->setLayout(btnLayout);
    btnWidget->setFixedWidth(30);
    mainLayout->addWidget(btnWidget);
    mainLayout->setAlignment(btnWidget, Qt::AlignTop);

    connect(m_minimizeBtn, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(m_closeBtn, &QPushButton::clicked, qApp, &QApplication::quit);

    setFixedSize(m_mainFrame->sizeHint());
    m_mainFrame->setGeometry(0, 0, width(), height());

    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    move(screenGeometry.width() - width() - 20, screenGeometry.height() - height() - 40);
}

void DesktopPet::loadRoles()
{
    // 角色0
    RoleData role0;
    role0.normalImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role1\\1.png";
    role0.clickImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role1\\2.png";
    role0.doubleClickImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role1\\2.png";
    role0.longPressImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role1\\3.png";
    role0.musicPath = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Music\\Role1\\Morning_Dew.ogg";
    m_roles.append(role0);

    // 角色1
    RoleData role1;
    role1.normalImage =  "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role2\\1.png";
    role1.clickImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role2\\2.png";
    role1.doubleClickImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role2\\2.png";
    role1.longPressImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role2\\3.png";
    role1.musicPath = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Music\\Role2\\bgm_city.ogg";
    m_roles.append(role1);

    // 角色2
    RoleData role2;
    role2.normalImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role3\\1.png";
    role2.clickImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role3\\2.png";
    role2.doubleClickImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role3\\2.png";
    role2.longPressImage = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Pictures\\Role3\\3.png";
    role2.musicPath = "C:\\Users\\Andy Wen\\Desktop\\CBigwork\\Music\\Role3\\bgm_sea.ogg";
    m_roles.append(role2);
}

void DesktopPet::setCurrentRole(int index)
{
    if (index < 0 || index >= m_roles.size()) return;
    m_currentRoleIndex = index;
    setPetImage("normal");
    if (m_musicEnabled) {
        m_mediaPlayer->stop();
        m_mediaPlayer->setSource(QUrl::fromLocalFile(m_roles[index].musicPath));
        m_mediaPlayer->play();
    }
}

void DesktopPet::setPetImage(const QString &state)
{
    if (m_currentRoleIndex < 0 || m_currentRoleIndex >= m_roles.size()) return;
    QString imagePath;
    const RoleData &role = m_roles[m_currentRoleIndex];
    if (state == "click") imagePath = role.clickImage;
    else if (state == "double") imagePath = role.doubleClickImage;
    else if (state == "long") imagePath = role.longPressImage;
    else imagePath = role.normalImage;

    QPixmap pix(imagePath);
    if (!pix.isNull()) {
        m_petLabel->setPixmap(pix);
    } else {
        m_petLabel->setText("Image\nMissing");
        qWarning() << "Failed to load image:" << imagePath;
    }
}

void DesktopPet::showBubbleWithPattern(const QString &pattern)
{
    m_bubbleLabel->setText(pattern);
    m_bubbleLabel->show();
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(m_bubbleLabel);
    m_bubbleLabel->setGraphicsEffect(effect);
    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(1500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(2000, this, [this]() { m_bubbleLabel->hide(); });
}

// 鼠标事件
void DesktopPet::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        m_longPressTimer->start();
        event->accept();
    }
}

void DesktopPet::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_longPressTimer->isActive()) {
            m_longPressTimer->stop();
            setPetImage("click");
            showBubbleWithPattern("😊");
            m_resetTimer->start();
        }
        m_mousePressed = false;
        event->accept();
    }
}

void DesktopPet::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_longPressTimer->isActive()) m_longPressTimer->stop();
        setPetImage("double");
        showBubbleWithPattern("🎉");
        m_resetTimer->start();
        event->accept();
    }
}

void DesktopPet::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_mousePressed) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void DesktopPet::onLongPress()
{
    setPetImage("long");
    showBubbleWithPattern("❤️");
    m_resetTimer->start();
}

void DesktopPet::updateDateTime()
{
    QDateTime now = QDateTime::currentDateTime();
    m_timeLabel->setText(now.toString("yyyy-MM-dd\nhh:mm:ss"));
}

// 天气相关
void DesktopPet::requestWeatherForCity(const QString &cityName)
{
    if (m_apiKey == "YOUR_OPENWEATHERMAP_API_KEY" || m_apiKey.isEmpty()) {
        m_weatherLabel->setText("❌ 请设置API Key");
        return;
    }
    QUrl url("https://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("q", cityName);
    query.addQueryItem("appid", m_apiKey);
    query.addQueryItem("units", "metric");
    query.addQueryItem("lang", "zh_cn");
    url.setQuery(query);
    QNetworkRequest request(url);
    m_weatherNetManager->get(request);
}

void DesktopPet::updateWeather()
{
    if (!m_currentCity.isEmpty())
        requestWeatherForCity(m_currentCity);
}

void DesktopPet::onWeatherReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Weather API error:" << reply->errorString();
        m_weatherLabel->setText("❌ 网络错误");
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        m_weatherLabel->setText("❌ 无效数据");
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();
    if (obj.contains("cod") && obj.value("cod").toInt() != 200) {
        m_weatherLabel->setText("❌ 城市未找到");
        reply->deleteLater();
        return;
    }

    QJsonObject mainObj = obj.value("main").toObject();
    double temp = mainObj.value("temp").toDouble();
    int humidity = mainObj.value("humidity").toInt();
    QJsonArray weatherArray = obj.value("weather").toArray();
    QString description = weatherArray[0].toObject().value("description").toString();
    QString iconCode = weatherArray[0].toObject().value("icon").toString();

    QString emoji = "🌡️";
    if (iconCode.contains("01")) emoji = "☀️";
    else if (iconCode.contains("02")) emoji = "⛅";
    else if (iconCode.contains("03") || iconCode.contains("04")) emoji = "☁️";
    else if (iconCode.contains("09")) emoji = "🌧️";
    else if (iconCode.contains("10")) emoji = "🌦️";
    else if (iconCode.contains("11")) emoji = "⛈️";
    else if (iconCode.contains("13")) emoji = "❄️";
    else if (iconCode.contains("50")) emoji = "🌫️";

    m_weatherLabel->setText(QString("%1 %2°C %3 湿度%4%")
                                .arg(emoji).arg(temp, 0, 'f', 1).arg(description).arg(humidity));
    reply->deleteLater();
}

void DesktopPet::onCitySelected(const QString &cityName)
{
    m_currentCity = cityName;
    requestWeatherForCity(cityName);
    if (m_weatherLabel->isVisible()) {
        m_weatherTimer->start();
    }
}

// 菜单信号
void DesktopPet::switchRole(int index) { setCurrentRole(index); }
void DesktopPet::toggleTimeDisplay(bool checked) { m_timeLabel->setVisible(checked); }
void DesktopPet::toggleWeatherDisplay(bool checked)
{
    m_weatherLabel->setVisible(checked);
    if (checked) {
        if (!m_currentCity.isEmpty()) requestWeatherForCity(m_currentCity);
        m_weatherTimer->start();
    } else {
        m_weatherTimer->stop();
    }
}
void DesktopPet::toggleTodosDisplay(bool checked)
{
    m_todosVisible = checked;
    m_todoTitleLabel->setVisible(checked && !m_todos.isEmpty());
    for (const TodoItem &item : m_todos)
        if (item.displayLabel) item.displayLabel->setVisible(checked);
}
void DesktopPet::setVolume(int volume) { m_audioOutput->setVolume(volume / 100.0); }
void DesktopPet::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) m_mediaPlayer->play();
}

// 待办
void DesktopPet::onAddTodo()
{
    TodoDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        QString name = dlg.getTodoName();
        QDateTime due = dlg.getDueDateTime();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Invalid", "Todo name cannot be empty.");
            return;
        }
        if (due <= QDateTime::currentDateTime()) {
            QMessageBox::warning(this, "Invalid", "Due time must be in the future.");
            return;
        }
        QLabel *label = new QLabel(m_leftPanel);
        label->setStyleSheet("font: 9pt 'Comic Sans MS'; color: #333; padding: 2px;");
        label->setWordWrap(true);
        label->setVisible(m_todosVisible);
        m_todoLayout->addWidget(label);
        TodoItem item;
        item.name = name;
        item.dueTime = due;
        item.displayLabel = label;
        m_todos.append(item);
        refreshTodoList();
    }
}

void DesktopPet::onClearAllTodos()
{
    for (const TodoItem &item : m_todos) {
        if (item.displayLabel) {
            m_todoLayout->removeWidget(item.displayLabel);
            delete item.displayLabel;
        }
    }
    m_todos.clear();
    m_todoTitleLabel->setVisible(false);
}

void DesktopPet::checkTodos()
{
    QDateTime now = QDateTime::currentDateTime();
    bool needRefresh = false;
    for (int i = m_todos.size() - 1; i >= 0; --i) {
        if (m_todos[i].dueTime <= now) {
            m_todoLayout->removeWidget(m_todos[i].displayLabel);
            delete m_todos[i].displayLabel;
            m_todos.removeAt(i);
            needRefresh = true;
        }
    }
    for (TodoItem &item : m_todos) {
        qint64 secsTo = now.secsTo(item.dueTime);
        if (secsTo <= 86400 && secsTo > 0)
            item.displayLabel->setStyleSheet("font: 9pt 'Comic Sans MS'; color: red; padding: 2px;");
        else
            item.displayLabel->setStyleSheet("font: 9pt 'Comic Sans MS'; color: #333; padding: 2px;");
    }
    if (needRefresh) refreshTodoList();
}

void DesktopPet::refreshTodoList()
{
    for (int i = 0; i < m_todos.size(); ++i) {
        m_todos[i].displayLabel->setText(QString("%1. %2 (%3)")
                                             .arg(i+1)
                                             .arg(m_todos[i].name)
                                             .arg(m_todos[i].dueTime.toString("MM-dd HH:mm")));
    }
    m_todoTitleLabel->setVisible(m_todosVisible && !m_todos.isEmpty());
}

// 菜单收起/展开
void DesktopPet::toggleMenu()
{
    bool visible = m_menu->isVisible();
    m_menu->setVisible(!visible);
    updateWindowSize();
}

void DesktopPet::updateWindowSize()
{
    m_mainFrame->adjustSize();
    QPoint pos = this->pos();
    setFixedSize(m_mainFrame->sizeHint());
    m_mainFrame->setGeometry(0, 0, width(), height());
    if (pos != this->pos()) move(pos);
}
